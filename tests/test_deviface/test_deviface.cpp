#include "test_deviface.h"

cNode *test_deviface::initTestCMdTree(cNode *cn)
{
    cNodeSCPI* Measure=new cNodeSCPI("MEASURE",isNode | isQuery,NULL,NULL,nixCmd,nixCmd);
    cNodeSCPI* ConfigurationMeasure=new cNodeSCPI("MEASURE",isNode,NULL,NULL, nixCmd,nixCmd);

    cNodeSCPI* ConfigurationComputationPHCorrectionTime=new cNodeSCPI("TIME"    ,isQuery | isCommand,             NULL,NULL,nixCmd,nixCmd);
    cNodeSCPI* ConfigurationComputationPHCorrectionPhase=new cNodeSCPI("PHASE"  ,isQuery | isCommand,             ConfigurationComputationPHCorrectionTime,NULL,nixCmd,nixCmd);
    cNodeSCPI* ConfigurationComputationPHCorrection=new cNodeSCPI("PHCORRECTION",isNode,                          NULL,ConfigurationComputationPHCorrectionPhase,nixCmd,nixCmd);
    cNodeSCPI* ConfigurationComputationOECorrectionFile=new cNodeSCPI("FILE"    ,isQuery | isCommand,             NULL,NULL,nixCmd,nixCmd);
    cNodeSCPI* ConfigurationComputationOECorrectionOn=new cNodeSCPI("ON"        ,isQuery | isCommand,             ConfigurationComputationOECorrectionFile,NULL,nixCmd,nixCmd);
    cNodeSCPI* ConfigurationComputationOECorrection=new cNodeSCPI("OECORRECTION",isNode,                          ConfigurationComputationPHCorrection,ConfigurationComputationOECorrectionOn,nixCmd,nixCmd);
    cNodeSCPI* ConfigurationComputationXOffset=new cNodeSCPI("XOFFSET"          ,isQuery | isCommand,             ConfigurationComputationOECorrection,NULL,nixCmd,nixCmd);
    cNodeSCPI* ConfigurationComputationNOffset=new cNodeSCPI("NOFFSET"          ,isQuery | isCommand,             ConfigurationComputationXOffset,NULL,nixCmd,nixCmd);

    cNodeSCPI* ConfigurationComputation=new cNodeSCPI("COMPUTATION"             ,isNode,                          ConfigurationMeasure,ConfigurationComputationNOffset,nixCmd,nixCmd);
    cNodeSCPI* ConfigurationOperationSignalCatalog=new cNodeSCPI("CATALOG"      ,isQuery,                         NULL,NULL,nixCmd,nixCmd);
    cNodeSCPI* ConfigurationOperationSignal=new cNodeSCPI("SIGNAL"              ,isNode | isQuery | isCommand,    NULL,ConfigurationOperationSignalCatalog,nixCmd,nixCmd);
    cNodeSCPI* ConfigurationOperationModeCatalog=new cNodeSCPI("CATALOG"        ,isQuery,                         NULL,NULL,nixCmd,nixCmd);
    cNodeSCPI* ConfigurationOperationMode=new cNodeSCPI("MODE"                  ,isNode | isQuery | isCommand,    ConfigurationOperationSignal,ConfigurationOperationModeCatalog,nixCmd,nixCmd);
    cNodeSCPI* ConfigurationOperation=  new cNodeSCPI("OPERATION"               ,isNode,                          ConfigurationComputation,ConfigurationOperationMode,nixCmd,nixCmd);
    cNodeSCPI* Configuration=           new cNodeSCPI("CONFIGURATION"           ,isNode | isCommand,              Measure,ConfigurationOperation,nixCmd,nixCmd);

    return (Configuration);
}

cNode *test_deviface::initTestCMdTreeFirst(cNode *cn)
{
    cNodeSCPI* deviceJustStatus = new cNodeSCPI("PROGRESS", isQuery ,NULL,NULL,nixCmd,nixCmd);
    cNodeSCPI* deviceJustOffset = new cNodeSCPI("OFFSET", isCommand ,deviceJustStatus,NULL,nixCmd,nixCmd);
    cNodeSCPI* deviceJustPhase = new cNodeSCPI("PHASE", isCommand ,deviceJustOffset,NULL,nixCmd,nixCmd);
    cNodeSCPI* deviceJust = new cNodeSCPI("JUST",isNode,NULL,deviceJustPhase,nixCmd,nixCmd);

    cNodeSCPI* deviceIFace = new cNodeSCPI("IFACE", isQuery,NULL,NULL,nixCmd,nixCmd);
    cNodeSCPI* device = new cNodeSCPI("DEVICE",isNode, deviceJust, deviceIFace, nixCmd, nixCmd);
    return (device);
}

cNode *test_deviface::initTestCmdTreeWithVarNode(cNode *cn)
{
    mMeasChannelList = {"N","X","ExT"};
    cNodeSCPI* SenseCNameRangeCatalog=new cNodeSCPI("CATALOG",isQuery,NULL,NULL,nixCmd,nixCmd);
    cNodeSCPI* SenseCNameRange=new cNodeSCPI("RANGE",isNode | isCommand | isQuery,NULL,SenseCNameRangeCatalog,nixCmd,nixCmd);
    cNodeSCPIVar* SenseCName=new cNodeSCPIVar(&mMeasChannelList,isNode,NULL,SenseCNameRange,nixCmd,nixCmd);
    cNodeSCPI* SenseChannelCatalog=new cNodeSCPI("CATALOG",isQuery,NULL,NULL,nixCmd,nixCmd);
    cNodeSCPI* SenseChannel=new cNodeSCPI("CHANNEL",isNode,SenseCName,SenseChannelCatalog,nixCmd,nixCmd);
    cNodeSCPI* Sense=new cNodeSCPI("SENSE",isNode,NULL,SenseChannel,nixCmd,nixCmd);
    return (Sense);
}

void test_deviface::test_numberone()
{
    cNode* commands;
    commands = initTestCMdTreeFirst(NULL);
    scpideviface deviface;
    QString str;
    str = deviface.GetInterface(commands);
    QStringList list;
    list = str.split("\n");
    QCOMPARE(list.count(),4);
    QCOMPARE(list.at(0),QString("DEVICE:IFACE?"));
    QCOMPARE(list.at(1),QString("JUST:PHASE!"));
    QCOMPARE(list.at(2),QString("JUST:OFFSET!"));
    QCOMPARE(list.at(3),QString("JUST:PROGRESS?"));


}

void test_deviface::test_numbertwo()
{
    cNode* commands;
    commands = initTestCMdTree(NULL);
    scpideviface deviface;
    QString str;
    str = deviface.GetInterface(commands);
    QStringList list;
    list = str.split("\n");
    QCOMPARE(list.count(),20);
    QCOMPARE(list.at(0),QString("CONFIGURATION!"));
    // ...
    QCOMPARE(list.at(19),QString("MEASURE?"));
}

void test_deviface::test_usesVarNodes()
{
    cNode* commands;
    commands = initTestCmdTreeWithVarNode(NULL);
    scpideviface deviface;
    QString str;
    str = deviface.GetInterface(commands);
    QStringList list;
    list = str.split("\n");
    QCOMPARE(list.count(),10);
    QCOMPARE(list.at(0),QString("SENSE:CHANNEL:CATALOG?"));
    // ...
    QCOMPARE(list.at(9),QString("SENSE:ExT:RANGE:CATALOG?"));
}

QTEST_MAIN(test_deviface)
