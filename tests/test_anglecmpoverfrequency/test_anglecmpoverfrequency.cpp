#include "test_anglecmpoverfrequency.h"

void test_anglecmpoverfrequency::test_getcompValue()
{
    double val;
    angleCmpOverFrequency test;
    val = test.getCompValue(80,50);
    QCOMPARE( qFuzzyCompare(val,static_cast<double>(0.656/180*M_PI)), true);
    val = test.getCompValue(80,60);
    QCOMPARE( qFuzzyCompare(val,static_cast<double>(0.546/180*M_PI)), true);
    val = test.getCompValue(256,50);
    QCOMPARE( qFuzzyCompare(val,static_cast<double>(0.789/180*M_PI)), true);
    val = test.getCompValue(256,60);
    QCOMPARE( qFuzzyCompare(val,static_cast<double>(0.649/180*M_PI)), true);
    val = test.getCompValue(96,50);
    QCOMPARE( qFuzzyCompare(val,static_cast<double>(0.0)), true);

}

void test_anglecmpoverfrequency::test_frequencyRange()
{
    bool ok;
    angleCmpOverFrequency test;
    ok = test.isFreuqencyInRange(12.0);
    QCOMPARE( ok, false);
    ok = test.isFreuqencyInRange(66.0);
    QCOMPARE( ok, false);
    ok = test.isFreuqencyInRange(45.0);
    QCOMPARE( ok, true);
    ok = test.isFreuqencyInRange(55.0);
    QCOMPARE( ok, true);
}

void test_anglecmpoverfrequency::test_angleCorrection()
{
    double val;
    angleCmpOverFrequency test;
    test.getCompValue(80,50);
    val = test.correctAngle(1.0,50.1,50);
    QCOMPARE( qFuzzyCompare(val,static_cast<double>(1.0+(0.1*0.656/180*M_PI))), true);

    test.getCompValue(80,60);
    val = test.correctAngle(-1.0,55.0,60.0);
    QCOMPARE( qFuzzyCompare(val,static_cast<double>(-1.0+(-5.0*0.546/180*M_PI))), true);

    test.getCompValue(256,50);
    val = test.correctAngle(1.0,55.0,50);
    QCOMPARE( qFuzzyCompare(val,static_cast<double>(1.0+(5.0*0.789/180*M_PI))), true);

    test.getCompValue(256,60);
    val = test.correctAngle(-1.0,55.0,60.0);
    QCOMPARE( qFuzzyCompare(val,static_cast<double>(-1.0+(-5.0*0.649/180*M_PI))), true);
}

QTEST_MAIN(test_anglecmpoverfrequency)
