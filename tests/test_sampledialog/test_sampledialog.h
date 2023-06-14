#ifndef TEST_SAMPLEDIALOG_H
#define TEST_SAMPLEDIALOG_H

#include <QtTest>
#include <QtGlobal>
#include <wmsampledialog.h>

// add necessary includes here

class test_sampledialog : public QObject
{
    Q_OBJECT

public:
    test_sampledialog();
    ~test_sampledialog();

private slots:
    void test_properties();
    void test_copyFloats();
    void test_minMax();
    void test_toHeight();
    void test_MinMaxFromData();
};

#endif // TEST_
