#ifndef TEST_PHASEJUSTHELPER_H
#define TEST_PHASEJUSTHELPER_H

#include <QtTest>
#include <QtGlobal>
#include <phastjusthelpers.h>

// add necessary includes here

class test_phasejusthelper : public QObject
{
    Q_OBJECT

public:
    test_phasejusthelper();
    ~test_phasejusthelper();

private slots:
    void test_case1();
    void test_case2();
    void test_case3();
    void test_case4();
/*    void test_case5();
    void test_case6();*/

private:
    QList<float> values, values2;

};


#endif // TEST_
