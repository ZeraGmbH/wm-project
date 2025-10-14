#ifndef WMMESSAGEBOX_H
#define WMMESSAGEBOX_H

#include "zhclientsocket.h"

#include <QObject>
#include <QMessageBox>

class wmMessageBox : public QObject
{
    Q_OBJECT
public:
    explicit wmMessageBox(QObject *parent = nullptr);
    void notJustMsgBox(int stat);
    int msgConnection(int error, QString host, int port);
    int msgAnswer(QString host, int port);
    int msgAnswerUnexpect(QString host, int port);
    int msgBusy(QString host, int port);
    void justDataWritten();
    void setSchnubbel();
    void Simulation();
    void msgRatioErr();
    void msgRatioEotErr();
    void noDocu(QString path);
signals:

};

#endif // WMMESSAGEBOX_H
