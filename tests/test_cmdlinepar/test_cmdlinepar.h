#ifndef TEST_CMDLINEPAR
#define TEST_CMDLINEPAR

#include <QTest>
#include "commandlineparameter.h"

class test_cmdlinepar : public QObject
{
	Q_OBJECT
private:
    void init(QString str);
    char * helper(QString str);
    QByteArray ba0,ba1;
    int argc;
    char *argv[2];
private slots:
    void test_numberone();
    void test_empty();
    void test_justage();
    void test_convent();
    void test_dc();
    void test_sample();
    void test_ppswdt();
};

#endif
