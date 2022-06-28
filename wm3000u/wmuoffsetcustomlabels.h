#ifndef WMUOFFSETCUSTOMLABELS_H
#define WMUOFFSETCUSTOMLABELS_H

#include <wmoffsetcustomlabels.h>

class WmuOffsetCustomLabels : public IWmOffsetCustomLabels
{
public:
    void setUiTexts(QLabel* nLabel, QLabel* xLabel) override;
    void updateValues(QLabel* nValue, QLabel* xValue, cConfData* conf, tJustValues* just) override;
};

#endif // WMUOFFSETCUSTOMLABELS_H
