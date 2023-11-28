#ifndef TEST_CMPACTVALUE
#define TEST_CMPACTVALUE

#include <QTest>
#include "cmpactvalues.h"

class test_cmpactvalue : public QObject
{
	Q_OBJECT
private slots:
    void test_phaseShift();
    void test_timeShift();
};

#endif
