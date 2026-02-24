#include "test_cmdlinepar.h"


char *test_cmdlinepar::helper(QString str)
{
    char* cstr;
    QByteArray ba = str.toLatin1();
    cstr = ba.data();
    return cstr;
}

void test_cmdlinepar::init(QString str)
{
    argc = 2;
    ba0.clear();
    ba0.append("wm3000");
    ba1.clear();
    ba1.append(str.toLatin1());
    argv[0] = ba0.data();
    argv[1] = ba1.data();
}

void test_cmdlinepar::test_numberone()
{
    init("-ip127.0.0.2");
    CommandLineParameter cmdLPar;
    cmdLPar.Parse(argc,argv);
    QString str, sollStr="ip ";
    str = cmdLPar.GetOptionString();
    QCOMPARE(str,sollStr);
}

void test_cmdlinepar::test_empty()
{
    init("");
    CommandLineParameter cmdLPar;
    cmdLPar.Parse(argc,argv);
    QString str, sollStr="";
    str = cmdLPar.GetOptionString();
    QCOMPARE(str,sollStr);
}

void test_cmdlinepar::test_justage()
{
    init("-justage");
    CommandLineParameter cmdLPar;
    cmdLPar.Parse(argc,argv);
    QString str, sollStr="just ";
    str = cmdLPar.GetOptionString();
    QCOMPARE(str,sollStr);
}

void test_cmdlinepar::test_convent()
{
    init("-convent");
    CommandLineParameter cmdLPar;
    cmdLPar.Parse(argc,argv);
    QString str, sollStr="conv ";
    str = cmdLPar.GetOptionString();
    QCOMPARE(str,sollStr);
}

void test_cmdlinepar::test_dc()
{
    init("-dc");
    CommandLineParameter cmdLPar;
    cmdLPar.Parse(argc,argv);
    QString str, sollStr="dc ";
    str = cmdLPar.GetOptionString();
    QCOMPARE(str,sollStr);
}

void test_cmdlinepar::test_sample()
{
    init("-newsamplerates");
    CommandLineParameter cmdLPar;
    cmdLPar.Parse(argc,argv);
    QString str, sollStr="newsr ";
    str = cmdLPar.GetOptionString();
    QCOMPARE(str,sollStr);
}

void test_cmdlinepar::test_ppswdt()
{
    init("-ppswatchdog");
    CommandLineParameter cmdLPar;
    cmdLPar.Parse(argc,argv);
    QString str, sollStr="pwdt ";
    str = cmdLPar.GetOptionString();
    QCOMPARE(str,sollStr);
}

void test_cmdlinepar::test_screen()
{
    init("-screen");
    CommandLineParameter cmdLPar;
    cmdLPar.Parse(argc,argv);
    QString str, sollStr="";
    str = cmdLPar.GetOptionString();
    QCOMPARE(str,sollStr);
}


QTEST_MAIN(test_cmdlinepar)
