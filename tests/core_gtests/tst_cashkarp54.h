#include "CoreEngine/cashkarp54.h"
#include "CoreEngine/pendulummapintegrator.h"
#include "CoreEngine/pendulumsystem.h"
#include <QString>
#include <array>
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>

namespace staticpendulum {

struct CashKarp54TestSet {
  CashKarp54TestSet(double relTol, double absTol, double startingStepSize,
                    double maxStepSize, double startTime, double endTime)
      : relTol(relTol), absTol(absTol), startingStepSize(startingStepSize),
        maxStepSize(maxStepSize), startTime(startTime), endTime(endTime) {}
  // relative tolerance
  double relTol;
  // absolute tolerance
  double absTol;
  // starting step size
  double startingStepSize;
  // maximum step size
  double maxStepSize;
  // start time of integration
  double startTime;
  // snd time of integration
  double endTime;
};

// The fixture for testing the CashKarp54 implementation.
class CashKarp54Test : public testing::TestWithParam<CashKarp54TestSet> {
protected:
  virtual void SetUp() {
    relTol = GetParam().relTol;
    absTol = GetParam().absTol;
    startingStepSize = GetParam().startingStepSize;
    maxStepSize = GetParam().startingStepSize;
    startTime = GetParam().startTime;
    endTime = GetParam().endTime;
  }

  double relTol;
  double absTol;
  double startingStepSize;
  double maxStepSize;
  double startTime;
  double endTime;
};

} // namespace staticpendulum
