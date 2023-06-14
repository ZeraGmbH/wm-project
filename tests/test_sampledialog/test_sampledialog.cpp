#include "test_sampledialog.h"

test_sampledialog::test_sampledialog()
{

}

test_sampledialog::~test_sampledialog()
{

}

void test_sampledialog::test_properties()
{
    wmSampleDialog test;
    QString str;
    str = "Messsignal0,640;";
    test.setSingalProperties(str,0);
    QCOMPARE(test.getSignalName(),QString("Messsignal0"));
    QCOMPARE(test.getSignalSamples(),640);
}

void test_sampledialog::test_copyFloats()
{
    QList<float> lTest;
    float fValues [5] {0.05,-1000.0,2000.0,-0.56,78.4};
    float *val = &fValues[0];
    wmSampleDialog test;
    QString str ;
    str = ("test,5;");
    test.setSingalProperties(str,0);
    lTest = test.copyValuesToList(val);
    QCOMPARE(lTest.at(0), fValues[0]);
    QCOMPARE(lTest.at(1), fValues[1]);
    QCOMPARE(lTest.at(2), fValues[2]);
    QCOMPARE(lTest.at(3), fValues[3]);
    QCOMPARE(lTest.at(4), fValues[4]);
}

void test_sampledialog::test_minMax()
{
    QPair<float,float> minMax;
    wmSampleData data;
    float fValues [5] {0.05,-1000.0,2000.0,-0.56,78.4};
    float *val = &fValues[0];
    wmSampleDialog test;
    QString str ;
    str = ("test,5;");
    test.setSingalProperties(str,0);
    test.copyValuesToMemberList(val, data);
    minMax = test.getMinMax(data);
    QCOMPARE(minMax.first,float(-1000.0));
    QCOMPARE(minMax.second,float(2000.0));
}

void test_sampledialog::test_toHeight()
{
    QList<int> list;
    wmSampleData data;
    float fValues [5] {0.05,-1000.0,2000.0,-0.56,78.4};
    float *val = &fValues[0];
    wmSampleDialog test;
    QString str ;
    str = ("test,5;");
    test.setSingalProperties(str,0);
    test.copyValuesToMemberList(val,data);
    test.setMinMaxToMember(data);
    test.getMinMaxFromValuesToMember(test.getMinMaxFromValues(data,data));
    list = test.scaleFloatMemberValuesToHight(data);
/*  ohne Korrektur links oben 0 und Werte werden nach "unten" größer
 *    QCOMPARE(list.at(0), 0);
    QCOMPARE(list.at(1), -160);
    QCOMPARE(list.at(2), 320);
    QCOMPARE(list.at(3), 0);
    QCOMPARE(list.at(4), 12);*/

    QCOMPARE(list.at(0), 254); //330); //320);
    QCOMPARE(list.at(1), 366); //480);
    QCOMPARE(list.at(2), 30); //0);
    QCOMPARE(list.at(3), 254); //330); //320);
    QCOMPARE(list.at(4), 246); //319); //308);
}

void test_sampledialog::test_MinMaxFromData()
{
    wmSampleDialog test;
    wmSampleData data0, data1;
    QPair<float,float> minMax;
    data0.mMinMax.first = -5.0;
    data1.mMinMax.first = -7.0;
    data0.mMinMax.second = 5.0;
    data1.mMinMax.second = 8.0;
    minMax = test.getMinMaxFromValues(data0,data1);
    QCOMPARE(minMax.first,float(-8.0));
    QCOMPARE(minMax.second,float(8.0));

}


QTEST_MAIN(test_sampledialog)
