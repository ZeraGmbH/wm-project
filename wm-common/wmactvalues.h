#ifndef WMACTVALUES_H
#define WMACTVALUES_H

#include "dspvar.h"
#include "dspactvalues.h"
#include "complex.h"
#include <Q3MemArray>
#include <q3ptrlist.h>

#include <qmap.h>
#include "qvariant.h"

typedef Q3MemArray<float> cDspVarMemArray;
typedef Q3PtrList<cDspVar> cDspVarPtrList;

class cwmActValues {    // wird an andere objekte gesendet
public:
    cwmActValues();
    cDspActValues dspActValues;
    cDspFastRMSValues dspRMSValues;
    double TDSync;
    double Frequenz;
    double RMSN, RMSNSek;  // hier die je nach mode berechneten werte in SI einheiten primär, sekundär
    double RMSX, RMSXSek; // dito
    double RMSN1, RMSN1Sek;
    double RMSX1, RMSX1Sek;
    complex UInCorr;   // der zugehörige korrekturvektor für un od. in
    complex VekN, VekNSek; // die vektoren
    complex VekX, VekXSek;
    complex VekDX, VekDXSek;
    double LoadPoint, LoadPoint1; // Lastpunkt v. rms total, bzw. rms grundwelle
    double LoadPointX, LoadPoint1X; // Lastpunkt v. rms total, bzw. rms grundwelle kanal x
    double AmplErrorIEC;
    double AmplErrorANSI;
    double AngleError;
    double RCF;
    double PHIN;
    double PHIX;
    bool bvalid;

    QString extractJson();
    void setWM3ki(bool b);
private:
    bool mWM3ki;
    QString maptoJson(QMap<QString, QVariant> map);
    QMap<QString, QVariant> addVektor(complex vector);
    QString escapeString(const QString &str);
    QString variantToJson(const QVariant &value);
};


#endif // WMACTVALUES_H
