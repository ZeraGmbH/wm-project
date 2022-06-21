#ifndef TEST_SESSIONFILENAME_H
#define TEST_SESSIONFILENAME_H

#include <QtTest>
#include <QString>

class test_sessionfilenamegen : public QObject
{
    Q_OBJECT
private slots:
    void defaultHomePath();
    void customHomePath();
    void customHomePathTrailingSeparator();
    void nameForNoSessionName();
    // remain compatible for now...
    void nameForSessionNameSes();
    void nameForSessionFull();
    void cmpOldNameSessionNameSes();
    void cmpOldNameSessionNameFull();
};

#endif // TEST_SESSIONFILENAME_H
