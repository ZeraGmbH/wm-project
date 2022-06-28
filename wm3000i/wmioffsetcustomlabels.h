#ifndef WMIOFFSETCUSTOMLABELS_H
#define WMIOFFSETCUSTOMLABELS_H

#include <wmoffsetcustomlabels.h>

class WmiOffsetCustomLabels : public IWmOffsetCustomLabels
{
public:
    void setUiTexts(QLabel *nLabel, QLabel *xLabel) override;
    void updateValues(QLabel *nValue, QLabel *xValue, cConfData* conf, tJustValues* just) override;
};

#endif // WMIOFFSETCUSTOMLABELS_H
