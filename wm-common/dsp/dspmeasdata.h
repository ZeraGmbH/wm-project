#ifndef DSPMEASDATA_H
#define DSPMEASDATA_H

#include <QString>
#include <QTextStream>
#include <QIODevice>
#include <QVector>

#include "dspvar.h"

enum AVType {vApplication = 1, vDspIntern = 2, vMemory = 4}; // application variable type

template <typename T>
class cDspMeasData
{
public:
    cDspMeasData(QString name);
    QString VarList(); // liste mit allen variablen mit längenangaben
    QString MeasVarList(); // liste aller namen vom typ vapplikation bzw. vmemory
    QString& name();
    void addVarItem(cDspVar);
    T* data();
    uint getDataLenght();
    void reset();
protected:
    QList<cDspVar> DspVarList;
private:
    QString m_sname;
    // Q3MemArray has been replaced by QVector https://fiona.dmcs.pl/po/qt-tutorial/docs/qt/porting4.html#qmemarray-t
    QVector<T> DspVarData;
};

#endif // DSPMEASDATA_H
