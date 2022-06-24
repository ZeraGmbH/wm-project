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

void test_geometrychangehandler::timerFiresOnceOnSingleChange()
{
    GeometryChangeHandler ghandler(changeTimerMs);
    GeomChangeHandlerTestClient tclient(&ghandler);
    ghandler.handleGeometryChange(WidgetGeometry());
    QTest::qWait(10*changeTimerMs);
    QCOMPARE(tclient.getChangeTimerSigCount(), 1);
}

void test_geometrychangehandler::timerFiresOnceOnMultipleChangeInShortSuccession()
{
    GeometryChangeHandler ghandler(changeTimerMs);
    GeomChangeHandlerTestClient tclient(&ghandler);
    ghandler.handleGeometryChange(WidgetGeometry());
    QTest::qWait(changeTimerMs/10);
    ghandler.handleGeometryChange(WidgetGeometry());
    QTest::qWait(2*changeTimerMs);
    QCOMPARE(tclient.getChangeTimerSigCount(), 1);
}

static WidgetGeometry setWidgetGeom(int x, int y, int w, int h, bool visible) // -> class?
{
    WidgetGeometry wg;
    QPoint pt(x, y);
    QSize sz(w, h);
    wg.setGeometry(pt, sz);
    wg.setVisible(visible);
    return wg;
}

void test_geometrychangehandler::multipleChangeCheckLastGeom()
{
    GeometryChangeHandler ghandler(changeTimerMs);
    GeomChangeHandlerTestClient tclient(&ghandler);
    WidgetGeometry wg1 = setWidgetGeom(1, 2, 3, 4, true);
    ghandler.handleGeometryChange(wg1);
    QTest::qWait(changeTimerMs/10);
    QCOMPARE(tclient.getChangeTimerSigCount(), 0);
    WidgetGeometry wg2 = setWidgetGeom(5, 6, 7, 8, false);
    ghandler.handleGeometryChange(wg2);
    QTest::qWait(2*changeTimerMs);
    QVERIFY(ghandler.getGeometry() == wg2);
}

void test_geometrychangehandler::streamOutIn()
{
    GeometryChangeHandler ghandlerW(changeTimerMs);
    GeomChangeHandlerTestClient tclientW(&ghandlerW);
    WidgetGeometry wg1 = setWidgetGeom(1, 2, 3, 4, true);
    ghandlerW.handleGeometryChange(wg1);

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
