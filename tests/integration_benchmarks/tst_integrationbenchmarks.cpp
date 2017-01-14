#include "CoreEngine/cashkarp54.h"
#include "CoreEngine/pendulummapintegrator.h"
#include "CoreEngine/pendulumsystem.h"
#include "xmmintrin.h"
#include <QString>
#include <QtTest>
#include <array>
#include <cmath>
#include <iostream>

using namespace staticpendulum;
template <typename SystemType, std::size_t StateSize>
inline int ref_cashKarp54(SystemType &&dxdt, std::array<double, StateSize> &x,
                          double &t, double &h, double relTol, double absTol,
                          double maxStepSize) {
  // Constants from Butcher tableau, see:
  // http://en.wikipedia.org/wiki/Cash-Karp_method
  // and http://en.wikipedia.org/wiki/Runge-Kutta_methods
  constexpr double c2 = 1.0 / 5.0;
  constexpr double c3 = 3.0 / 10.0;
  constexpr double c4 = 3.0 / 5.0;
  constexpr double c5 = 1.0;
  constexpr double c6 = 7.0 / 8.0;

  constexpr double b5th1 = 37.0 / 378.0;
  constexpr double b5th2 = 0.0;
  constexpr double b5th3 = 250.0 / 621.0;
  constexpr double b5th4 = 125.0 / 594.0;
  constexpr double b5th5 = 0.0;
  constexpr double b5th6 = 512.0 / 1771.0;

  constexpr double b4th1 = 2825.0 / 27648.0;
  constexpr double b4th2 = 0.0;
  constexpr double b4th3 = 18575.0 / 48384.0;
  constexpr double b4th4 = 13525.0 / 55296.0;
  constexpr double b4th5 = 277.0 / 14336.0;
  constexpr double b4th6 = 1.0 / 4.0;

  constexpr double bDiff1 = b5th1 - b4th1;
  constexpr double bDiff2 = b5th2 - b4th2;
  constexpr double bDiff3 = b5th3 - b4th3;
  constexpr double bDiff4 = b5th4 - b4th4;
  constexpr double bDiff5 = b5th5 - b4th5;
  constexpr double bDiff6 = b5th6 - b4th6;

  constexpr double a21 = 1.0 / 5.0;
  constexpr double a31 = 3.0 / 40.0;
  constexpr double a32 = 9.0 / 40.0;
  constexpr double a41 = 3.0 / 10.0;
  constexpr double a42 = -9.0 / 10.0;
  constexpr double a43 = 6.0 / 5.0;
  constexpr double a51 = -11.0 / 54.0;
  constexpr double a52 = 5.0 / 2.0;
  constexpr double a53 = -70.0 / 27.0;
  constexpr double a54 = 35.0 / 27.0;
  constexpr double a61 = 1631.0 / 55296.0;
  constexpr double a62 = 175.0 / 512.0;
  constexpr double a63 = 575.0 / 13824.0;
  constexpr double a64 = 44275.0 / 110592.0;
  constexpr double a65 = 253.0 / 4096.0;

  // temp state used to store state for next k value and later used
  // for error difference
  std::array<double, StateSize> tempState;

  std::array<double, StateSize> k1;
  dxdt(x, k1, t); // fill k1

  for (std::size_t i = 0; i < StateSize; ++i) {
    tempState[i] = x[i] + h * a21 * k1[i];
  }

  std::array<double, StateSize> k2;
  dxdt(tempState, k2, t + c2 * h); // fill k2

  for (std::size_t i = 0; i < StateSize; ++i) {
    tempState[i] = x[i] + h * (a31 * k1[i] + a32 * k2[i]);
  }

  std::array<double, StateSize> k3;
  dxdt(tempState, k3, t + c3 * h); // fill k3

  for (std::size_t i = 0; i < StateSize; ++i) {
    tempState[i] = x[i] + h * (a41 * k1[i] + a42 * k2[i] + a43 * k3[i]);
  }

  std::array<double, StateSize> k4;
  dxdt(tempState, k4, t + c4 * h); // fill k4

  for (std::size_t i = 0; i < StateSize; ++i) {
    tempState[i] =
        x[i] + h * (a51 * k1[i] + a52 * k2[i] + a53 * k3[i] + a54 * k4[i]);
  }

  std::array<double, StateSize> k5;
  dxdt(tempState, k5, t + c5 * h); // fill k5
  for (std::size_t i = 0; i < StateSize; ++i) {
    tempState[i] = x[i] +
                   h * (a61 * k1[i] + a62 * k2[i] + a63 * k3[i] + a64 * k4[i] +
                        a65 * k5[i]);
  }

  std::array<double, StateSize> k6;
  dxdt(tempState, k6, t + c6 * h); // fill k6

  std::array<double, StateSize> order5Solution;
  for (std::size_t i = 0; i < StateSize; ++i) {
    order5Solution[i] = h * (b5th1 * k1[i] + b5th2 * k2[i] + b5th3 * k3[i] +
                             b5th4 * k4[i] + b5th5 * k5[i] + b5th6 * k6[i]);
  }
  // difference between order 4 and 5, used for error check, reusing tempState
  // variable
  for (std::size_t i = 0; i < StateSize; ++i) {
    tempState[i] = h * (bDiff1 * k1[i] + bDiff2 * k2[i] + bDiff3 * k3[i] +
                        bDiff4 * k4[i] + bDiff5 * k5[i] + bDiff6 * k6[i]);
  }
  std::array<double, StateSize> potentialSolution;
  for (std::size_t i = 0; i < StateSize; ++i) {
    potentialSolution[i] = x[i] + order5Solution[i];
  }
  // boost odeint syle error step sizing method
  std::array<double, StateSize> errorValueList;
  for (std::size_t i = 0; i < StateSize; ++i) {
    errorValueList[i] =
        std::abs(tempState[i] / (absTol + relTol * (potentialSolution[i])));
  }
  double maxErrorValue =
      *(std::max_element(errorValueList.begin(), errorValueList.end()));

  // reject step and decrease step size
  if (maxErrorValue > 1.0) {
    h = h * std::max(0.9 * std::pow(maxErrorValue, -0.25), 0.2);
    return 0;
  }

  // use the step
  t += h;
  for (std::size_t i = 0; i < StateSize; ++i) {
    x[i] = potentialSolution[i];
  }

  // if error is small enough then increase step size
  if (maxErrorValue < 0.5) {
    h = std::min(h * std::min(0.9 * std::pow(maxErrorValue, -0.20), 5.0),
                 maxStepSize);
  }

  return 1;
}

