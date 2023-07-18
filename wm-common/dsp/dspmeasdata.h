#ifndef DSPMEASDATA_H
#define DSPMEASDATA_H

#include <QString>
#include <QTextStream>
#include <QIODevice>
#include <QVector>

#include "dspvar.h"

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
    QVector<float> DspVarData;
    // Q3MemArray has been replaced by QVector https://fiona.dmcs.pl/po/qt-tutorial/docs/qt/porting4.html#qmemarray-t
    QString m_sname;
    QString m_slist;
};


#endif // DSPMEASDATA_H
