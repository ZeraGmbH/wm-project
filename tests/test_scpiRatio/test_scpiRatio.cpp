#include "test_scpiRatio.h"


void test_scpiRatio::init()
{
    m_pParser = new cParse;
    m_pScpiHelper = new scpiHelper(m_pParser);
}

void test_scpiRatio::fromParameter(QString param, int typ)
{
    QStringList list;
    QString sprim, ssek;
    QString stest;
    bool ok;
    init();
    list = param.split(":");
    QByteArray barr = param.toLocal8Bit();
    m_s = barr.data();
    ok = m_pScpiHelper->GetTransformerRatio(&m_s,sprim, ssek, true, typ);
    if (!ok)
        stest = "hallo fuzzy";
    QCOMPARE(ok, true);
    QRegExp regexp("[AV]");
    if (list.at(0).contains(regexp))
    {   // einheiten waren in Param
        QCOMPARE(sprim,list.at(0));
    }
    if (list.at(1).contains(regexp))
    {   // einheiten waren in Param
        QCOMPARE(ssek,list.at(1));
    }

}

void test_scpiRatio::test_justRatio()
{
    fromParameter("100:10",currentChan);
    fromParameter("100:10",voltageChan);
    fromParameter("100:10",ECTChan);
}

void test_scpiRatio::test_withUnits()
{
    fromParameter("100A:10A",currentChan);
    fromParameter("100V:10V",voltageChan);
}


void test_scpiRatio::test_withDivider()
{
    fromParameter("100/3:10",currentChan);
    fromParameter("100:10/3",currentChan);
    fromParameter("100/w3:10",currentChan);
    fromParameter("100:10/w3",currentChan);

    fromParameter("100/3:10",voltageChan);
    fromParameter("100:10/3",voltageChan);
    fromParameter("100/w3:10",voltageChan);
    fromParameter("100:10/w3",voltageChan);

    fromParameter("100/3:10",ECTChan);
    fromParameter("100:10/3",ECTChan);
    fromParameter("100/w3:10",ECTChan);
    fromParameter("100:10/w3",ECTChan);

}

void test_scpiRatio::test_withPrefix()
{
    fromParameter("100k:10",currentChan);
    fromParameter("100:10k",currentChan);
    fromParameter("100m:10",currentChan);
    fromParameter("100:10m",currentChan);
    fromParameter("100u:10",currentChan);
    fromParameter("100:10u",currentChan);

    fromParameter("100k:10",voltageChan);
    fromParameter("100:10k",voltageChan);
    fromParameter("100m:10",voltageChan);
    fromParameter("100:10m",voltageChan);
    fromParameter("100M:10",voltageChan);
    fromParameter("100:10M",voltageChan);

    fromParameter("100k:10",ECTChan);
    fromParameter("100:10k",ECTChan);
    fromParameter("100m:10",ECTChan);
    fromParameter("100:10m",ECTChan);
    fromParameter("100:10u",ECTChan);

}

void test_scpiRatio::test_withPrefixVoltage()
{
    fromParameter("100mV:10V",voltageChan);
    fromParameter("100kV:10V",voltageChan);
    fromParameter("100V:10mV",voltageChan);
    fromParameter("100V:10kV",voltageChan);
}

void test_scpiRatio::test_withPrefixCurrent()
{
    fromParameter("100mA:10A",currentChan);
    fromParameter("100kA:10A",currentChan);
    fromParameter("100A:10mA",currentChan);
    fromParameter("100A:10kA",currentChan);
    fromParameter("1mA:10A",currentChan);
    fromParameter("1kA:10A",currentChan);

}

void test_scpiRatio::test_withDividerVoltage()
{
    fromParameter("100V/3:10V",voltageChan);
    fromParameter("100V:10V/3",voltageChan);
    fromParameter("100V/w3:10V",voltageChan);
    fromParameter("100V:10V/w3",voltageChan);
}

void test_scpiRatio::test_withDividerCurrent()
{
    fromParameter("100A/3:10A",currentChan);
    fromParameter("100A:10A/3",currentChan);
    fromParameter("100A/w3:10A",currentChan);
    fromParameter("100A:10A/w3",currentChan);
}

void test_scpiRatio::test_withNeww2Divider()
{
    fromParameter("100V/w2:10V",voltageChan);
    fromParameter("100kV/w2:10V",voltageChan);
    fromParameter("100mV/w2:10V",voltageChan);
    fromParameter("100:100V/w2",voltageChan);
    fromParameter("100:100kV/w2",voltageChan);
    fromParameter("100:100mV/w2",voltageChan);
}

QTEST_MAIN(test_scpiRatio)
