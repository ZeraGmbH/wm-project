#ifndef CCMDTIMER_H
#define CCMDTIMER_H

#include <QTimer>

class cCmdTimer: public QTimer
{
    Q_OBJECT

public:
     cCmdTimer(QObject * parent = 0, const char * name = 0 );
     int start(int, QString&);

signals:
     void Command2Execute(QString&);

private slots:
    void TimeExpired();

private:
    QString m_sCmd;
};

#endif // CCMDTIMER_H
