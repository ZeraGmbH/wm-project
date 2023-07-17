#include "dspmeasdata.h"


cDspMeasData::cDspMeasData(QString s)
{
    m_sname = s;
    //DspVarList.setAutoDelete(true);
}

cDspMeasData::~cDspMeasData()
{
    foreach (cDspVar *poi, DspVarList) {
        delete poi;
    }
}


float* cDspMeasData::data() // gibt einen zeiger zurück auf die var daten vom typ vapplication
{
    return DspVarData.data();
}


void cDspMeasData::addVarItem(cDspVar* var) // eine neue dsp variable
{
    DspVarList.append(var);
    // wenn var für application relevant -> platz reservieren für das ergebnis
    if ( (var->type() & (vApplication | vMemory)) > 0)
        DspVarData.resize ( DspVarData.size() + var->size());
}


QString& cDspMeasData::MeasVarList()
{
    //    QString list;
    m_slist="";
    QTextStream ts( &m_slist, QIODevice::WriteOnly );
    foreach ( cDspVar *DspVar, DspVarList )
        if ((DspVar->type() & (vApplication | vMemory)) > 0) ts << QString("%1;").arg(DspVar->Name());
    return m_slist;
}


QString& cDspMeasData::VarList()
{
    //    QString vlist;
    m_slist="";
    QTextStream ts( &m_slist, QIODevice::WriteOnly );
    foreach ( cDspVar *DspVar, DspVarList )
        ts << QString("%1,%2;").arg(DspVar->Name()).arg(DspVar->size());
    return m_slist;
}


QString& cDspMeasData::name()
{
    return m_sname;
}

uint cDspMeasData::getDataLenght()
{
    return DspVarData.size();
}
