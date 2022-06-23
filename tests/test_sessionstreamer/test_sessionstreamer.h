#ifndef TEST_SESSIONSTREAMER_H
#define TEST_SESSIONSTREAMER_H

#include <QtTest>
#include <QString>

class test_sessionstreamer : public QObject
{
    Q_OBJECT
private slots:
    void cleanup();

    void noDefaultsOnWriteCannotOpenFile();
    void setDefaultsOnReadCannotOpenFile();

    void noDefaultsOnWriteCanOpenFile();
    void noDefaultsOnReadCanOpenFile();

    void returnFalseOnWriteCannotOpenFile();
    void returnTrueOnWriteCanOpenFile();

    void readStreamCalled();
    void writeStreamCalled();

    void writeCreatesEmptyFile();
    void writeCreatesNonEmptyFile();

    void setDefaultOnUnderRead();
    void setDefaultOnOverRead();
    void noDefaultOnMatchingRead();

    void readWriteContensOk();
};

#endif // TEST_SESSIONSTREAMER_H
