#ifndef TEST_GEOMETRYSERIALIZER_H
#define TEST_GEOMETRYSERIALIZER_H

#include <QTest>

class test_geometrychangehandler : public QObject
{
    Q_OBJECT
private slots:
    void timerFiresOnceOnSingleChange();
    void timerFiresOnceOnMultipleChangeInShortSuccession();
};

#endif // TEST_GEOMETRYSERIALIZER_H
