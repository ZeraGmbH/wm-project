// headerfile für zera info dienste 

#ifndef ZERAINFO_H
#define ZERAINFO_H

#include <qobject.h>

class cZeraInfo:public QObject
{
    Q_OBJECT
public:
    cZeraInfo(QString machineInfoTitle, QString machineInfo);
public slots:
    void AboutZeraSlot();
    void AboutWM3000Slot();
private:
    QString m_machineInfoTitle;
    QString m_machineInfo;
};

#endif	
