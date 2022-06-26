#include "test_geometrychangehandler.h"
#include "geometrychangehandler.h"
#include <QFile>

class GeomChangeHandlerTestClient : public QObject
{
    Q_OBJECT
public:
    GeomChangeHandlerTestClient(GeometryChangeHandler* handler) {
        connect(handler, SIGNAL(sigNeedsStreamWrite()), this, SLOT(onChangeTimer()));
    }
    int getChangeTimerSigCount() { return m_changeTimerCount; }
private slots:
    void onChangeTimer() { m_changeTimerCount++; }
private:
    int m_changeTimerCount = 0;
};

static const int changeTimerMs = 10;

void test_geometrychangehandler::noTimerOnSetGeom()
{
    GeometryChangeHandler ghandler(changeTimerMs);
    GeomChangeHandlerTestClient tclient(&ghandler);
    ghandler.setGeometry(WidgetGeometry());
    QTest::qWait(10*changeTimerMs);
    QCOMPARE(tclient.getChangeTimerSigCount(), 0);
}

static WidgetGeometry setWidgetGeom(int x, int y, int w, int h, bool visible) // -> class?
{
    WidgetGeometry wg;
    wg.setPoint(QPoint(x, y));
    wg.setSize(QSize(w, h));
    wg.setVisible(visible);
    return wg;
}

void test_geometrychangehandler::setGeomSetsProperly()
{
    WidgetGeometry wg = setWidgetGeom(1, 2, 3, 4, false);
    GeometryChangeHandler ghandler(changeTimerMs);
    QVERIFY(!(wg == ghandler.getGeometry()));
    ghandler.setGeometry(wg);
    QVERIFY(wg == ghandler.getGeometry());
}

void test_geometrychangehandler::timerFiresOnceOnSingleSizeChange()
{
    GeometryChangeHandler ghandler(changeTimerMs);
    GeomChangeHandlerTestClient tclient(&ghandler);
    ghandler.handleSizeChange(QSize());
    QTest::qWait(10*changeTimerMs);
    QCOMPARE(tclient.getChangeTimerSigCount(), 1);
}

void test_geometrychangehandler::timerFiresOnceOnSinglePointChange()
{
    GeometryChangeHandler ghandler(changeTimerMs);
    GeomChangeHandlerTestClient tclient(&ghandler);
    ghandler.handlePointChange(QPoint());
    QTest::qWait(10*changeTimerMs);
    QCOMPARE(tclient.getChangeTimerSigCount(), 1);
}

void test_geometrychangehandler::timerFiresOnceOnSingleVisibleChange()
{
    GeometryChangeHandler ghandler(changeTimerMs);
    GeomChangeHandlerTestClient tclient(&ghandler);
    ghandler.handleVisibleChange(false);
    QTest::qWait(10*changeTimerMs);
    QCOMPARE(tclient.getChangeTimerSigCount(), 1);
}

void test_geometrychangehandler::timerFiresOnceOnMultipleChangeInShortSuccession()
{
    GeometryChangeHandler ghandler(changeTimerMs);
    GeomChangeHandlerTestClient tclient(&ghandler);
    ghandler.handleSizeChange(QSize());
    QTest::qWait(changeTimerMs/10);
    ghandler.handlePointChange(QPoint());
    QTest::qWait(changeTimerMs/10);
    ghandler.handleVisibleChange(false);
    QTest::qWait(2*changeTimerMs);
    QCOMPARE(tclient.getChangeTimerSigCount(), 1);
}

void test_geometrychangehandler::multipleChangeCheckLastGeom()
{
    GeometryChangeHandler ghandler(changeTimerMs);
    GeomChangeHandlerTestClient tclient(&ghandler);
    WidgetGeometry wg1 = setWidgetGeom(1, 2, 3, 4, true);
    ghandler.handleSizeChange(wg1.getSize());
    ghandler.handlePointChange(wg1.getPoint());
    ghandler.handleVisibleChange(wg1.getVisible());
    QTest::qWait(changeTimerMs/10);
    QCOMPARE(tclient.getChangeTimerSigCount(), 0);
    WidgetGeometry wg2 = setWidgetGeom(5, 6, 7, 8, false);
    ghandler.handleSizeChange(wg2.getSize());
    ghandler.handlePointChange(wg2.getPoint());
    ghandler.handleVisibleChange(wg2.getVisible());
    QTest::qWait(2*changeTimerMs);
    QVERIFY(ghandler.getGeometry() == wg2);
}

void test_geometrychangehandler::streamOutIn()
{
    GeometryChangeHandler ghandlerW(changeTimerMs);
    GeomChangeHandlerTestClient tclientW(&ghandlerW);
    WidgetGeometry wg1 = setWidgetGeom(1, 2, 3, 4, true);
    ghandlerW.handleSizeChange(wg1.getSize());
    ghandlerW.handlePointChange(wg1.getPoint());
    ghandlerW.handleVisibleChange(wg1.getVisible());

    QFile file("/tmp/test_geometrychangehandler_streamOutIn");
    file.open(QFile::WriteOnly);
    QDataStream streamW(&file);
    streamW << ghandlerW;
    file.close();

    GeometryChangeHandler ghandlerR(changeTimerMs);
    GeomChangeHandlerTestClient tclientR(&ghandlerR);
    file.open(QFile::ReadOnly);
    QDataStream streamR(&file);
    streamR >> ghandlerR;
    file.close();

    QVERIFY(ghandlerR.getGeometry() == wg1);

    QTest::qWait(2*changeTimerMs);
    QCOMPARE(tclientR.getChangeTimerSigCount(), 0);
}

QTEST_MAIN(test_geometrychangehandler)

#include "test_geometrychangehandler.moc"
