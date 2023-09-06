#include "test_phasejusthelper.h"

test_phasejusthelper::test_phasejusthelper()
{
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
}

test_phasejusthelper::~test_phasejusthelper()
{

}

void test_phasejusthelper::test_case1()
{
    PhastJustHelpers helper;
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



QTEST_MAIN(test_phasejusthelper)
