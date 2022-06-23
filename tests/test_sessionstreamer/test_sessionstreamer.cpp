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
    virtual void readStream(QDataStream& stream) override {
        m_readStreamCallCount++;
        for(QString* str : m_stringsToRead) {
            stream >> *str;
        }
    }
    virtual void writeStream(QDataStream& stream) override {
        m_writeStreamCallCount++;
        for(QString str : m_stringsToWrite) {
            stream << str;
        }
    }
    virtual void setDefaults() override { m_defaultSetCount++; }


    int getDefaultSetCount() { return m_defaultSetCount; }
    int getReadStreamCallCount() { return m_readStreamCallCount; }
    int getWriteStreamCallCount() { return m_writeStreamCallCount; }
    void setStringsToWrite(QStringList stringsToWrite) { m_stringsToWrite = stringsToWrite; }
    void setStringsToRead(QList<QString*> stringsToRead) { m_stringsToRead = stringsToRead; }
private:
    int m_defaultSetCount = 0;
    int m_readStreamCallCount = 0;
    int m_writeStreamCallCount = 0;
    QStringList m_stringsToWrite;
    QList<QString*> m_stringsToRead;
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

void test_sessionstreamer::noDefaultsOnWriteCannotOpenFile()
{
    SessionStreamImplementorTest streamImp;
    SessionStreamer sessionStreamer(testMachineName, &streamImp, testInvalidHomePath);
    sessionStreamer.writeSession(testSessionBaseName);
    QCOMPARE(streamImp.getDefaultSetCount(), 0);
}

void test_sessionstreamer::setDefaultsOnReadCannotOpenFile()
{
    SessionStreamImplementorTest streamImp;
    SessionStreamer sessionStreamer(testMachineName, &streamImp, testInvalidHomePath);
    sessionStreamer.readSession(testSessionBaseName);
    QCOMPARE(streamImp.getDefaultSetCount(), 1);
}

void test_sessionstreamer::noDefaultsOnWriteCanOpenFile()
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

void test_sessionstreamer::noDefaultsOnReadCanOpenFile()
{
    QVERIFY(genEmptySessionFile());

    SessionStreamImplementorTest streamImp;
    SessionStreamer sessionStreamer(testMachineName, &streamImp, testHomePath);
    sessionStreamer.readSession(testSessionBaseName);
    QCOMPARE(streamImp.getDefaultSetCount(), 0);
}

void test_sessionstreamer::returnFalseOnWriteCannotOpenFile()
{
    SessionStreamImplementorTest streamImp;
    SessionStreamer sessionStreamer(testMachineName, &streamImp, testInvalidHomePath);
    QVERIFY(!sessionStreamer.writeSession(testSessionBaseName));
}

void test_sessionstreamer::returnTrueOnWriteCanOpenFile()
{
    SessionStreamImplementorTest streamImp;
    SessionStreamer sessionStreamer(testMachineName, &streamImp, testHomePath);
    QVERIFY(sessionStreamer.writeSession(testSessionBaseName));
}

void test_sessionstreamer::readStreamCalled()
{
    QVERIFY(genEmptySessionFile());

    SessionStreamImplementorTest streamImp;
    SessionStreamer sessionStreamer(testMachineName, &streamImp, testHomePath);
    sessionStreamer.readSession(testSessionBaseName);
    QCOMPARE(streamImp.getReadStreamCallCount(), 1);
}

void test_sessionstreamer::writeStreamCalled()
{
    SessionStreamImplementorTest streamImp;
    SessionStreamer sessionStreamer(testMachineName, &streamImp, testHomePath);
    sessionStreamer.writeSession(testSessionBaseName);
    QCOMPARE(streamImp.getWriteStreamCallCount(), 1);
}

void test_sessionstreamer::writeCreatesEmptyFile()
{
    SessionStreamImplementorTest streamImp;
    SessionStreamer sessionStreamer(testMachineName, &streamImp, testHomePath);
    sessionStreamer.writeSession(testSessionBaseName);

    SessionFileNameGen namGen(testMachineName, testHomePath);
    QFile file(namGen.getSessionFileName(testSessionBaseName));
    QVERIFY(file.exists());
    QCOMPARE(int(file.size()), 0);
}

void test_sessionstreamer::writeCreatesNonEmptyFile()
{
    SessionStreamImplementorTest streamImp;
    SessionStreamer sessionStreamer(testMachineName, &streamImp, testHomePath);
    QStringList strs = QStringList() << "foo";
    streamImp.setStringsToWrite(strs);
    sessionStreamer.writeSession(testSessionBaseName);

    SessionFileNameGen namGen(testMachineName, testHomePath);
    QFile file(namGen.getSessionFileName(testSessionBaseName));
    QVERIFY(file.exists());
    QVERIFY(file.size() >= 4);
}

void test_sessionstreamer::setDefaultOnUnderRead()
{
    SessionStreamImplementorTest streamImp;
    SessionStreamer sessionStreamer(testMachineName, &streamImp, testHomePath);
    QStringList strs = QStringList() << "foo" << "bar";
    streamImp.setStringsToWrite(strs);
    QString str1;
    QList<QString*> readList = QList<QString*>() << &str1;
    streamImp.setStringsToRead(readList);
    sessionStreamer.writeSession(testSessionBaseName);
    sessionStreamer.readSession(testSessionBaseName);
    QCOMPARE(streamImp.getDefaultSetCount(), 1);
}

void test_sessionstreamer::setDefaultOnOverRead()
{
    SessionStreamImplementorTest streamImp;
    SessionStreamer sessionStreamer(testMachineName, &streamImp, testHomePath);
    QStringList strs = QStringList() << "foo";
    streamImp.setStringsToWrite(strs);
    QString str1;
    QString str2;
    QList<QString*> readList = QList<QString*>() << &str1 << &str2;
    streamImp.setStringsToRead(readList);
    sessionStreamer.writeSession(testSessionBaseName);
    sessionStreamer.readSession(testSessionBaseName);
    QCOMPARE(streamImp.getDefaultSetCount(), 1);
}

void test_sessionstreamer::noDefaultOnMatchingRead()
{
    SessionStreamImplementorTest streamImp;
    SessionStreamer sessionStreamer(testMachineName, &streamImp, testHomePath);
    QStringList strs = QStringList() << "foo" << "bar";
    streamImp.setStringsToWrite(strs);
    QString str1;
    QString str2;
    QList<QString*> readList = QList<QString*>() << &str1 << &str2;
    streamImp.setStringsToRead(readList);
    sessionStreamer.writeSession(testSessionBaseName);
    sessionStreamer.readSession(testSessionBaseName);
    QCOMPARE(streamImp.getDefaultSetCount(), 0);
}

void test_sessionstreamer::readWriteContensOk()
{
    SessionStreamImplementorTest streamImp;
    SessionStreamer sessionStreamer(testMachineName, &streamImp, testHomePath);
    QStringList write = QStringList() << "foo" << "bar" << "baz";
    streamImp.setStringsToWrite(write);
    QStringList read = QStringList() << QString() << QString() << QString();
    QList<QString*> readList = QList<QString*>() << &read[0] << &read[1]  << &read[2];
    streamImp.setStringsToRead(readList);
    sessionStreamer.writeSession(testSessionBaseName);
    sessionStreamer.readSession(testSessionBaseName);
    QCOMPARE(streamImp.getDefaultSetCount(), 0);
    QVERIFY(read == write);
}

QTEST_MAIN(test_sessionstreamer)

