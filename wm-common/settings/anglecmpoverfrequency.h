#ifndef ANGLECMPOVERFREQUENCY_H
#define ANGLECMPOVERFREQUENCY_H

#include <QObject>
#include <math.h>
#include "confdata.h"
#include "wmactvalues.h"

/*
 * WM3000I and WM3000U
 * 80 Samples/Per. 50Hz à 0,656°/Hz
 * 80 Samples/Per. 60Hz à 0,546°/Hz
 * 256 Samples/Per. 50Hz à 0,789°/Hz
 * 256 Samples/Per. 60Hz à 0,649°/Hz
 */

const double samp80Freq50 = 0.656 / 180 * M_PI;
const double samp80Freq60 = 0.546 / 180 * M_PI;
const double samp256Freq50 = 0.777 / 180 * M_PI;
const double samp256Freq60 = 0.648 / 180 * M_PI;
const double samp80Freq1623 = 1.965 / 180 * M_PI;
const double samp256Freq1623 = 2.3305 / 180 * M_PI;

class angleCmpOverFrequency : public QObject
{
    Q_OBJECT
public:
    angleCmpOverFrequency();
    angleCmpOverFrequency(cConfData *confData, cwmActValues *actValues);
    double getCompValue(int sampleRate, int confFrequency);
    bool isFreuqencyInRange(double frequency);
    double correctAngle(double Angle, double actualFrequency, double confFrequency);
    double getAngleCorrectionValue();
private:
    double m_angleCorrectionFactor;
    double m_angleCorrectionValue;

    int getSampleRate(int sr);
};

#endif // ANGLECMPOVERFREQUENCY_H
