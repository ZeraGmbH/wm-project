#include "test_cmdlinepar.h"

#include "string.h"
/* argc
   argv
*/


char *test_cmdlinepar::helper(QString str)
{
    char* cstr;
    std::string fname = str.toStdString();
    cstr = new char [fname.size()+1];
    strlcpy( cstr, fname.c_str(),fname.size()+1 );
    return cstr;
}

void test_cmdlinepar::init(QString str)
{
    argc = 2;
    argv[0] = helper("wm3000");
    argv[1] = helper(str);
}

void test_cmdlinepar::test_numberone()
{
    init("-ip127.0.0.2");
    CommandLineParameter cmdLPar;
    cmdLPar.Parse(argc,argv);
    QString str, sollStr="ip";
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


QTEST_MAIN(test_cmdlinepar)
