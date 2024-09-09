#include "test_cmpactvalue.h"
#include "math.h"

const double PI_180 = 1.74532925e-2;

void test_cmpactvalue::test_phaseShift()
{
    cConfData data;
    data.m_fxPhaseShift = 0.0;
    data.m_fxTimeShift = 0.0;
    data.m_bDCmeasurement = false;

    cwmActValues values;
    values.Frequenz = 50.0;
    values.VekXSek = {1.0,0.0};
    values.VekNSek = {1.0,0.0};

    cmpActValues compute(&data,&values);
    compute.phikArteche();

    values.VekN = values.VekNSek;
    values.VekX = values.VekXSek;
    compute.fehlerBerechnung();
    // test no phaseshift
    QCOMPARE(qFuzzyCompare(values.AngleError,0.0),true);

    data.m_fxPhaseShift = 1.0;
    values.VekXSek = {1.0,0.0};
    values.VekNSek = {1.0,0.0};

    compute.phikArteche();

    values.VekN = values.VekNSek;
    values.VekX = values.VekXSek;
    compute.fehlerBerechnung();
    // test no positive phaseshift
    QCOMPARE(qFuzzyCompare(values.AngleError,-0.0174532925),true);

    data.m_fxPhaseShift = -1.0;
    values.VekXSek = {1.0,0.0};
    values.VekNSek = {1.0,0.0};

    compute.phikArteche();

    values.VekN = values.VekNSek;
    values.VekX = values.VekXSek;
    compute.fehlerBerechnung();
    // test no negative phaseshift, alldough there is non!
    QCOMPARE(qFuzzyCompare(values.AngleError,0.0174532925),true);

}

void test_cmpactvalue::test_timeShift()
{
    cConfData data;
    data.m_fxPhaseShift = 0.0;
    data.m_fxTimeShift = 0.0;
    data.m_bDCmeasurement = false;

    cwmActValues values;
    values.Frequenz = 50.0;
    values.VekXSek = {1.0,0.0};
    values.VekNSek = {1.0,0.0};

    cmpActValues compute(&data,&values);
    compute.phikArteche();

    values.VekN = values.VekNSek;
    values.VekX = values.VekXSek;
    compute.fehlerBerechnung();
    // test no timeshift
    QCOMPARE(qFuzzyCompare(values.AngleError,0.0),true);

    data.m_fxTimeShift = 1.0;
    values.VekXSek = {1.0,0.0};
    values.VekNSek = {1.0,0.0};

    compute.phikArteche();

    values.VekN = values.VekNSek;
    values.VekX = values.VekXSek;
    compute.fehlerBerechnung();
    // test no positive timeshift
    // double angle = values.AngleError;
    QCOMPARE(qFuzzyCompare(values.AngleError,0.314159265),true);

    data.m_fxTimeShift = -1.0;
    values.VekXSek = {1.0,0.0};
    values.VekNSek = {1.0,0.0};

    compute.phikArteche();

    values.VekN = values.VekNSek;
    values.VekX = values.VekXSek;
    compute.fehlerBerechnung();
    // test no negative timeshift, alldough there is non!
    QCOMPARE(qFuzzyCompare(values.AngleError,-0.314159265),true);
}

QTEST_MAIN(test_cmpactvalue)
