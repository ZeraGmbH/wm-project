#include "scpihelper.h"
#include "scpierrorindicator.h"
#include "wmparameter.h"


scpiHelper::scpiHelper(cParse *poiParser) :
    m_pParser(poiParser)
{

}

bool scpiHelper::GetTransformerRatio(char** s, QString& ps, QString& ss, bool chkEnd)
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

    for ( i = 0; i < 2; i++)
    {
        ep[i] = par[i];
        if ( !( ep[i].isVoltage() || ep[i].withoutUnit() ))
            ok = false;
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

void scpiHelper::AddEventError(int e)
{
    m_Error |= e;
}
