#include "testwidget.h"
#include "main-unittest.h"

static QObject* pTestWidget = addTest(new TestWidget);

void TestWidget::init()
{

}

void TestWidget::cleanup()
{

}

void TestWidget::dummyWidget()
{
    QWidget widget;
    widget.resize(100,100);
    widget.show();

    QTest::qWait(1000);
    QCOMPARE(widget.isVisible(), true);

    widget.resize(200,200);
    QTest::qWait(1000);

    widget.hide();
    QTest::qWait(1000);
    QCOMPARE(widget.isVisible(), false);
}
