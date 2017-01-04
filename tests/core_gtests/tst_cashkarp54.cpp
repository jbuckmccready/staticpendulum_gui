#include "tst_cashkarp54.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <sstream>

// "Correct" reference implementations.
namespace {
// CashKarp54
template <typename SystemType, std::size_t StateSize>
inline int ref_cashKarp54(SystemType &&dxdt, std::array<double, StateSize> &x,
                          double &t, double &h, double relTol, double absTol,
                          double maxStepSize) {
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

  // remove need to increment unsigned int (unsigned int requries possible
  // checks for wrapping)
  constexpr int intSize = static_cast<int>(StateSize);

  // temp state used to store state for next k value and later used
  // for error difference
  std::array<double, StateSize> tempState;

  std::array<double, StateSize> k1;
  dxdt(x, k1, t); // fill k1

  for (int i = 0; i < intSize; ++i) {
    tempState[i] = x[i] + h * a21 * k1[i];
  }

  std::array<double, StateSize> k2;
  dxdt(tempState, k2, t + c2 * h); // fill k2

  for (int i = 0; i < intSize; ++i) {
    tempState[i] = x[i] + h * (a31 * k1[i] + a32 * k2[i]);
  }

  std::array<double, StateSize> k3;
  dxdt(tempState, k3, t + c3 * h); // fill k3

  for (int i = 0; i < intSize; ++i) {
    tempState[i] = x[i] + h * (a41 * k1[i] + a42 * k2[i] + a43 * k3[i]);
  }

  std::array<double, StateSize> k4;
  dxdt(tempState, k4, t + c4 * h); // fill k4

  for (int i = 0; i < intSize; ++i) {
    tempState[i] =
        x[i] + h * (a51 * k1[i] + a52 * k2[i] + a53 * k3[i] + a54 * k4[i]);
  }

  std::array<double, StateSize> k5;
  dxdt(tempState, k5, t + c5 * h); // fill k5
  for (int i = 0; i < intSize; ++i) {
    tempState[i] = x[i] +
                   h * (a61 * k1[i] + a62 * k2[i] + a63 * k3[i] + a64 * k4[i] +
                        a65 * k5[i]);
  }

  std::array<double, StateSize> k6;
  dxdt(tempState, k6, t + c6 * h); // fill k6

  std::array<double, StateSize> order5Solution;
  for (int i = 0; i < intSize; ++i) {
    order5Solution[i] = h * (b5th1 * k1[i] + b5th2 * k2[i] + b5th3 * k3[i] +
                             b5th4 * k4[i] + b5th5 * k5[i] + b5th6 * k6[i]);
  }
  // difference between order 4 and 5, used for error check, reusing tempState
  // variable
  for (int i = 0; i < intSize; ++i) {
    tempState[i] = h * (bDiff1 * k1[i] + bDiff2 * k2[i] + bDiff3 * k3[i] +
                        bDiff4 * k4[i] + bDiff5 * k5[i] + bDiff6 * k6[i]);
  }
  std::array<double, StateSize> potentialSolution;
  for (int i = 0; i < intSize; ++i) {
    potentialSolution[i] = x[i] + order5Solution[i];
  }
  // boost odeint syle error step sizing method
  std::array<double, StateSize> errorValueList;
  for (int i = 0; i < intSize; ++i) {
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
  for (int i = 0; i < intSize; ++i) {
    x[i] = potentialSolution[i];
  }

  // if error is small enough then increase step size
  if (maxErrorValue < 0.5) {
    h = std::min(h * std::min(0.9 * std::pow(maxErrorValue, -0.20), 5.0),
                 maxStepSize);
  }

  return 1;
}

// Pendulum System
struct ref_PendulumSystem {
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

  void operator()(const StateType &x, StateType &dxdt,
                  const double /* t */) const;
};

//! Function call that returns the derivative of the current state.
inline void ref_PendulumSystem::operator()(
    const StateType &
        x /*!< Current state input; index 0 is x position, index 1 is y position, index 2 is x velocity, and index 3 is y velocity. */,
    StateType &
        dxdt /*!< Derivative of the state, value modified by reference; follows the same indexing as the input state. */,
    const double /*t*/ /*!< Note: system has no time dependence. Parameter here to fit signature for integration.*/)
    const {
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

  dxdt[0] = x[2];
  dxdt[1] = x[3];
  dxdt[2] = (x[0] * gravityValue - drag * x[2] + xAttractionForce) / mass;
  dxdt[3] = (x[1] * gravityValue - drag * x[3] + yAttractionForce) / mass;
}
} // namespace

namespace staticpendulum {

std::ostream &operator<<(std::ostream &os, const CashKarp54TestSet &set) {
  os << "relTol: " << set.relTol << ", absTol: " << set.absTol
     << ", startingStepSize: " << set.startingStepSize
     << ", maxStepSize: " << set.maxStepSize << ", starTime: " << set.startTime
     << ", endTime: " << set.endTime;
  return os;
}

TEST_P(CashKarp54Test, pendulumSystemTests) {
  // system build up
  ref_PendulumSystem sys;
  sys.distance = 0.05;
  sys.mass = 1.0;
  sys.gravity = 9.8;
  sys.drag = 0.2;
  sys.length = 10.0;
  const double yMag = std::sqrt(1 - 0.5 * 0.5);
  sys.attractorList.emplace_back(-0.5, yMag, 1);
  sys.attractorList.emplace_back(-0.5, -yMag, 1);
  sys.attractorList.emplace_back(1.0, 0.0, 1);

  auto ref_integrator = [=](auto &system, auto &x, double &t, double &h) {
    return ref_cashKarp54(system, x, t, h, relTol, absTol, maxStepSize);
  };

  auto integrator = [=](auto &system, auto &x, double &t, double &h) {
    return cashKarp54(system, x, t, h, relTol, absTol, maxStepSize);
  };

  auto compareWithStartState = [=] (std::array<double, 4> state) {
    // make a copy of the start state
    auto expectedState = state;
    double t = startTime;
    double step = startingStepSize;
    while (t < endTime) {
      integrator(sys, state, t, step);
    }
    t = startTime;
    step = startingStepSize;
    while (t < endTime) {
      ref_integrator(sys, expectedState, t, step);
    }
    for (int i = 0; i < static_cast<int>(state.size()); ++i) {
      EXPECT_DOUBLE_EQ(state[i], expectedState[i]) << "Unequal at index: " << i;
    }
  };

  compareWithStartState({{5.0, 5.0, 0.0, 0.0}});
  compareWithStartState({{2.5, -4.5, 0.1, -0.2}});
}

// Test data
namespace {
std::vector<CashKarp54TestSet> buildCashKarp54Data() {
  // Create all combinations of these data sets to test against
  std::vector<double> tolerances = {1e-6, 5e-7, 1e-9};
  std::vector<double> startStepSizes = {0.001, 0.01};
  std::vector<double> maxStepSizes = {0.01, 0.1};
  std::vector<double> startTimes = {0.0};
  std::vector<double> endTimes = {5.0, 10.0, 15.0, 20.0};

  std::vector<CashKarp54TestSet> data;
  for (const auto absTol : tolerances) {
    for (const auto relTol : tolerances) {
      for (const auto startStepSize : startStepSizes) {
        for (const auto maxStepSize : maxStepSizes) {
          for (const auto startTime : startTimes) {
            for (const auto endTime : endTimes) {
              data.emplace_back(absTol, relTol, startStepSize, maxStepSize,
                                startTime, endTime);
            }
          }
        }
      }
    }
  }
  return data;
}

INSTANTIATE_TEST_CASE_P(CashKarp54Data, CashKarp54Test,
                        testing::ValuesIn(buildCashKarp54Data()));
}

} // namespace staticpendulum