template <typename SystemType>
inline int avx_cashKarp54(SystemType &&dxdt, __m256d &x, double &t, double &h,
                          double relTol, double absTol, double maxStepSize) {
  // Constants from Butcher tableau, see:
  // http://en.wikipedia.org/wiki/Cash-Karp_method
  // and http://en.wikipedia.org/wiki/Runge-Kutta_methods
  constexpr double c2 = 1.0 / 5.0;
  constexpr double c3 = 3.0 / 10.0;
  constexpr double c4 = 3.0 / 5.0;
  constexpr double c5 = 1.0;
  constexpr double c6 = 7.0 / 8.0;

  constexpr double b5th1 = 37.0 / 378.0;
  constexpr double b5th2 = 0.0;
  constexpr double b5th3 = 250.0 / 621.0;
  constexpr double b5th4 = 125.0 / 594.0;
  constexpr double b5th5 = 0.0;
  constexpr double b5th6 = 512.0 / 1771.0;

  const __m256d b5th1_256 = _mm256_set1_pd(b5th1);
  const __m256d b5th2_256 = _mm256_set1_pd(b5th2);
  const __m256d b5th3_256 = _mm256_set1_pd(b5th3);
  const __m256d b5th4_256 = _mm256_set1_pd(b5th4);
  const __m256d b5th5_256 = _mm256_set1_pd(b5th5);
  const __m256d b5th6_256 = _mm256_set1_pd(b5th6);

  constexpr double b4th1 = 2825.0 / 27648.0;
  constexpr double b4th2 = 0.0;
  constexpr double b4th3 = 18575.0 / 48384.0;
  constexpr double b4th4 = 13525.0 / 55296.0;
  constexpr double b4th5 = 277.0 / 14336.0;
  constexpr double b4th6 = 1.0 / 4.0;

  constexpr double bDiff1 = b5th1 - b4th1;
  constexpr double bDiff2 = b5th2 - b4th2;
  constexpr double bDiff3 = b5th3 - b4th3;
  constexpr double bDiff4 = b5th4 - b4th4;
  constexpr double bDiff5 = b5th5 - b4th5;
  constexpr double bDiff6 = b5th6 - b4th6;

  const __m256d bDiff1_256 = _mm256_set1_pd(bDiff1);
  const __m256d bDiff2_256 = _mm256_set1_pd(bDiff2);
  const __m256d bDiff3_256 = _mm256_set1_pd(bDiff3);
  const __m256d bDiff4_256 = _mm256_set1_pd(bDiff4);
  const __m256d bDiff5_256 = _mm256_set1_pd(bDiff5);
  const __m256d bDiff6_256 = _mm256_set1_pd(bDiff6);

  constexpr double a21 = 1.0 / 5.0;
  constexpr double a31 = 3.0 / 40.0;
  constexpr double a32 = 9.0 / 40.0;
  constexpr double a41 = 3.0 / 10.0;
  constexpr double a42 = -9.0 / 10.0;
  constexpr double a43 = 6.0 / 5.0;
  constexpr double a51 = -11.0 / 54.0;
  constexpr double a52 = 5.0 / 2.0;
  constexpr double a53 = -70.0 / 27.0;
  constexpr double a54 = 35.0 / 27.0;
  constexpr double a61 = 1631.0 / 55296.0;
  constexpr double a62 = 175.0 / 512.0;
  constexpr double a63 = 575.0 / 13824.0;
  constexpr double a64 = 44275.0 / 110592.0;
  constexpr double a65 = 253.0 / 4096.0;

  const __m256d a21_256 = _mm256_set1_pd(a21);
  const __m256d a31_256 = _mm256_set1_pd(a31);
  const __m256d a32_256 = _mm256_set1_pd(a32);
  const __m256d a41_256 = _mm256_set1_pd(a41);
  const __m256d a42_256 = _mm256_set1_pd(a42);
  const __m256d a43_256 = _mm256_set1_pd(a43);
  const __m256d a51_256 = _mm256_set1_pd(a51);
  const __m256d a52_256 = _mm256_set1_pd(a52);
  const __m256d a53_256 = _mm256_set1_pd(a53);
  const __m256d a54_256 = _mm256_set1_pd(a54);
  const __m256d a61_256 = _mm256_set1_pd(a61);
  const __m256d a62_256 = _mm256_set1_pd(a62);
  const __m256d a63_256 = _mm256_set1_pd(a63);
  const __m256d a64_256 = _mm256_set1_pd(a64);
  const __m256d a65_256 = _mm256_set1_pd(a65);

  const __m256d h_256 = _mm256_set1_pd(h);

  // temp state used to store state for next k value and later used
  // for error difference
  __m256d tempState;

  __m256d k1;
  dxdt(x, k1, t); // fill k1
//  std::cout << "k1: " << k1[0] << ", " << k1[1] << ", " << k1[2] << ", " <<
//  k1[3] << '\n';

//  for (std::size_t i = 0; i < StateSize; ++i) {
//    tempState[i] = x[i] + h * a21 * k1[i];
//  }
  tempState = _mm256_mul_pd(a21_256, k1);
  tempState = _mm256_fmadd_pd(tempState, h_256, x);

  __m256d k2;
  dxdt(tempState, k2, std::fma(c2, h, t)); // fill k2
//  std::cout << "k2: " << k2[0] << ", " << k2[1] << ", " << k2[2] << ", " <<
//  k2[3] << '\n';

//  for (std::size_t i = 0; i < StateSize; ++i) {
//    tempState[i] = x[i] + h * (a31 * k1[i] + a32 * k2[i]);
//  }
  tempState = _mm256_mul_pd(a31_256, k1);
  tempState = _mm256_fmadd_pd(a32_256, k2, tempState);
  tempState = _mm256_fmadd_pd(tempState, h_256, x);

  __m256d k3;
  dxdt(tempState, k3, std::fma(c3, h, t)); // fill k3
//  std::cout << "k3: " << k3[0] << ", " << k3[1] << ", " << k3[2] << ", " <<
//  k3[3] << '\n';

//  for (std::size_t i = 0; i < StateSize; ++i) {
//    tempState[i] = x[i] + h * (a41 * k1[i] + a42 * k2[i] + a43 * k3[i]);
//  }
  tempState = _mm256_mul_pd(a41_256, k1);
  tempState = _mm256_fmadd_pd(a42_256, k2, tempState);
  tempState = _mm256_fmadd_pd(a43_256, k3, tempState);
  tempState = _mm256_fmadd_pd(tempState, h_256, x);

  __m256d k4;
  dxdt(tempState, k4, std::fma(c4, h, t)); // fill k4
//  std::cout << "k4: " << k4[0] << ", " << k4[1] << ", " << k4[2] << ", " <<
//  k4[3] << '\n';

  //  for (std::size_t i = 0; i < StateSize; ++i) {
  //    tempState[i] =
  //        x[i] + h * (a51 * k1[i] + a52 * k2[i] + a53 * k3[i] + a54 * k4[i]);
  //  }
  tempState = _mm256_mul_pd(a51_256, k1);
  tempState = _mm256_fmadd_pd(a52_256, k2, tempState);
  tempState = _mm256_fmadd_pd(a53_256, k3, tempState);
  tempState = _mm256_fmadd_pd(a54_256, k4, tempState);
  tempState = _mm256_fmadd_pd(tempState, h_256, x);

  __m256d k5;
  dxdt(tempState, k5, std::fma(c5, h, t)); // fill k5
//  std::cout << "k5: " << k5[0] << ", " << k5[1] << ", " << k5[2] << ", " <<
//  k5[3] << '\n';

  //  for (std::size_t i = 0; i < StateSize; ++i) {
  //    tempState[i] = x[i] +
  //                   h * (a61 * k1[i] + a62 * k2[i] + a63 * k3[i] + a64 *
  //                   k4[i] +
  //                        a65 * k5[i]);
  //  }
  tempState = _mm256_mul_pd(a61_256, k1);
  tempState = _mm256_fmadd_pd(a62_256, k2, tempState);
  tempState = _mm256_fmadd_pd(a63_256, k3, tempState);
  tempState = _mm256_fmadd_pd(a64_256, k4, tempState);
  tempState = _mm256_fmadd_pd(a65_256, k5, tempState);
  tempState = _mm256_fmadd_pd(tempState, h_256, x);

  __m256d k6;
  dxdt(tempState, k6, std::fma(c6, h, t)); // fill k6
//  std::cout << "k6: " << k6[0] << ", " << k6[1] << ", " << k6[2] << ", " <<
//  k6[3] << '\n';

  //  for (std::size_t i = 0; i < StateSize; ++i) {
  //    order5Solution[i] = h * (b5th1 * k1[i] + b5th2 * k2[i] + b5th3 * k3[i] +
  //                             b5th4 * k4[i] + b5th5 * k5[i] + b5th6 * k6[i]);
  //  }
  __m256d order5Solution = _mm256_mul_pd(b5th1_256, k1);
  order5Solution = _mm256_fmadd_pd(b5th2_256, k2, order5Solution);
  order5Solution = _mm256_fmadd_pd(b5th3_256, k3, order5Solution);
  order5Solution = _mm256_fmadd_pd(b5th4_256, k4, order5Solution);
  order5Solution = _mm256_fmadd_pd(b5th5_256, k5, order5Solution);
  order5Solution = _mm256_fmadd_pd(b5th6_256, k6, order5Solution);
  order5Solution = _mm256_mul_pd(order5Solution, h_256);
//  std::cout << "order5_sol: " << order5Solution[0] << ", " << order5Solution[1] << ", " << order5Solution[2] << ", " <<
//  order5Solution[3] << '\n';

  // difference between order 4 and 5, used for error check, reusing tempState
  // variable
  //  for (std::size_t i = 0; i < StateSize; ++i) {
  //    tempState[i] = h * (bDiff1 * k1[i] + bDiff2 * k2[i] + bDiff3 * k3[i] +
  //                        bDiff4 * k4[i] + bDiff5 * k5[i] + bDiff6 * k6[i]);
  //  }
  tempState = _mm256_mul_pd(bDiff1_256, k1);
  tempState = _mm256_fmadd_pd(bDiff2_256, k2, tempState);
  tempState = _mm256_fmadd_pd(bDiff3_256, k3, tempState);
  tempState = _mm256_fmadd_pd(bDiff4_256, k4, tempState);
  tempState = _mm256_fmadd_pd(bDiff5_256, k5, tempState);
  tempState = _mm256_fmadd_pd(bDiff6_256, k6, tempState);
  tempState = _mm256_mul_pd(tempState, h_256);
//  std::cout << "diff54: " << tempState[0] << ", " << tempState[1] << ", " << tempState[2] << ", " <<
//  tempState[3] << '\n';

  //  for (std::size_t i = 0; i < StateSize; ++i) {
  //    potentialSolution[i] = x[i] + order5Solution[i];
  //  }
  __m256d potentialSolution_256 = _mm256_add_pd(x, order5Solution);

  // extract to doubles
  double potentialSolution[4];
  __m128d a = _mm256_extractf128_pd(potentialSolution_256, 0);
  __m128d b = _mm256_extractf128_pd(potentialSolution_256, 1);

  _mm_storel_pd(&potentialSolution[0], a);
  _mm_storeh_pd(&potentialSolution[1], a);
  _mm_storel_pd(&potentialSolution[2], b);
  _mm_storeh_pd(&potentialSolution[3], b);
//  std::cout << "potential_sol: " << potentialSolution[0] << ", " << potentialSolution[1] << ", " << potentialSolution[2] << ", " <<
//  potentialSolution[3] << '\n';

  //  __m256d error = _mm256_fmadd_pd(relTol, potentialSolution, absTol);
  //  error = _mm256_div_pd(tempState, error);

  // boost odeint syle error step sizing method
  std::array<double, 4> errorValueList;
  for (std::size_t i = 0; i < 4; ++i) {
    errorValueList[i] =
        std::abs(tempState[i] / (absTol + relTol * (potentialSolution[i])));
  }
  double maxErrorValue =
      *(std::max_element(errorValueList.begin(), errorValueList.end()));

  // reject step and decrease step size
  if (maxErrorValue > 1.0) {
    h = h * std::max(0.9 * std::pow(maxErrorValue, -0.25), 0.2);
    return 0;
  }

  // use the step
  t += h;
  x = potentialSolution_256;
  //  for (std::size_t i = 0; i < StateSize; ++i) {
  //    x[i] = potentialSolution[i];
  //  }

  // if error is small enough then increase step size
  if (maxErrorValue < 0.5) {
    h = std::min(h * std::min(0.9 * std::pow(maxErrorValue, -0.20), 5.0),
                 maxStepSize);
  }

  return 1;
}

