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
    void nameForSessionSet();
};

#endif // TEST_SESSIONFILENAME_H
