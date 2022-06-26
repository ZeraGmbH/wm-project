#include "test_geometrychangetimer.h"
#include "geometrychangetimer.h"
#include <QFile>

class GeomChangeHandlerTestClient : public QObject
{
    Q_OBJECT
public:
    GeomChangeHandlerTestClient(GeometryChangeTimer* handler) {
        connect(handler, SIGNAL(sigWriteStreamForGeomChange()), this, SLOT(onChangeTimer()));
    }
    int getChangeTimerSigCount() { return m_changeTimerCount; }
private slots:
    void onChangeTimer() { m_changeTimerCount++; }
private:
    int m_changeTimerCount = 0;
};

static const int changeTimerMs = 10;

void test_geometrychangetimer::timerFiresOnceOnSingleChange()
{
    GeometryChangeTimer ghandler(changeTimerMs);
    GeomChangeHandlerTestClient tclient(&ghandler);
    ghandler.handleGeomChange();
    QTest::qWait(10*changeTimerMs);
    QCOMPARE(tclient.getChangeTimerSigCount(), 1);
}

void test_geometrychangetimer::timerFiresOnceOnMultipleChangeInShortSuccession()
{
    GeometryChangeTimer ghandler(changeTimerMs);
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

QTEST_MAIN(test_geometrychangetimer)

#include "test_geometrychangetimer.moc"
