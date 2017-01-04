#include <QString>
#include <QtTest>

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
  QTest::addColumn<QString>("data");
  QTest::newRow("hello") << QString("hello");
}

void IntegrationBenchmarks::benchmark1()
{
  QFETCH(QString, data);
  QBENCHMARK {
    data.toUpper();
  }
}

QTEST_APPLESS_MAIN(IntegrationBenchmarks)

#include "tst_integrationbenchmarks.moc"
