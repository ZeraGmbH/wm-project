#ifndef DSPMEASDATA_H
#define DSPMEASDATA_H

#include <QString>
#include <QTextStream>
#include <QIODevice>
#include <QVector>

#include "dspvar.h"

enum AVType {vApplication = 1, vDspIntern = 2, vMemory = 4}; // application variable type

class cDspMeasDataBase
{
public:

    QString& VarList(); // liste mit allen variablen mit längenangaben
    QString& MeasVarList(); // liste aller namen vom typ vapplikation bzw. vmemory
    QString& name();

protected:
    QList<cDspVar> DspVarList;
    // Q3MemArray has been replaced by QVector https://fiona.dmcs.pl/po/qt-tutorial/docs/qt/porting4.html#qmemarray-t
    QString m_sname;
    QString m_slist;
};


class cDspMeasDataUlong : public cDspMeasDataBase
{
public:
    cDspMeasDataUlong(QString); // name des messdaten satzes
    void addVarItem(cDspVar);
    ulong* data();
    uint getDataLenght();
    void reset();
private:
    QVector<ulong> DspVarData;
};


class cDspMeasData : public cDspMeasDataBase
{
public:
    cDspMeasData(QString); // name des messdaten satzes
    void addVarItem(cDspVar);
    float* data();
    uint getDataLenght();
private:
    QVector<float> DspVarData;
};


#endif // DSPMEASDATA_H