struct avx_PendulumSystem {
  typedef std::array<double, 4> StateType; // container type for the state

  //! Container for an attractor, stores the position as an x-y coordinate, and
  //! an attractive force coefficient.
  struct Attractor {
    Attractor(double xPos, double yPos, double forceCoeff)
        : xPosition(xPos), yPosition(yPos), forceCoeff(forceCoeff) {}
    double xPosition; /*!< x coordinate position. */
    double yPosition; /*!< y coordinate position. */
    double
        forceCoeff; /*!< Attractive force coefficient where \f$F_{attractor} =
                       \frac{-k}{x^2+y^2}\f$ */
  };

  double distance; /*!< Distance between the pendulum head at rest and the base
                      plate. */
  double mass;     /*!< Mass of the head of the pendulum. */
  double gravity;  /*!< Acceleration due to gravity. */
  double drag;     /*!< Linear drag coefficient. */
  double length;   /*!< Length of the pendulum. */
  std::vector<Attractor>
      attractorList; /*!< List of attractors for the system. */

  void operator()(__m256d x, __m256d &dxdt, const double /* t */) const;
};

//! Function call that returns the derivative of the current state.
inline void avx_PendulumSystem::operator()(__m256d x, __m256d &dxdt,
                                           const double /*t*/) const {
  // see latex equation or readme for more readable math, this is coded to
  // minimize repeated calculations
  const double xSquared = x[0] * x[0];
  const double ySquared = x[1] * x[1];
  const double lengthSquared = length * length;
  const double normSquared = xSquared + ySquared;
  const double sqrtTerm = std::sqrt(1.0 - normSquared / lengthSquared);

  const double gravityValue = -mass * gravity / length * sqrtTerm;

  double xAttractionForce = 0.0;
  double yAttractionForce = 0.0;

  const double value1 = distance + length * (1.0 - sqrtTerm);
  const double value2 = value1 * value1;

  // sum up all the attractor forces
  for (auto attractor : attractorList) {
    double value3 = x[0] - attractor.xPosition;
    double value4 = x[1] - attractor.yPosition;
    double value5 = value3 * value3;
    double value6 = value4 * value4;
    double value7 =
        -attractor.forceCoeff / std::pow(value5 + value6 + value2, 1.5);

    xAttractionForce += value3 * value7;
    yAttractionForce += value4 * value7;
  }

  //  double tmp[4] __attribute__((aligned(32))) = {
  //      x[2], x[3], (x[0] * gravityValue - drag * x[2] + xAttractionForce) /
  //      mass,
  //      (x[1] * gravityValue - drag * x[3] + yAttractionForce) / mass};
  //  std::cout << tmp[0] << ", " << tmp[1] << ", " << tmp[2] << ", " << tmp[3]
  //  << '\n';
  //  dxdt = _mm256_load_pd(&tmp[0]);
  //  dxdt = _mm256_set1_pd(tmp[0]);

  dxdt[0] = x[2];
  dxdt[1] = x[3];
  dxdt[2] = (x[0] * gravityValue - drag * x[2] + xAttractionForce) / mass;
  dxdt[3] = (x[1] * gravityValue - drag * x[3] + yAttractionForce) / mass;
}

