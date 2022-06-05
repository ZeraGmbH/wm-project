#ifndef CDSPVAR_H
#define CDSPVAR_H

#include <QString>

class cDspVar
{
public:
    cDspVar(QString name, int size, int type) : m_sName(name), m_nsize(size), m_nType(type) {};
    QString& Name() { return m_sName;};
    int size() { return m_nsize; };
    int type() { return m_nType; };
private:
    QString m_sName; // hat einen namen
    int m_nsize; // besitzt eine anzahl v. elementen
    int m_nType; // ist vapplication und/oder vdspintern
};

#endif // CDSPVAR_H
