#include "test_phasejusthelper.h"

test_phasejusthelper::test_phasejusthelper()
{

}

test_phasejusthelper::~test_phasejusthelper()
{

}

void test_phasejusthelper::test_case1()
{
    PhastJustHelpers helper;
    initTestValues();
    helper.calculateMinMaxDiffValues(&values);
    QCOMPARE( qFuzzyCompare(helper.getMinValue(),static_cast<float>(0.040)), true);
    QCOMPARE( qFuzzyCompare(helper.getMaxValue(),static_cast<float>(1.72)), true);
}

void test_phasejusthelper::test_case2()
{
    PhastJustHelpers helper;
    helper.calculateMinMaxDiffValues(&values2);
    QCOMPARE( qFuzzyCompare(helper.getMinValue(),static_cast<float>(38.1)), true);
    QCOMPARE( qFuzzyCompare(helper.getMaxValue(),static_cast<float>(38.9)), true);
}

void test_phasejusthelper::test_case3()
{
    PhastJustHelpers helper;
    helper.calculateMinMaxDiffValues(&values);
    helper.deleteFaultyPhasenJustageItem(helper.getMeanValues(),helper.getDiffValue(),&values);
    QCOMPARE(values.count(),19);
}

void test_phasejusthelper::test_case4()
{
    PhastJustHelpers helper;
    helper.calculateMinMaxDiffValues(&values2);
    helper.deleteFaultyPhasenJustageItem(helper.getMeanValues(),helper.getDiffValue(),&values2);
    QCOMPARE(values2.count(),19);

}

void test_phasejusthelper::test_case5()
{
    PhastJustHelpers helper;
    helper.calculateMinMaxDiffValues(&values3);
    helper.deleteFaultyPhasenJustageItem(helper.getMeanValues(),helper.getDiffValue(),&values3);
    QCOMPARE(values2.count(),19);
    //QString log;
    //log = helper.getLogString();
}

void test_phasejusthelper::test_case6()
{
    bool bVal;
    initTestValues();
    PhastJustHelpers helper;
    helper.calculateMinMaxDiffValues(&values3);
    bVal = helper.deleteFaultyPhasenJustageItem(helper.getMeanValues(),helper.getDiffValue(),&values3);
    QCOMPARE(bVal, true);
    helper.calculateMinMaxDiffValues(&values3);
    bVal = helper.deleteFaultyPhasenJustageItem(helper.getMeanValues(),helper.getDiffValue(),&values3);
    QCOMPARE(bVal, false);
}

void test_phasejusthelper::test_addLogString()
{
    PhastJustHelpers helper;
    helper.addToLog("TEST");
    helper.addToLog("TEST");
    QString test ="TEST\nTEST\n";
    QCOMPARE(helper.getLogString(),test);
}

void test_phasejusthelper::test_clearLogString()
{
    PhastJustHelpers helper;
    helper.addToLog("TEST");
    QString test ="TEST\n";
    QCOMPARE(helper.getLogString(),test);
    helper.clearLogStr();
    test.clear();
    QCOMPARE(helper.getLogString(),test);
}

void test_phasejusthelper::initTestValues()
{
    values.clear();
    values2.clear();
    values3.clear();

    values.append(0.041);
    values.append (0.042);
    values.append (0.040);
    values.append (1.72);
    values.append (0.041);
    values.append (0.042);
    values.append (0.040);
    values.append (0.041);
    values.append (0.042);
    values.append (0.040);
    values.append (0.041);
    values.append (0.042);
    values.append (0.040);
    values.append (0.041);
    values.append (0.042);
    values.append (0.040);
    values.append (0.040);
    values.append (0.041);
    values.append (0.042);
    values.append (0.040);

    values2.append(38.8);
    values2.append(38.7);
    values2.append(38.9);
    values2.append(38.1);
    values2.append(38.8);
    values2.append(38.7);
    values2.append(38.9);
    values2.append(38.8);
    values2.append(38.7);
    values2.append(38.9);
    values2.append(38.8);
    values2.append(38.7);
    values2.append(38.9);
    values2.append(38.8);
    values2.append(38.7);
    values2.append(38.9);
    values2.append(38.8);
    values2.append(38.7);
    values2.append(38.9);
    values2.append(38.8);

    values3.append(0.0187967);
    values3.append(0.0188513);
    values3.append(0.0188513);
    values3.append(0.0188513);
    values3.append(0.0187967);
    values3.append(0.0187967);
    values3.append(0.018906);
    values3.append(0.0188513);
    values3.append(0.0188513);
    values3.append(0.0188513);
    values3.append(0.0188513);
    values3.append(0.0187967);
    values3.append(0.0188513);
    values3.append(0.0187967);
    values3.append(0.018906);
    values3.append(0.0187967);
    values3.append(0.0187967);
    values3.append(0.0189879);
    values3.append(0.0188513);
    values3.append(1.70558);
}



QTEST_MAIN(test_phasejusthelper)
