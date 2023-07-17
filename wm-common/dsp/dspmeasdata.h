#ifndef DSPMEASDATA_H
#define DSPMEASDATA_H

#include <QString>
#include <QTextStream>
#include <QIODevice>

#include "dspvar.h"
#include <Q3MemArray>

enum AVType {vApplication = 1, vDspIntern = 2, vMemory = 4}; // application variable type

class cDspMeasData
{
public:
    cDspMeasData(QString); // name des messdaten satzes
    ~cDspMeasData();
    float* data();
    void addVarItem(cDspVar*);
    QString& VarList(); // liste mit allen variablen mit längenangaben
    QString& MeasVarList(); // liste aller namen vom typ vapplikation bzw. vmemory
    QString& name();
    uint getDataLenght();

private:
    QList<cDspVar*> DspVarList;
    Q3MemArray<float> DspVarData;
    QString m_sname;
    QString m_slist;
};


#endif // DSPMEASDATA_H
