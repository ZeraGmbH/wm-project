#include "test_phasejusthelper.h"

test_phasejusthelper::test_phasejusthelper()
{

}

test_phasejusthelper::~test_phasejusthelper()
{

}

void test_phasejusthelper::test_case1()
{
    PhastJustHelpers helper;
    initTestValues();
    helper.calculateMinMaxDiffValues(&values);
    QCOMPARE( qFuzzyCompare(helper.getMinValue(),static_cast<float>(0.040)), true);
    QCOMPARE( qFuzzyCompare(helper.getMaxValue(),static_cast<float>(1.72)), true);
}

void test_phasejusthelper::test_case2()
{
    PhastJustHelpers helper;
    helper.calculateMinMaxDiffValues(&values2);
    QCOMPARE( qFuzzyCompare(helper.getMinValue(),static_cast<float>(38.1)), true);
    QCOMPARE( qFuzzyCompare(helper.getMaxValue(),static_cast<float>(38.9)), true);
}

void test_phasejusthelper::test_case3()
{
    PhastJustHelpers helper;
    helper.calculateMinMaxDiffValues(&values);
    helper.deleteFaultyPhasenJustageItem(helper.getMeanValues(),helper.getDiffValue(),&values);
    QCOMPARE(values.count(),19);
}

void test_phasejusthelper::test_case4()
{
    PhastJustHelpers helper;
    helper.calculateMinMaxDiffValues(&values2);
    helper.deleteFaultyPhasenJustageItem(helper.getMeanValues(),helper.getDiffValue(),&values2);
    QCOMPARE(values2.count(),19);

}

void test_phasejusthelper::test_case5()
{
    PhastJustHelpers helper;
    helper.calculateMinMaxDiffValues(&values3);
    helper.deleteFaultyPhasenJustageItem(helper.getMeanValues(),helper.getDiffValue(),&values3);
    QCOMPARE(values2.count(),19);
    //QString log;
    //log = helper.getLogString();
}

void test_phasejusthelper::test_case6()
{
    bool bVal;
    initTestValues();
    PhastJustHelpers helper;
    helper.calculateMinMaxDiffValues(&values3);
    bVal = helper.deleteFaultyPhasenJustageItem(helper.getMeanValues(),helper.getDiffValue(),&values3);
    QCOMPARE(bVal, true);
    helper.calculateMinMaxDiffValues(&values3);
    bVal = helper.deleteFaultyPhasenJustageItem(helper.getMeanValues(),helper.getDiffValue(),&values3);
    QCOMPARE(bVal, false);
}

void test_phasejusthelper::test_negPhaseValues()
{
    initTestValues();
    PhastJustHelpers helper;
    helper.calculateMinMaxDiffValues(&negValues);
    //float diff = helper.getDiffValue();
    QCOMPARE( qFuzzyCompare(helper.getDiffValue(),static_cast<float>(0.0018988)),true);

}

void test_phasejusthelper::test_addLogString()
{
    PhastJustHelpers helper;
    helper.addToLog("TEST");
    helper.addToLog("TEST");
    QString test ="TEST\nTEST\n";
    QCOMPARE(helper.getLogString(),test);
}

void test_phasejusthelper::test_clearLogString()
{
    PhastJustHelpers helper;
    helper.addToLog("TEST");
    QString test ="TEST\n";
    QCOMPARE(helper.getLogString(),test);
    helper.clearLogStr();
    test.clear();
    QCOMPARE(helper.getLogString(),test);
}

void test_phasejusthelper::test_BellyDetector()
{
    PhastJustHelpers helper;
    initBellyTestValues(1); // 3.3 Min
    helper.calculateMinMaxDiffValues(&bellyVal);
    QCOMPARE(helper.hasBelly(),true);

    initBellyTestValues(2); // 1.2 Min
    helper.calculateMinMaxDiffValues(&bellyVal);
    helper.clearLogStr();
    QCOMPARE(helper.hasBelly(),true);
    bool test = helper.getLogString().contains("Belly");
    QCOMPARE(test,true);

    initBellyTestValues(3); // 0.1 Min
    helper.calculateMinMaxDiffValues(&bellyVal);
    helper.clearLogStr();
    QCOMPARE(helper.hasBelly(),false);
    QCOMPARE(helper.getLogString().length(),0);
}

