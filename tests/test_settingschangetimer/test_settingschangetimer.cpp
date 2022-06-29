#include "test_settingschangetimer.h"
#include "settingschangetimer.h"
#include <QFile>

class GeomChangeHandlerTestClient : public QObject
{
    Q_OBJECT
public:
    GeomChangeHandlerTestClient(SettingsChangeTimer* handler) {
        connect(handler, SIGNAL(sigWriteStreamForGeomChange()), this, SLOT(onChangeTimer()));
    }
    int getChangeTimerSigCount() { return m_changeTimerCount; }
private slots:
    void onChangeTimer() { m_changeTimerCount++; }
private:
    int m_changeTimerCount = 0;
};

static const int changeTimerMs = 10;

void test_settingschangetimer::timerFiresOnceOnSingleChange()
{
    SettingsChangeTimer ghandler(changeTimerMs);
    GeomChangeHandlerTestClient tclient(&ghandler);
    ghandler.startDelayed();
    QTest::qWait(10*changeTimerMs);
    QCOMPARE(tclient.getChangeTimerSigCount(), 1);
}

void test_settingschangetimer::timerFiresOnceOnMultipleChangeInShortSuccession()
{
    SettingsChangeTimer ghandler(changeTimerMs);
    GeomChangeHandlerTestClient tclient(&ghandler);
    ghandler.startDelayed();
    QTest::qWait(changeTimerMs/10);
    ghandler.startDelayed();
    QTest::qWait(changeTimerMs/10);
    ghandler.startDelayed();
    QTest::qWait(changeTimerMs/10);
    QCOMPARE(tclient.getChangeTimerSigCount(), 0);
    QTest::qWait(2*changeTimerMs);
    QCOMPARE(tclient.getChangeTimerSigCount(), 1);
}

QTEST_MAIN(test_settingschangetimer)

#include "test_settingschangetimer.moc"
