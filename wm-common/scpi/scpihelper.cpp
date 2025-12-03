#include "scpihelper.h"
#include "qregexp.h"
#include "scpierrorindicator.h"
#include "wmparameter.h"


scpiHelper::scpiHelper(cParse *poiParser) :
    m_pParser(poiParser)
{

}

bool scpiHelper::GetTransformerRatio(char** s, QString& ps, QString& ss, bool chkEnd, int iChan)
{
    bool ok = true ;
    int i;
    QString par[2];
    WmParameter ep[2];
    m_Error = 0;

    QString ws = m_pParser->SetWhiteSpace(" :"); // wir definieren whitespace um

    for ( i = 0; i < 2; i++)
        par[i] = m_pParser->GetKeyword(s); //  2  parameter lesen

    m_pParser->SetWhiteSpace(ws); // jetzt wieder zurück

    if (chkEnd && !(m_pParser->GetKeyword(s)).isEmpty() )
    {
        AddEventError(InvalidSeparator);
        return false;
    }

    if ( par[0].isEmpty() || par[1].isEmpty() )
    {
        AddEventError(MissingParameter);
        return false;
    }

    presetUnits(par[0],par[1],iChan);

    if (iChan == 0) // Current N, X
    {
        for ( i = 0; i < 2; i++)
        {
            ep[i] = par[i];
            if ( !( ep[i].isCurrent()))
                ok = false;
        }
    }
    if (iChan == 1) // voltage N, X, EVT
    {
        for ( i = 0; i < 2; i++)
        {
            ep[i] = par[i];
            if ( !( ep[i].isVoltage()))
                ok = false;
        }
    }
    if (iChan == 2) // ECT
    {
        for ( i = 0; i < 2; i++)
        {
            ep[i] = par[i];
            if ( !(  ep[i].isCurrent() || ep[i].isVoltage()))
                ok = false;
        }
    }

    if ( !ok)
    {
        AddEventError(ParameterNotAllowed);
        return false;
    }

    ps = par[0];
    ss = par[1];

    return true;
}

void scpiHelper::presetUnits(QString &par1, QString &par2, int iChan)
{
    QString unit1, unit2;
    switch (iChan)
    {
    case 0 :unit1 = "A";
            unit2 = "A";
            break;
    case 1 :unit1 = "V";
            unit2 = "V";
            break;
    case 2 :unit1 = "A";
            unit2 = "V";
            break;
    }
    if (!par1.contains(unit1))
    {
        if (par1.contains("/"))
            par1.insert(par1.indexOf("/"),unit1);
        else
            par1.append(unit1);
    }
    if (!par2.contains(unit2))
    {
        if(par2.contains("/"))
            par2.insert(par2.indexOf("/"),unit2);
        else
            par2.append(unit2);
    }
}

void scpiHelper::AddEventError(int e)
{
    m_Error |= e;
}
