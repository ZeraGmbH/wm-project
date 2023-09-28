#include "phastjusthelpers.h"

// we found that there are values that do not match our expectations e.g.: 19 times 0.0343149 but one : 1.72088
// this will lead to Justgae issue! solving this might be done:
//  1) by finding the root cause of the issue
//  2) by repeating the measurement
//  3) by deleting the faulty measurement
// alldough the first would be the right solution and the second is a right solution, we are going to use the third approach!


PhastJustHelpers::PhastJustHelpers()
{

}

bool PhastJustHelpers::deleteFaultyPhasenJustageItem(float mittel, float diff, QList<float> *anglesList)
{
    bool retValue(false);
    float val, highTrigger, lowTrigger;
    if (diff > 0.4){
        highTrigger = (mittel + (diff*0.8));
        lowTrigger = (mittel - (diff*0.8));
        for (int i(0); i < anglesList->count(); i++ ){
            val = anglesList->at(i);
            if ((val > highTrigger) || (val < lowTrigger)){
                anglesList->removeAt(i);
                addToLog(QString("removed Item Nr.: %1 ; val: %2").arg(i).arg(val));
                retValue = true;
            }
        }
    }
    return retValue;
}

void PhastJustHelpers::calculateMinMaxDiffValues(QList<float> *angleList, bool log)
{
    qreal min(359.99), max(0.0), diff(0.0), value(0.0), ph0(0.0);
    for (int i = 0; i < angleList->count(); i++){
        value = angleList->at(i);
        if (log) addToLog(QString("Ph0: %1").arg(value));
        if( min > value) min = value;
        if (max < value) max = value;
        ph0 -= value;
    }
    diff = (max - min) * 60.0;  // 1,6858° -> 1° 41' 9" -> 41,1516'
    addToLog(QString("Diff: %1 Minuten").arg(diff));
    ph0 /= angleList->count();
    m_fMin = min;
    m_fMax = max;
    m_fDiff = diff / 60.0;
    m_fMean = ph0 *-1;
}

bool PhastJustHelpers::hasBelly()
{
    bool retValue;
    retValue = (m_fDiff > 1.0/60.0);
    if (retValue) addToLog(QString("Belly detected: %1").arg(m_fDiff));
    return retValue;
}

void PhastJustHelpers::addToLog(QString str)
{
    m_logString.append(str+"\n");
}

QString PhastJustHelpers::getLogString()
{
    return m_logString;
}

void PhastJustHelpers::clearLogStr()
{
    m_logString.clear();
}

float PhastJustHelpers::getMinValue()
{
    return m_fMin;
}

float PhastJustHelpers::getMaxValue()
{
    return m_fMax;
}

float PhastJustHelpers::getDiffValue()
{
    return m_fDiff;
}

float PhastJustHelpers::getMeanValues()
{
    return m_fMean;
}

