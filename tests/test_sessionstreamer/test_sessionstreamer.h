#ifndef TEST_SESSIONSTREAMER_H
#define TEST_SESSIONSTREAMER_H

#include <QtTest>
#include <QString>

class test_sessionstreamer : public QObject
{
    Q_OBJECT
private slots:
    void cleanup();

    void setDefaultsOnWriteCannotOpenFile();
    void setDefaultsOnReadCannotOpenFile();

    void noDefaultsOnWriteFileValid();
    void noDefaultsOnReadFileExist();
};

#endif // TEST_SESSIONSTREAMER_H
