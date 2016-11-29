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
#ifndef PENDULUMMAPINTEGRATOR_H
#define PENDULUMMAPINTEGRATOR_H
#include "pendulumsystem.h"
#include <vector>

namespace staticpendulum {
/// Struct used to store point information.
struct Point {
  double xPosition;
  double yPosition;
  double xVelocity;
  double yVelocity;
  int convergePosition = -2; // -2 reserved for points that are out of bounds,
                             // -1 for points that converge to the middle
  double convergeTime = 0.0;
  int stepCount = 0;
};

/// Struct used to represent a 2D map of points.
struct Map {
public:
  /// Default constructor empty map.
  Map() {}

  /// Constructs map with Points xPosition and yPosition initialized according
  /// to the ranges and resolution given.
  Map(double xStart, double yStart, double xEnd, double yEnd,
      double resolution);
  std::size_t rows() const { return m_rows; }
  std::size_t cols() const { return m_cols; }
  double xStart() const { return m_xStart; }
  double yStart() const { return m_yStart; }
  double xEnd() const { return m_xEnd; }
  double yEnd() const { return m_yEnd; }
  double resolution() const { return m_resolution; }
  auto begin() { return m_mapData.begin(); }
  auto end() { return m_mapData.end(); }

private:
  std::size_t m_rows;
  std::size_t m_cols;
  double m_xStart;
  double m_yStart;
  double m_xEnd;
  double m_yEnd;
  double m_resolution;
  std::vector<Point> m_mapData;
};

namespace impl_detail {
inline bool isNearAttractor(double attX, double attY, double currX,
                            double currY, double threshold) {
  return (attX - threshold < currX) && (currX < attX + threshold) &&
         (attY - threshold < currY) && (currY < attY + threshold);
}

inline bool isNearMiddle(double currX, double currY, double threshold) {
  return (-threshold < currX) && (currX < threshold) && (-threshold < currY) &&
         (currY < threshold);
}
}

template <typename Integrator>
inline void
integratePoint(Integrator &&theIntegrator, const PendulumSystem &theSystem,
               Point &thePoint, double startingStepSize,
               double attractorPositionThreshold, double midPositionThreshold,
               double convergeTimeThreshold) {

  using namespace impl_detail;
  // check if the point is within the pendulum length boundary
  if (std::sqrt(std::pow(thePoint.xPosition, 2) +
                std::pow(thePoint.yPosition, 2)) > (theSystem.length - 1e-10))
    return;

  // check if the point is (0,0) as it is undefined by our pendulum system
  if (std::abs(thePoint.xPosition) < 1e-10 &&
      std::abs(thePoint.yPosition) < 1e-10)
    return;

  // integration good to go, create local state for integration to keep start
  // state
  PendulumSystem::StateType current_state = {
      {thePoint.xPosition, thePoint.yPosition, thePoint.xVelocity,
       thePoint.yVelocity}};

  double currTime = 0.0;
  double stepSize = startingStepSize;
  int trialCount = 0;
  bool converged = false;
  double initialTimeFound = 0.0;
  bool nearAttractor = false; // start not near any attractor
  int currentAttractor = -2;
  while (!converged && trialCount < 1000000) {
    thePoint.stepCount +=
        theIntegrator(theSystem, current_state, currTime, stepSize);
    ++trialCount;

    // check if pendulum head near an attractor and if it's been near for long
    // enough time to consider converged
    for (int i = 0,
             attractorCount = static_cast<int>(theSystem.attractorList.size());
         i < attractorCount; ++i) {
      if (isNearAttractor(theSystem.attractorList[i].xPosition,
                          theSystem.attractorList[i].yPosition,
                          current_state[0], current_state[1],
                          attractorPositionThreshold)) {
        nearAttractor = true;
        if (currentAttractor == i) {
          if (currTime - initialTimeFound > convergeTimeThreshold) {
            thePoint.convergeTime = currTime;
            thePoint.convergePosition = i;
            converged = true;
          }
        } else {
          currentAttractor = i;
          initialTimeFound = currTime;
        }
        break; // escape for-loop after finding that we're near an attractor
      }
    }
    if (!nearAttractor) {
      // check if pendulum head near middle
      if (isNearMiddle(current_state[0], current_state[1],
                       midPositionThreshold)) {
        if (currentAttractor == -1) {
          if (currTime - initialTimeFound > convergeTimeThreshold) {
            thePoint.convergeTime = currTime;
            thePoint.convergePosition = -1;
            converged = true;
          }
        } else {
          currentAttractor = -1;
          initialTimeFound = currTime;
        }
      }
    }
    // ELSE: not near middle or any attractor, just go to next step
    nearAttractor = false; // set flag for next step
  }
}

} // namespace staticpendulum

#endif // PENDULUMMAPINTEGRATOR_H
