#ifndef TEST_SCPIRATIO
#define TEST_SCPIRATIO

#include <QTest>
#include "scpihelper.h"

class test_scpiRatio : public QObject
{
	Q_OBJECT
private:
    cParse* m_pParser;
    scpiHelper* m_pScpiHelper;
    char* m_s; // das Kommando?

    void init();
    void fromParameter(QString param);
private slots:
    void test_justRatio();
    void test_withUnits();
//    void test_withDivider();
//    void test_withPrefix();
    void test_withPrefixVoltage();
    void test_withPrefixCurrent();
    void test_withDividerVoltage();
    void test_withDividerCurrent();

};

#endif
