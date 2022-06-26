#include "test_geometrychangehandler.h"
#include "geometrychangehandler.h"
#include <QFile>

class GeomChangeHandlerTestClient : public QObject
{
    Q_OBJECT
public:
    GeomChangeHandlerTestClient(GeometryChangeHandler* handler) {
        connect(handler, SIGNAL(sigWriteStreamForGeomChange()), this, SLOT(onChangeTimer()));
    }
    int getChangeTimerSigCount() { return m_changeTimerCount; }
private slots:
    void onChangeTimer() { m_changeTimerCount++; }
private:
    int m_changeTimerCount = 0;
};

static const int changeTimerMs = 10;

void test_geometrychangehandler::timerFiresOnceOnSingleChange()
{
    GeometryChangeHandler ghandler(changeTimerMs);
    GeomChangeHandlerTestClient tclient(&ghandler);
    ghandler.handleGeomChange();
    QTest::qWait(10*changeTimerMs);
    QCOMPARE(tclient.getChangeTimerSigCount(), 1);
}

void test_geometrychangehandler::timerFiresOnceOnMultipleChangeInShortSuccession()
{
    GeometryChangeHandler ghandler(changeTimerMs);
    GeomChangeHandlerTestClient tclient(&ghandler);
    ghandler.handleGeomChange();
    QTest::qWait(changeTimerMs/10);
    ghandler.handleGeomChange();
    QTest::qWait(changeTimerMs/10);
    ghandler.handleGeomChange();
    QTest::qWait(changeTimerMs/10);
    QCOMPARE(tclient.getChangeTimerSigCount(), 0);
    QTest::qWait(2*changeTimerMs);
    QCOMPARE(tclient.getChangeTimerSigCount(), 1);
}

QTEST_MAIN(test_geometrychangehandler)

#include "test_geometrychangehandler.moc"
