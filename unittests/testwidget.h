#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QObject>

class TestWidget : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void dummyWidget();
};

#endif // TESTWIDGET_H
