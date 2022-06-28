#ifndef WMVIEWOFFSETVALUELABELS_H
#define WMVIEWOFFSETVALUELABELS_H

#include "confdata.h"
#include "justValues.h"
#include <QLabel>

class IWmOffsetCustomLabels
{
public:
    virtual ~IWmOffsetCustomLabels() { }
    virtual void setUiTexts(QLabel *nLabel, QLabel *xLabel) = 0;
    virtual void updateValues(QLabel *nValue, QLabel *xValue, cConfData *conf, tJustValues *just) = 0;
};

#endif // WMVIEWOFFSETVALUELABELS_H
