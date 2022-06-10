#include "main-unittest.h"

static QList<QObject*> listTests;

QObject *addTest(QObject *test)
{
    listTests.append(test);
    return test;
}

int main(int argc, char *argv[])
{
    int status = 0;
    //Q_INIT_RESOURCE(resource);

    // Make qCritical in tests
    qputenv("QT_FATAL_CRITICALS", "1");

    while(!listTests.isEmpty()) {
        QCoreApplication app(argc, argv);
        QObject *test = listTests.takeLast();
        status |= QTest::qExec(test, argc, argv);
        delete test;
    }
    if(status) {
#ifdef QT_DEBUG
        qWarning("\n\033[1;31m%s\033[0m","One or more tests failed!!!");
#else
        qWarning("\nOne or more tests failed!!!");
#endif
    }
    else {
#ifdef QT_DEBUG
        qDebug("\n\033[1;32m%s\033[0m","All tests passed :)");
#else
        qDebug("\nAll tests passed :)");
#endif
    }
    return status;
}
