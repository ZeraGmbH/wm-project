#include "test_sessionstreamer.h"
#include "sessionstreamer.h"
#include <QTest>

// Invalid path /dev/null based upon
// https://stackoverflow.com/questions/1311037/are-there-any-invalid-linux-filenames
static const QString testInvalidHomePath = "/dev/null";
static const QString testHomePath = "/tmp/test_sessionstreamer";
static const QString testMachineName = "test-machine";
static const QString testSessionBaseName = "test-base";

class SessionStreamImplementorTest : public ISessionStreamImplementor
{
public:
    virtual void readStream(QDataStream& stream) override { Q_UNUSED(stream) }
    virtual void writeStream(QDataStream& stream) override { Q_UNUSED(stream) }
    virtual void setDefaults() override { m_defaultSetCount++; }
    int getDefaultSetCount() { return m_defaultSetCount; }
private:
    int m_defaultSetCount = 0;
};

// based upon https://stackoverflow.com/questions/11050977/removing-a-non-empty-folder-in-qt
static void removeDir(QString dirName)
{
    QDir dir(dirName);
    if (dir.exists()) {
        for(QFileInfo info : dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                removeDir(info.absoluteFilePath());
            }
            else {
                QFile::remove(info.absoluteFilePath());
            }
        }
        QDir().rmdir(dirName);
    }
}

void test_sessionstreamer::cleanup()
{
    removeDir(testHomePath);
}

void test_sessionstreamer::setDefaultsOnWriteCannotOpenFile()
{
    SessionStreamImplementorTest streamImp;
    SessionStreamer sessionStreamer(testMachineName, &streamImp, testInvalidHomePath);
    sessionStreamer.writeSession(testSessionBaseName);
    QCOMPARE(streamImp.getDefaultSetCount(), 1);
}

void test_sessionstreamer::setDefaultsOnReadCannotOpenFile()
{
    SessionStreamImplementorTest streamImp;
    SessionStreamer sessionStreamer(testMachineName, &streamImp, testInvalidHomePath);
    sessionStreamer.readSession(testSessionBaseName);
    QCOMPARE(streamImp.getDefaultSetCount(), 1);
}

void test_sessionstreamer::noDefaultsOnWriteFileValid()
{
    SessionStreamImplementorTest streamImp;
    SessionStreamer sessionStreamer(testMachineName, &streamImp, testHomePath);
    sessionStreamer.writeSession(testSessionBaseName);
    QCOMPARE(streamImp.getDefaultSetCount(), 0);
}

static bool genEmptySessionFile()
{
    SessionFileNameGen namGen(testMachineName, testHomePath);
    QDir().mkpath(namGen.getSessionPath());
    QFile file(namGen.getSessionFileName(testSessionBaseName));
    return file.open(QFile::WriteOnly);
}

void test_sessionstreamer::noDefaultsOnReadFileExist()
{
    QVERIFY(genEmptySessionFile());

    SessionStreamImplementorTest streamImp;
    SessionStreamer sessionStreamer(testMachineName, &streamImp, testHomePath);
    sessionStreamer.readSession(testSessionBaseName);
    QCOMPARE(streamImp.getDefaultSetCount(), 0);
}

QTEST_MAIN(test_sessionstreamer)

