#include "dspmeasdata.h"

cDspMeasDataBase::cDspMeasDataBase(QString name) : m_sname(name)
{
}

QString cDspMeasDataBase::MeasVarList()
{
    QString str = QString();
    QTextStream ts( &str, QIODevice::WriteOnly );
    foreach ( cDspVar DspVar, DspVarList )
        if ((DspVar.type() & (vApplication | vMemory)) > 0) ts << QString("%1;").arg(DspVar.Name());
    return str;
}

QString cDspMeasDataBase::VarList()
{
    QString str = QString();
    QTextStream ts( &str, QIODevice::WriteOnly );
    foreach ( cDspVar DspVar, DspVarList )
        ts << QString("%1,%2;").arg(DspVar.Name()).arg(DspVar.size());
    return str;
}


QString& cDspMeasDataBase::name()
{
    return m_sname;
}


cDspMeasData::cDspMeasData(QString name) :
    cDspMeasDataBase(name)
{
}


uint cDspMeasData::getDataLenght()
{
    return DspVarData.size();
}

float* cDspMeasData::data() // gibt einen zeiger zurück auf die var daten vom typ vapplication
{
    return DspVarData.data();
}


void cDspMeasData::addVarItem(cDspVar var) // eine neue dsp variable
{
    DspVarList.append(var);
    // wenn var für application relevant -> platz reservieren für das ergebnis
    if ( (var.type() & (vApplication | vMemory)) > 0)
        DspVarData.resize ( DspVarData.size() + var.size());
}



cDspMeasDataUlong::cDspMeasDataUlong(QString name) :
    cDspMeasDataBase(name)
{
}


uint cDspMeasDataUlong::getDataLenght()
{
    return DspVarData.size();
}


void cDspMeasDataUlong::reset()
{ //sets the defined variables to '0'
    foreach (ulong lval, DspVarData){
        lval = 0;
    }
}


ulong* cDspMeasDataUlong::data() // gibt einen zeiger zurück auf die var daten vom typ vapplication
{
    return DspVarData.data();
}


void cDspMeasDataUlong::addVarItem(cDspVar var) // eine neue dsp variable
{
    DspVarList.append(var);
    // wenn var für application relevant -> platz reservieren für das ergebnis
    if ( (var.type() & (vApplication | vMemory)) > 0)
        DspVarData.resize ( DspVarData.size() + var.size());
}
