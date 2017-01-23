#include <QString>
#include <QtTest>
#include <array>
#include "CoreEngine/cashkarp54.h"
#include "CoreEngine/pendulummapintegrator.h"
#include "CoreEngine/pendulumsystem.h"
#include <cmath>

using namespace staticpendulum;

Q_DECLARE_METATYPE(PendulumSystem);
using StateType = std::array<double, 4>;
Q_DECLARE_METATYPE(StateType);

class IntegrationBenchmarks : public QObject
{
  Q_OBJECT

public:
  IntegrationBenchmarks();

private Q_SLOTS:
  void benchmark1_data();
  void benchmark1();
};

IntegrationBenchmarks::IntegrationBenchmarks()
{
}

void IntegrationBenchmarks::benchmark1_data()
{
  QTest::addColumn<StateType>("state");
  QTest::addColumn<PendulumSystem>("system");
  PendulumSystem sys;
  const double yMag = std::sqrt(1 - 0.5 * 0.5);
  sys.attractorList.emplace_back(-0.5, yMag, 1);
  sys.attractorList.emplace_back(-0.5, -yMag, 1);
  sys.attractorList.emplace_back(1.0, 0.0, 1);
  sys.attractorList.emplace_back(1.0, 1.0, 1);
  QTest::newRow("test1") << StateType {{ 1.0, 1.0, 0.0, 0.0 }} << sys;
  QTest::newRow("test2") << StateType {{ 5.0, 5.0, 0.0, 0.0 }} << sys;
  QTest::newRow("test3") << StateType {{ 2.5, -4.5, 0.1, -0.2 }} << sys;
}

void IntegrationBenchmarks::benchmark1()
{
  QFETCH(StateType, state);
  QFETCH(PendulumSystem, system);
  QBENCHMARK {
    const double endTime = 20.0;
    double t = 0.0;
    double step = 0.01;
    while (t < endTime) {
      cashKarp54(system, state, t, step, 1e-7, 1e-7, 0.1);
    }
  }
}

QTEST_APPLESS_MAIN(IntegrationBenchmarks)

#include "tst_integrationbenchmarks.moc"
