#include "dspmeasdata.h"

template <typename T>
cDspMeasData<T>::cDspMeasData(QString name) : m_sname(name)
{
}

template <typename T>
QString cDspMeasData<T>::MeasVarList()
{
    QString str = QString();
    QTextStream ts( &str, QIODevice::WriteOnly );
    foreach ( cDspVar DspVar, DspVarList )
        if ((DspVar.type() & (vApplication | vMemory)) > 0) ts << QString("%1;").arg(DspVar.Name());
    return str;
}

template <typename T>
QString cDspMeasData<T>::VarList()
{
    QString str = QString();
    QTextStream ts( &str, QIODevice::WriteOnly );
    foreach ( cDspVar DspVar, DspVarList )
        ts << QString("%1,%2;").arg(DspVar.Name()).arg(DspVar.size());
    return str;
}

template <typename T>
QString& cDspMeasData<T>::name()
{
    return m_sname;
}

template <typename T>
uint cDspMeasData<T>::getDataLenght()
{
    return DspVarData.size();
}

template <typename T>
T* cDspMeasData<T>::data() // gibt einen zeiger zurück auf die var daten vom typ vapplication
{
    return DspVarData.data();
}

template <typename T>
void cDspMeasData<T>::addVarItem(cDspVar var) // eine neue dsp variable
{
    DspVarList.append(var);
    // wenn var für application relevant -> platz reservieren für das ergebnis
    if ( (var.type() & (vApplication | vMemory)) > 0)
        DspVarData.resize ( DspVarData.size() + var.size());
}

template <typename T>
void cDspMeasData<T>::reset()
{ //sets the defined variables to '0'
    foreach (T val, DspVarData){
        val = 0;
    }
}

template class cDspMeasData<float>;
template class cDspMeasData<ulong>;