void test_phasejusthelper::initTestValues()
{
    values.clear();
    values2.clear();
    values3.clear();
    negValues.clear();

    values.append(0.041);
    values.append (0.042);
    values.append (0.040);
    values.append (1.72);
    values.append (0.041);
    values.append (0.042);
    values.append (0.040);
    values.append (0.041);
    values.append (0.042);
    values.append (0.040);
    values.append (0.041);
    values.append (0.042);
    values.append (0.040);
    values.append (0.041);
    values.append (0.042);
    values.append (0.040);
    values.append (0.040);
    values.append (0.041);
    values.append (0.042);
    values.append (0.040);

    values2.append(38.8);
    values2.append(38.7);
    values2.append(38.9);
    values2.append(38.1);
    values2.append(38.8);
    values2.append(38.7);
    values2.append(38.9);
    values2.append(38.8);
    values2.append(38.7);
    values2.append(38.9);
    values2.append(38.8);
    values2.append(38.7);
    values2.append(38.9);
    values2.append(38.8);
    values2.append(38.7);
    values2.append(38.9);
    values2.append(38.8);
    values2.append(38.7);
    values2.append(38.9);
    values2.append(38.8);

    values3.append(0.0187967);
    values3.append(0.0188513);
    values3.append(0.0188513);
    values3.append(0.0188513);
    values3.append(0.0187967);
    values3.append(0.0187967);
    values3.append(0.018906);
    values3.append(0.0188513);
    values3.append(0.0188513);
    values3.append(0.0188513);
    values3.append(0.0188513);
    values3.append(0.0187967);
    values3.append(0.0188513);
    values3.append(0.0187967);
    values3.append(0.018906);
    values3.append(0.0187967);
    values3.append(0.0187967);
    values3.append(0.0189879);
    values3.append(0.0188513);
    values3.append(1.70558);

    negValues.append(-0.0171966);
    negValues.append(-0.0165272);
    negValues.append(-0.0159945);
    negValues.append(-0.0166365);
    negValues.append(-0.0158852);
    negValues.append(-0.015489);
    negValues.append(-0.0166775);
    negValues.append(-0.0172102);
    negValues.append(-0.0166365);
    negValues.append(-0.0168687);
    negValues.append(-0.0172649);
    negValues.append(-0.0166228);
    negValues.append(-0.0164999);
    negValues.append(-0.0160628);
    negValues.append(-0.0165136);
    negValues.append(-0.016213);
    negValues.append(-0.0164043);
    negValues.append(-0.0173878);
    negValues.append(-0.0163496);
    negValues.append(-0.0169234);
}

