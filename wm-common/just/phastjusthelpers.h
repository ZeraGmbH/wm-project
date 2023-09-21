#ifndef PHASTJUSTHELPERS_H
#define PHASTJUSTHELPERS_H

#include <QList>
#include <QTextStream>

class PhastJustHelpers
{
public:
    PhastJustHelpers();
    bool deleteFaultyPhasenJustageItem(float mittel, float diff, QList<float> *anglesList);
    void calculateMinMaxDiffValues(QList<float> *angleList, bool log = true);
    void addToLog(QString str);
    QString getLogString();
    void clearLogStr();
    float getMinValue();
    float getMaxValue();
    float getDiffValue();
    float getMeanValues();

private:
    float m_fMin;
    float m_fMax;
    float m_fDiff;
    float m_fMean;

    QString m_logString;
};

#endif // PHASTJUSTHELPERS_H