Q_DECLARE_METATYPE(PendulumSystem);
using StateType = std::array<double, 4>;
Q_DECLARE_METATYPE(StateType);

Q_DECLARE_METATYPE(avx_PendulumSystem);
Q_DECLARE_METATYPE(__m256d);

class IntegrationBenchmarks : public QObject {
  Q_OBJECT

public:
  IntegrationBenchmarks();

private Q_SLOTS:
//  void ref_benchmark_data();
//  void ref_benchmark();
  void benchmark_data();
  void benchmark();
  void avx_benchmark_data();
  void avx_benchmark();
};

IntegrationBenchmarks::IntegrationBenchmarks() {}

//void IntegrationBenchmarks::ref_benchmark_data() {
//  QTest::addColumn<StateType>("state");
//  QTest::addColumn<PendulumSystem>("system");
//  PendulumSystem sys;
//  const double yMag = std::sqrt(1 - 0.5 * 0.5);
//  sys.addAttractor(-0.5, yMag, 1);
//  sys.addAttractor(-0.5, -yMag, 1);
//  sys.addAttractor(1.0, 0.0, 1);
//  sys.addAttractor(1.0, 1.0, 1);
//  QTest::newRow("test1") << StateType{{1.0, 1.0, 0.0, 0.0}} << sys;
//  QTest::newRow("test2") << StateType{{5.0, 5.0, 0.0, 0.0}} << sys;
//  QTest::newRow("test3") << StateType{{2.5, -4.5, 0.1, -0.2}} << sys;
//  QTest::newRow("test2") << StateType{{1.0, 0.0, 0.0, 0.0}} << sys;
//  QTest::newRow("test2") << StateType{{0.0, 1.0, 0.0, 0.0}} << sys;
//}

