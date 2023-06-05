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
    test.setSingalProperties(str);
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
    test.setSingalProperties(str);
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
    float fValues [5] {0.05,-1000.0,2000.0,-0.56,78.4};
    float *val = &fValues[0];
    wmSampleDialog test;
    QString str ;
    str = ("test,5;");
    test.setSingalProperties(str);
    test.copyValuesToMemberList(val);
    minMax = test.getMinMax();
    QCOMPARE(minMax.first,float(-1000.0));
    QCOMPARE(minMax.second,float(2000.0));
}

void test_sampledialog::test_toHeight()
{
    QList<int> list;
    float fValues [5] {0.05,-1000.0,2000.0,-0.56,78.4};
    float *val = &fValues[0];
    wmSampleDialog test;
    QString str ;
    str = ("test,5;");
    test.setSingalProperties(str);
    test.copyValuesToMemberList(val);
    test.setMinMaxToMember();
    list = test.scaleFloatMemberValuesToHight();
/*  ohne Korrektur links oben 0 und Werte werden nach "unten" größer
 *    QCOMPARE(list.at(0), 0);
    QCOMPARE(list.at(1), -160);
    QCOMPARE(list.at(2), 320);
    QCOMPARE(list.at(3), 0);
    QCOMPARE(list.at(4), 12);*/

    QCOMPARE(list.at(0), 330); //320);
    QCOMPARE(list.at(1), 480);
    QCOMPARE(list.at(2), 30); //0);
    QCOMPARE(list.at(3), 330); //320);
    QCOMPARE(list.at(4), 319); //308);
}


QTEST_MAIN(test_sampledialog)
