#ifndef RELEASEINFO_H
#define RELEASEINFO_H

#include <QProcess>

class cReleaseInfo: public QObject
{
    Q_OBJECT
public slots:
    virtual void show();
private:
    QProcess m_process;
};

#endif // RELEASEINFO_H