//void IntegrationBenchmarks::ref_benchmark() {
//  QFETCH(StateType, state);
//  QFETCH(PendulumSystem, system);
//  QBENCHMARK {
//    const double endTime = 20.0;
//    double t = 0.0;
//    double step = 0.01;
//    while (t < endTime) {
//      ref_cashKarp54(system, state, t, step, 1e-7, 1e-7, 0.1);
//    }
//  }
//}

void IntegrationBenchmarks::benchmark_data() {
//  QTest::addColumn<StateType>("state");
//  QTest::addColumn<PendulumSystem>("system");
//  PendulumSystem sys;
//  const double yMag = std::sqrt(1 - 0.5 * 0.5);
//  sys.addAttractor(-0.5, yMag, 1);
//  sys.addAttractor(-0.5, -yMag, 1);
//  sys.addAttractor(1.0, 0.0, 1);
//  sys.addAttractor(1.0, 1.0, 1);
//  QTest::newRow("test1") << StateType{{1.0, 1.0, 0.0, 0.0}} << sys;
//  QTest::newRow("test2") << StateType{{5.0, 5.0, 0.0, 0.0}} << sys;
//  QTest::newRow("test3") << StateType{{2.5, -4.5, 0.1, -0.2}} << sys;
//  QTest::newRow("test2") << StateType{{1.0, 0.0, 0.0, 0.0}} << sys;
//  QTest::newRow("test2") << StateType{{0.0, 1.0, 0.0, 0.0}} << sys;
}

