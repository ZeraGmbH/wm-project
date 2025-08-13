#ifndef SCPIHELPER_H
#define SCPIHELPER_H

#include <QObject>
#include "parse.h"

class scpiHelper
{
public:
    scpiHelper(cParse* m_pParser);
    bool GetTransformerRatio(char**, QString&, QString&, bool);
    int getErrors() {return m_Error;};
private:
    cParse *m_pParser;
    int m_Error;
    void AddEventError(int e);
};

#endif // SCPIHELPER_H
