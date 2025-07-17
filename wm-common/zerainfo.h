// headerfile für zera info dienste 

#ifndef ZERAINFO_H
#define ZERAINFO_H

#include <qobject.h>

enum wmInfoEnum {
    wmInfoWm1000I = 0,
    wmInfoWm3000I,
    wmInfoWm1000U,
    wmInfoWm3000U
};

class cZeraInfo:public QObject
{
    Q_OBJECT
public:
    cZeraInfo(int nr);
    bool isWM3000();
public slots:
    void AboutZeraSlot();
    void AboutWM3000Slot();
private:
    void setInfoStrings(int nr);
    QString m_machineInfoTitle;
    QString m_machineInfo;
    int m_nr;

};

#endif	