void IntegrationBenchmarks::benchmark() {
  PendulumSystem system;
  system.distance = 0.05;
  system.mass = 1.0;
  system.gravity = 9.8;
  system.drag = 0.2;
  system.length = 10.0;
  const double yMag = std::sqrt(1 - 0.5 * 0.5);
  system.addAttractor(-0.5, yMag, 1);
  system.addAttractor(-0.5, -yMag, 1);
  system.addAttractor(1.0, 0.0, 1);

//  QFETCH(StateType, state);
//  QFETCH(PendulumSystem, system);
  QBENCHMARK {
  std::array<double, 4> state = {{5.0, 3.0, 0.0, 0.0}};
    const double endTime = 20.0;
    double t = 0.0;
    double step = 0.01;
    while (t < endTime) {
      cashKarp54(system, state, t, step, 1e-7, 1e-7, 0.1);
    }
  }
//    std::cout << state[0] << ", " << state[1] << ", " << state[2] << ", " <<
//    state[3] << '\n';

}

void IntegrationBenchmarks::avx_benchmark_data() {
  //  QTest::addColumn<__m256d>("state");
  //  QTest::addColumn<avx_PendulumSystem>("system");
  //  avx_PendulumSystem sys;
  //  const double yMag = std::sqrt(1 - 0.5 * 0.5);
  //  sys.attractorList.emplace_back(-0.5, yMag, 1);
  //  sys.attractorList.emplace_back(-0.5, -yMag, 1);
  //  sys.attractorList.emplace_back(1.0, 0.0, 1);
  //  sys.attractorList.emplace_back(1.0, 1.0, 1);
  //  QTest::newRow("test1") << StateType{{1.0, 1.0, 0.0, 0.0}} << sys;
  //  QTest::newRow("test2") << StateType{{5.0, 5.0, 0.0, 0.0}} << sys;
  //  QTest::newRow("test3") << StateType{{2.5, -4.5, 0.1, -0.2}} << sys;
  //  QTest::newRow("test2") << StateType{{1.0, 0.0, 0.0, 0.0}} << sys;
  //  QTest::newRow("test2") << StateType{{0.0, 1.0, 0.0, 0.0}} << sys;
}

void IntegrationBenchmarks::avx_benchmark() {
  //  QFETCH(StateType, state);
  //  QFETCH(PendulumSystem, system);
  avx_PendulumSystem sys;
  sys.distance = 0.05;
  sys.mass = 1.0;
  sys.gravity = 9.8;
  sys.drag = 0.2;
  sys.length = 10.0;
  const double yMag = std::sqrt(1 - 0.5 * 0.5);
  sys.attractorList.emplace_back(-0.5, yMag, 1);
  sys.attractorList.emplace_back(-0.5, -yMag, 1);
  sys.attractorList.emplace_back(1.0, 0.0, 1);

  QBENCHMARK {
  __m256d state = {5.0, 3.0, 0.0, 0.0};
    const double endTime = 20.0;
    double t = 0.0;
    double step = 0.01;
    while (t < endTime) {
      avx_cashKarp54(sys, state, t, step, 1e-7, 1e-7, 0.1);
    }
  }
//    std::cout << state[0] << ", " << state[1] << ", " << state[2] << ", " <<
//    state[3] << '\n';
}

QTEST_APPLESS_MAIN(IntegrationBenchmarks)

#include "tst_integrationbenchmarks.moc"
