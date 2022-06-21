#include "test_sessionfilenamegen.h"
#include "sessionfilenamegen.h"
#include <QTest>
#include <QDir>

QTEST_MAIN(test_sessionfilenamegen)

static const QString machineName = "foo-machine";
static const QString customHome = "/tmp";
static const QString objName = "testObj";
static const QString dotSes = ".ses";
static const QString sessionName = "session.ses";

void test_sessionfilenamegen::defaultHomePath()
{
    SessionFileNameGen sessNameGen(machineName);
    QString expectedPath = QDir::homePath() + "/." + machineName;
    QString genPath = sessNameGen.getSessionPath();
    QCOMPARE(genPath, expectedPath);
}

void test_sessionfilenamegen::customHomePath()
{
    SessionFileNameGen sessNameGen(machineName, customHome);
    QString expectedPath = customHome + "/." + machineName;
    QString genPath = sessNameGen.getSessionPath();
    QCOMPARE(genPath, expectedPath);
}

void test_sessionfilenamegen::customHomePathTrailingSeparator()
{
    SessionFileNameGen sessNameGen(machineName, customHome + "/");
    QString expectedPath = customHome + "/." + machineName;
    QString genPath = sessNameGen.getSessionPath();
    QCOMPARE(genPath, expectedPath);
}

void test_sessionfilenamegen::nameForNoSessionName()
{
    SessionFileNameGen sessNameGen(machineName);
    QString expectedName = QDir::homePath() + "/." + machineName + "/" + objName + ".ses";
    QString genName = sessNameGen.getSessionFileName(objName);
    QCOMPARE(genName, expectedName);
}

void test_sessionfilenamegen::nameForSessionNameSes()
{
    SessionFileNameGen sessNameGen(machineName);
    QString expectedName = QDir::homePath() + "/." + machineName + "/" + objName + dotSes;
    QString genName = sessNameGen.getSessionFileName(objName, dotSes);
    QCOMPARE(genName, expectedName);
}

void test_sessionfilenamegen::nameForSessionFull()
{
    SessionFileNameGen sessNameGen(machineName);
    QString expectedName = QDir::homePath() + "/." + machineName + "/" + objName + sessionName;
    QString genName = sessNameGen.getSessionFileName(objName, sessionName);
    QCOMPARE(genName, expectedName);
}

