#ifndef EUNIT_H
#define EUNIT_H

#include <QString>
#include <QDataStream>

class eUnit {
public:
    eUnit() {};
    eUnit(QString s, double d) : Name(s), fak(d) {};
    friend QDataStream& operator << (QDataStream& ds, eUnit& eu);
    friend QDataStream& operator >> (QDataStream& ds, eUnit& eu);
    QString Name;
    double fak;
};


#endif // EUNIT_H
