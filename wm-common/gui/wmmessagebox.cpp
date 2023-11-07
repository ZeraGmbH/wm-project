#include "wmmessagebox.h"

wmMessageBox::wmMessageBox(QObject *parent)
    : QObject{parent}
{

}

void wmMessageBox::notJustMsgBox(int stat)
{
    QString s;
    s = tr("Attention !");
    s+="\n";
    if (stat & 7)
        s += trUtf8("Device not justified !");
    if (stat & 2)
        s += trUtf8("\nVersions number not identical !");
    if (stat & 4)
        s += trUtf8("\nSerial number not identical !");

    QMessageBox::critical( 0, "Justage", s);
}

int wmMessageBox::msgConnection(int error, QString host, int port)
{
    int userRM;
    QString m = tr("Keine Verbindung zu %1:%2\n") .arg(host).arg(port);
    if (error & ! myErrHostNotFound)
        m+=tr("Host nicht gefunden.\n");
    else
        m+=tr("Host gefunden. Keine Verbindung zu Server.\n");

    m+=tr("Das Programm kann ohne Server nur\n"
            "im Simulations Modus betrieben werden.\n");

    userRM = QMessageBox::warning( 0, tr("TCP-Verbindung"),m,
                                  tr("Programm&Abbruch"),
                                  tr("&Wiederholen"),
                                  tr("&Simulation"),
                                  1,-1 );
    return userRM;
}

int wmMessageBox::msgAnswer(QString host, int port)
{
    int userRM;
    userRM = QMessageBox::warning( 0, tr("TCP-Verbindung"),
                                  tr("Fehler beim Schreiben von Daten\n"
                                     "für %1:%2 .\n"
                                     "Details stehen in LogFile.").arg(host) .arg(port),
                                  tr("Programm&Abbruch"),
                                  tr("&Wiederholen"),
                                  tr("&Simulation"),
                                  1,-1 );
    return userRM;
}

int wmMessageBox::msgAnswerUnexpect(QString host, int port)
{
    int userRM;
    userRM =  QMessageBox::warning( 0, tr("TCP-Verbindung"),
                                  tr("Unerwartete Antwort beim Lesen\n"
                                     "von %1:%2 erhalten.\n"
                                     "Details stehen in LogFile.").arg(host).arg(port),
                                  tr("Programm&Abbruch"),
                                  tr("&Wiederholen"),
                                  tr("&Simulation"),1,-1 );
    return userRM;
}

int wmMessageBox::msgBusy(QString host, int port)
{
    int userRM;
    userRM = QMessageBox::warning( 0, tr("TCP-Verbindung"),
                                  tr("Device ist busy\n"
                                     "( %1:%2 ).\n"
                                     "Details stehen in LogFile.").arg(host) .arg(port),
                                  tr("Programm&Abbruch"),
                                  tr("&Wiederholen"),
                                  tr("&Simulation"),
                                  1,-1 );
    return userRM;
}

void wmMessageBox::justDataWritten()
{
    QMessageBox::information(0,tr("Justage Data Written"),tr("Justage Data Written"));
}

