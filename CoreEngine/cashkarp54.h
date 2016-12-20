/* ===========================================================================
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Jedidiah Buck McCready <jbuckmccready@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * ===========================================================================*/
#ifndef CASHKARP54_H
#define CASHKARP54_H
#include <algorithm>
#include <array>
#include <cmath>

namespace staticpendulum {
/*!
 * @brief Cash and Karp embedded Runge Kutta order 5(4) adaptive step
 *integrator.
 *
 * See: J. R. Cash, A. H. Karp. “A Variable Order Runge-Kutta Method for Initial
 *Value Problems with
 * Rapidly Varying Right-Hand Sides.” ACM Transactions on Mathematical Software,
 *Vol. 16, StateSizeo. 3, 1990.
 *
 * And Wikipedia: http://en.wikipedia.org/wiki/Cash-Karp_method,
 *http://en.wikipedia.org/wiki/List_of_Runge-Kutta_methods
 * @tparam SystemType The type for the system being integrated, must be a
 *callable object that accepts a std::array<double, StateSize> as the current
 *state, a std::array<double, StateSize>& to write the state after performing
 *the step, and a double to represent the time value to perform the step at.
 * @tparam StateSize The length of the std::array holding the state.
 * @param[in] dxdt The system being integrated as a callable object to perform a
 *step.
 * @param[in,out] x The state to be integrated forward one step.
 * @param[in,out] t Current time of the system that will be updated after taking
 *the step.
 * @param[in,out] h The step size to take, this is updated after taking the
 *step.
 * @param[in] relTol The relative tolerance allowed, this affects how the step
 *size is adjusted.
 * @param[in] absTol The absolute tolerance allowed, this affects how the step
 *size is adjusted.
 * @param[in] maxStepSize The maximum step size allowed, h will not grow larger
 *than this value.
 * @return 0 if the step size created too much error and did not perform the
 *step but adjusted the step size, 1 if step size was accepted and step was
 *performed.
 */

template <typename SystemType, std::size_t StateSize>
inline int cashKarp54(SystemType &&dxdt, std::array<double, StateSize> &x,
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
} // namespace staticpendulum
#endif // CASHKARP54_H
