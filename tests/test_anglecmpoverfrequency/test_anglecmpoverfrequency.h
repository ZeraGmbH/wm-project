#ifndef TEST_ANGLECMPOVERFREQUENCY
#define TEST_ANGLECMPOVERFREQUENCY

#include <QTest>
#include "anglecmpoverfrequency.h"

class test_anglecmpoverfrequency : public QObject
{
	Q_OBJECT
private slots:
    void test_getcompValue();
    void test_frequencyRange();
    void test_angleCorrection();
};

#endif
