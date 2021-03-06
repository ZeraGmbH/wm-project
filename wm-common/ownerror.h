// headerfile für eigenfehlerklasse (cOwnError) .
// es wird eine struktur zur anzeigenübergabe definiert (cOwnErrorView). 
// die klasse erhält einen slot zur übergabe von konfigurationsdaten und 
// einen für die übergabe von istwerte. aus den konfigurationsdaten wird 
// entnommen, ob eine eigenfehlertabelle geladen ist und welche über-
// setzungsverhälnisse verwendet werden. wenn istwerte empfangen werden,
// kann der aktuelle lastpunkt bestimmt werden. dann wird aus der geladenen
// tabelle der eigenfehler für betrag und phase entnommen. 
// cOwnError berechnet auf anfrage einen komplexen korrekturvektor und
// generiert dann auch neue anzeigedaten welche mittels signal emittiert werden

#ifndef COWNERROR_H
#define COWNERROR_H

#include <qobject.h>
#include <qstring.h>
#include <qstringlist.h>

#include "ownerrorviewdata.h"
#include "wmactvalues.h"
#include "wmparameter.h"
#include "confdata.h"
#include "complex.h"

class IOwnErrorParamUISpecific
{
public:
    virtual ~IOwnErrorParamUISpecific() { }
    virtual bool checkUISpecificInvalidParam(WmParameter* param) = 0;
};

class cOwnError:public QObject
{
    Q_OBJECT
    
public:
    cOwnError(IOwnErrorParamUISpecific* uiSpecificParamCheck);
    virtual ~cOwnError();
    
    complex GetOECorrVector();
    QStringList& GetNPrimList(); // abfrage der liste der primärstufen des verwendeten normalwandlers;
    QStringList& GetNSekList(); // liste der sekundärstufen des verwendeten normalwandlers;
 
public slots:
    void SetConfInfoSlot(cConfData*);
    void SetActualValuesSlot(cwmActValues*);
    
signals:
    void SendOEViewData(cOwnErrorViewData*);
    void SendAnalizeDone(void);
    
private:
    complex CmpOECorrVector();
    bool isValidEntry(QString);
    QString strippedName(QString);
    complex OEVector;
    cOwnErrorViewData *m_pViewData;
    cConfData m_ConfData;
    cwmActValues m_ActValues;
    QString m_sOEFileName; // name der geladenen eigenfehler datei
    QStringList m_sNPrimList,m_sNSekList;
    QStringList m_sOEEntriesList; // liste aller eigenfehler einträge -> wir brauchen datei nur 1x lesen
    double m_OEAmplitude; // eigenfehler amplitude
    double m_OEPhase; // eigenfehler phase
    IOwnErrorParamUISpecific* m_uiSpecificParamCheck;
};


#endif
