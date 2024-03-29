#include <QtTest>

#include "dspmeasdata.h"

// add necessary includes here

class test_despmemory : public QObject
{
    Q_OBJECT

private slots:
    void test_measVarListSingle();
    void test_measVarListMultiple();
    void test_getFloatPointer();
    void test_getDataLenght();
    void test_getDataLenght1024();
    void test_getMeasDataName();
    void test_varlist();
};

void test_despmemory::test_measVarListSingle()
{
    cDspMeasData<float> test("test");
    test.addVarItem(cDspVar("MAXN",1,vApplication | vDspIntern));
    QString strActual, strExpected;
    strActual = test.MeasVarList();
    strExpected = "MAXN;";
    QCOMPARE(strExpected,strActual);
}

void test_despmemory::test_measVarListMultiple()
{
    cDspMeasData<float> test("test");
    test.addVarItem(cDspVar("MAXN",1,vApplication | vDspIntern));
    test.addVarItem(cDspVar("Hallo",1,vApplication | vDspIntern));
    test.addVarItem(cDspVar("Fuzzy",1,vApplication | vDspIntern));
    QString strActual, strExpected;
    strActual = test.MeasVarList();
    strExpected = "MAXN;Hallo;Fuzzy;";
    QCOMPARE(strExpected,strActual);
}

void test_despmemory::test_getFloatPointer()
{
    cDspMeasData<float> test("test");
    test.addVarItem(cDspVar("MAXN",1,vApplication | vDspIntern));
    float* fpoi = nullptr;
    fpoi = test.data();
    if(fpoi == nullptr) QCOMPARE(1,2);
}

void test_despmemory::test_getDataLenght()
{
    cDspMeasData<float> test("test");
    test.addVarItem(cDspVar("MAXN",1,vApplication | vDspIntern));
    test.addVarItem(cDspVar("MAXX",1,vApplication | vDspIntern));
    test.addVarItem(cDspVar("MAXG",1,vApplication | vDspIntern));
    uint i = test.getDataLenght();
    QCOMPARE(i,(uint)3);
    test.addVarItem(cDspVar("MAXI",1,vApplication | vDspIntern));
    i = test.getDataLenght();
    QCOMPARE(i,(uint)4);

}

void test_despmemory::test_getDataLenght1024()
{
    cDspMeasData<float> test("test");
    test.addVarItem(cDspVar("MAXG",1024,vApplication | vDspIntern));
    uint i = test.getDataLenght();
    QCOMPARE(i,(uint)1024);
}

void test_despmemory::test_getMeasDataName()
{
    cDspMeasData<float> test("test");
    QString strActual, strExpected;
    strExpected = "test";
    strActual = test.name();
    QCOMPARE(strActual,strExpected);
}

void test_despmemory::test_varlist()
{
    cDspMeasData<float> test("test");
    test.addVarItem(cDspVar("MAXN",1,vApplication | vDspIntern));
    test.addVarItem(cDspVar("MAXG",1024,vApplication | vDspIntern));
    QString str = test.VarList();
    QString strExpected = "MAXN,1;MAXG,1024;";
    QCOMPARE(str,strExpected);
    uint i = test.getDataLenght();
    QCOMPARE(i,(uint)1025);
}

QTEST_APPLESS_MAIN(test_despmemory)

#include "test_despmemory.moc"
