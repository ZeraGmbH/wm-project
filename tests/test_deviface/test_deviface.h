#ifndef TEST_DEVIFACE
#define TEST_DEVIFACE

#include <QTest>
#include "scpideviface.h"
#include "scpicommoncmdtype.h"

class test_deviface : public QObject
{
	Q_OBJECT
private:
    cNode* initTestCMdTree(cNode* cn);
    cNode* initTestCMdTreeFirst(cNode* cn);
private slots:
    void test_numberone();
    void test_numbertwo();
};

#endif
