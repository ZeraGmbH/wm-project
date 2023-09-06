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

void PhastJustHelpers::deleteFaultyPhasenJustageItem(float mittel, float diff, QList<float> *anglesList)
{
    float val, highTrigger, lowTrigger;
    if (diff > 0.4){
        highTrigger = (mittel + (diff*0.8));
        lowTrigger = (mittel - (diff*0.8));
        for (int i(0); i < anglesList->count(); i++ ){
            val = anglesList->at(i);
            if ((val > highTrigger) || (val < lowTrigger)){
                anglesList->removeAt(i);
            }
        }
    }
}

void PhastJustHelpers::calculateMinMaxDiffValues(QList<float> *angleList)
{
    qreal min(359.99), max(0.0), diff(0.0), value(0.0), ph0(0.0);
    for (int i = 0; i < angleList->count(); i++){
        value = angleList->at(i);
        if( min > value) min = value;
        if (max < value) max = value;
        ph0 -= value;
    }
    diff = (max - min) * 60.0;  // 1,6858° -> 1° 41' 9" -> 41,1516'
    ph0 /= angleList->count();
    m_fMin = min;
    m_fMax = max;
    m_fDiff = diff / 60.0;
    m_fMean = ph0 *-1;
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

