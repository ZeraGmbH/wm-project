#include "test_scpiRatio.h"


void test_scpiRatio::init()
{
    m_pParser = new cParse;
    m_pScpiHelper = new scpiHelper(m_pParser);
}

void test_scpiRatio::fromParameter(QString param)
{
    QStringList list;
    QString sprim, ssek;
    bool ok;
    init();
    list = param.split(":");
    QByteArray barr = param.toLocal8Bit();
    m_s = barr.data();
    ok = m_pScpiHelper->GetTransformerRatio(&m_s,sprim, ssek, true);
    QCOMPARE(ok, true);
    QCOMPARE(sprim,list.at(0));
    QCOMPARE(ssek,list.at(1));

}

void test_scpiRatio::test_justRatio()
{
    fromParameter("100:10");
}

void test_scpiRatio::test_withUnits()
{
    fromParameter("100A:10A");
    fromParameter("100V:10V");
}

/*
void test_scpiRatio::test_withDivider()
{
    fromParameter("100/3:10");
    fromParameter("100:10/3");
    fromParameter("100/w3:10");
    fromParameter("100:10/w3");
}

void test_scpiRatio::test_withPrefix()
{
    fromParameter("100k:10");
    fromParameter("100:10k");
    fromParameter("100m:10");
    fromParameter("100:10m");
    fromParameter("100M:10");
    fromParameter("100:10M");

}*/

void test_scpiRatio::test_withPrefixVoltage()
{
    fromParameter("100mV:10V");
    fromParameter("100kV:10V");
    fromParameter("100V:10mV");
    fromParameter("100V:10kV");
}

void test_scpiRatio::test_withPrefixCurrent()
{
    fromParameter("100mA:10A");
    fromParameter("100kA:10A");
    fromParameter("100A:10mA");
    fromParameter("100A:10kA");
    fromParameter("1mA:10A");
    fromParameter("1kA:10A");

}

void test_scpiRatio::test_withDividerVoltage()
{
    fromParameter("100V/3:10V");
    fromParameter("100V:10V/3");
    fromParameter("100V/w3:10V");
    fromParameter("100V:10V/w3");
}

void test_scpiRatio::test_withDividerCurrent()
{
    fromParameter("100A/3:10A");
    fromParameter("100A:10A/3");
    fromParameter("100A/w3:10A");
    fromParameter("100A:10A/w3");
}

void test_scpiRatio::test_withNeww2Divider()
{
    fromParameter("100V/w2:10V");
    fromParameter("100kV/w2:10V");
    fromParameter("100mV/w2:10V");
    fromParameter("100:100V/w2");
    fromParameter("100:100kV/w2");
    fromParameter("100:100mV/w2");

}

QTEST_MAIN(test_scpiRatio)