void test_phasejusthelper::initBellyTestValues(int i)
{
    bellyVal.clear();
    switch(i){
    case 1:
    bellyVal.append( 0.193376);
    bellyVal.append( 0.187871);
    bellyVal.append( 0.182134);
    bellyVal.append( 0.175891);
    bellyVal.append( 0.169594);
    bellyVal.append( 0.163009);
    bellyVal.append( 0.156302);
    bellyVal.append( 0.150633);
    bellyVal.append( 0.147027);
    bellyVal.append( 0.144103);
    bellyVal.append( 0.141986);
    bellyVal.append( 0.140319);
    bellyVal.append( 0.138707);
    bellyVal.append( 0.138544);
    bellyVal.append( 0.138544);
    bellyVal.append( 0.138024);
    bellyVal.append( 0.138257);
    bellyVal.append( 0.13883);
    bellyVal.append( 0.140593);
    bellyVal.append( 0.141931);
    //Diff: 3.32111 Minuten
    break;
    case 2:
    bellyVal.append( 0.21944);
    bellyVal.append( 0.220998);
    bellyVal.append( 0.215656);
    bellyVal.append( 0.206613);
    bellyVal.append( 0.204195);
    bellyVal.append( 0.21302);
    bellyVal.append( 0.220984);
    bellyVal.append( 0.220479);
    bellyVal.append( 0.212856);
    bellyVal.append( 0.201682);
    bellyVal.append( 0.207296);
    bellyVal.append( 0.217036);
    bellyVal.append( 0.221612);
    bellyVal.append( 0.207925);
    bellyVal.append( 0.201559);
    bellyVal.append( 0.203403);
    bellyVal.append( 0.211299);
    bellyVal.append( 0.220301);
    bellyVal.append( 0.214277);
    bellyVal.append( 0.204947);
    //Diff: 1.20321 Minuten
    break;
        case 3:
    bellyVal.append( 0.0548328);
    bellyVal.append( 0.0563217);
    bellyVal.append( 0.0555841);
    bellyVal.append( 0.0564447);
    bellyVal.append( 0.0566632);
    bellyVal.append( 0.0556251);
    bellyVal.append( 0.0551196);
    bellyVal.append( 0.0560349);
    bellyVal.append( 0.0554065);
    bellyVal.append( 0.0550513);
    bellyVal.append( 0.0558026);
    bellyVal.append( 0.0561578);
    bellyVal.append( 0.0562125);
    bellyVal.append( 0.0558026);
    bellyVal.append( 0.0548328);
    bellyVal.append( 0.0556934);
    bellyVal.append( 0.0553519);
    bellyVal.append( 0.0554611);
    bellyVal.append( 0.0555158);
    bellyVal.append( 0.0558026);
    //Diff: 0.10983 Minuten
    break;
        case 4:
    bellyVal.append( 0.103996);
    bellyVal.append( 0.103423);
    bellyVal.append( 0.102562);
    bellyVal.append( 0.104106);
    bellyVal.append( 0.103696);
    bellyVal.append( 0.103477);
    bellyVal.append( 0.103477);
    bellyVal.append( 0.10371);
    bellyVal.append( 0.102507);
    bellyVal.append( 0.10319);
    bellyVal.append( 0.103942);
    bellyVal.append( 0.103942);
    bellyVal.append( 0.10371);
    bellyVal.append( 0.10371);
    bellyVal.append( 0.10319);
    bellyVal.append( 0.104679);
    bellyVal.append( 0.10319);
    bellyVal.append( 0.10319);
    bellyVal.append( 0.103996);
    bellyVal.append( 0.103477);
    //Diff: 0.13032 Minuten
    break;
        case 5:
    bellyVal.append( 0.211367);
    bellyVal.append( 0.209359);
    bellyVal.append( 0.205179);
    bellyVal.append( 0.203348);
    bellyVal.append( 0.200015);
    bellyVal.append( 0.197092);
    bellyVal.append( 0.192229);
    bellyVal.append( 0.185467);
    bellyVal.append( 0.178814);
    bellyVal.append( 0.173036);
    bellyVal.append( 0.165181);
    bellyVal.append( 0.156411);
    bellyVal.append( 0.147081);
    bellyVal.append( 0.138134);
    bellyVal.append( 0.129951);
    bellyVal.append( 0.122206);
    bellyVal.append( 0.114884);
    bellyVal.append( 0.109146);
    bellyVal.append( 0.103136);
    bellyVal.append( 0.101128);
    //Diff: 6.61435 Minuten
    break;
        case 6:
    bellyVal.append( 0.225164);
    bellyVal.append( 0.231188);
    bellyVal.append( 0.221681);
    bellyVal.append( 0.203512);
    bellyVal.append( 0.191887);
    bellyVal.append( 0.212228);
    bellyVal.append( 0.228087);
    bellyVal.append( 0.22918);
    bellyVal.append( 0.216066);
    bellyVal.append( 0.190221);
    bellyVal.append( 0.199278);
    bellyVal.append( 0.217897);
    bellyVal.append( 0.231188);
    bellyVal.append( 0.228661);
    bellyVal.append( 0.195658);
    bellyVal.append( 0.191997);
    bellyVal.append( 0.202378);
    bellyVal.append( 0.220984);
    bellyVal.append( 0.226079);
    bellyVal.append( 0.208895);
    //Diff: 2.45805 Minuten
    break;
        case 7:
    bellyVal.append( 0.057114);
    bellyVal.append( 0.0560349);
    bellyVal.append( 0.0548874);
    bellyVal.append( 0.0564857);
    bellyVal.append( 0.0541361);
    bellyVal.append( 0.0562671);
    bellyVal.append( 0.0563764);
    bellyVal.append( 0.0559256);
    bellyVal.append( 0.0567725);
    bellyVal.append( 0.0568408);
    bellyVal.append( 0.0552972);
    bellyVal.append( 0.0566632);
    bellyVal.append( 0.0543683);
    bellyVal.append( 0.0567179);
    bellyVal.append( 0.0553519);
    bellyVal.append( 0.0564447);
    bellyVal.append( 0.0559119);
    bellyVal.append( 0.0559119);
    bellyVal.append( 0.0544366);
    bellyVal.append( 0.0544366);
    //Diff: 0.178678 Minuten
    break;
        case 8:
    bellyVal.append( 0.104447);
    bellyVal.append( 0.105704);
    bellyVal.append( 0.102617);
    bellyVal.append( 0.102904);
    bellyVal.append( 0.104789);
    bellyVal.append( 0.1036);
    bellyVal.append( 0.10457);
    bellyVal.append( 0.104051);
    bellyVal.append( 0.103423);
    bellyVal.append( 0.105253);
    bellyVal.append( 0.104734);
    bellyVal.append( 0.104338);
    bellyVal.append( 0.104215);
    bellyVal.append( 0.105021);
    bellyVal.append( 0.103587);
    bellyVal.append( 0.103819);
    bellyVal.append( 0.104393);
    bellyVal.append( 0.102221);
    bellyVal.append( 0.105076);
    bellyVal.append( 0.102671);
    //Diff: 0.209004 Minuten
    break;
        case 9:
    bellyVal.append( 0.140251);
    bellyVal.append( 0.144732);
    bellyVal.append( 0.153952);
    bellyVal.append( 0.158993);
    bellyVal.append( 0.167367);
    bellyVal.append( 0.17779);
    bellyVal.append( 0.186328);
    bellyVal.append( 0.19727);
    bellyVal.append( 0.209987);
    bellyVal.append( 0.223511);
    bellyVal.append( 0.237431);
    bellyVal.append( 0.249302);
    bellyVal.append( 0.262867);
    bellyVal.append( 0.278276);
    bellyVal.append( 0.288029);
    bellyVal.append( 0.297373);
    bellyVal.append( 0.308315);
    bellyVal.append( 0.315705);
    bellyVal.append( 0.323505);
    bellyVal.append( 0.325786);
    //Diff: 11.1321 Minuten
    break;
        case 10:
    bellyVal.append( 0.24448);
    bellyVal.append( 0.25314);
    bellyVal.append( 0.226148);
    bellyVal.append( 0.189825);
    bellyVal.append( 0.191027);
    bellyVal.append( 0.228784);
    bellyVal.append( 0.253537);
    bellyVal.append( 0.238756);
    bellyVal.append( 0.20466);
    bellyVal.append( 0.179115);
    bellyVal.append( 0.209482);
    bellyVal.append( 0.242239);
    bellyVal.append( 0.252799);
    bellyVal.append( 0.195152);
    bellyVal.append( 0.174511);
    bellyVal.append( 0.183746);
    bellyVal.append( 0.217159);
    bellyVal.append( 0.247239);
    bellyVal.append( 0.222364);
    bellyVal.append( 0.186792);
    //Diff: 4.74152 Minuten
    break;
    }


}



QTEST_MAIN(test_phasejusthelper)
