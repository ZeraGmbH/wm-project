#ifndef PHASTJUSTHELPERS_H
#define PHASTJUSTHELPERS_H

#include <QList>
#include <QTextStream>

class PhastJustHelpers
{
public:
    PhastJustHelpers();
    void deleteFaultyPhasenJustageItem(float mittel, float diff, QList<float> *anglesList);
    void calculateMinMaxDiffValues(QList<float> *angleList);
    float getMinValue();
    float getMaxValue();
    float getDiffValue();
    float getMeanValues();

private:
    float m_fMin;
    float m_fMax;
    float m_fDiff;
    float m_fMean;
};

#endif // PHASTJUSTHELPERS_H
