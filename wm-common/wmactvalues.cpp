#include "wmactvalues.h"
#include "qstringlist.h"

/* 27.3.26
 * Fixe Idee am Donnerstag abend für freie Forschung und entwicklung also heute.
 * Export der Messwerte als Json, damit man wenigstens weiß was das für Werte sein sollen.
 * Ist alles ganz einfach ab Qt5, gibt es dafür Klassen.
 * In Qt4.8.7 nocht nicht (25. Mai 2015)
 * Daher muss es hier erstmal per Hand gemacht werden, denke ich.
 * Beipeil aus wikipedia:
 *
     {
      "Herausgeber": "Xema",
      "Nummer": "1234-5678-9012-3456",
      "Deckung": 2e+6,
      "Waehrung": "EURO",
      "Inhaber":
      {
        "Name": "Mustermann",
        "Vorname": "Max",
        "Teilnahme am Bonusprogramm": true,
        "Hobbys": ["Reiten", "Golfen", "Lesen"],
        "Alter": 42,
        "Kinder": [],
        "Partner": null
      }
    }
 * Mit den Werten aus wmactvalues:
 *
 *
    {
    "Frequenz" : "",
    "PrimaryRMSN":
    {
    RMSN: "",
    RMSNSek: ""  // hier die je nach mode berechneten werte in SI einheiten primär, sekundär
    }
    "PrimaryRMSX":
    {
    RMSX:"",
    RMSXSek: ""
    }
    "PrimaryRMSN_fundamental":
    {
    RMSN1:"",
    RMSN1Sek: ""
    }
    "PrimaryRMSX"_fundamental":
    {
    RMSX1:"",
    RMSX1Sek:""
    }
    UInCorrektionVector
    {
        UInCorr.Real:"",
        UInCorr.Img:"",// der zugehörige korrekturvektor für un od. in
    }
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

//#include <QJsonDocument>
//#include <QJsonObject>

Aus gegebenem Anlass sei noch zu erwähnen, das qt 4.8.7 nicht über json export verfügt.
Daher die Lösung über die verfügbaren "Bordmittel"
*/

QString cwmActValues::extractJson()
{
    {
        QString str;
        QMap<QString, QVariant> map;

        map["frequnecy"] = Frequenz;

        QMap<QString, QVariant> PrimaryRMSN;
        PrimaryRMSN["RMSN"] = RMSN;
        PrimaryRMSN["RMSNSek"] = RMSNSek;
        map["PrimaryRMSN"] = PrimaryRMSN;

        QMap<QString, QVariant> PrimaryRMSX;
        PrimaryRMSX["RMSX"] = RMSX;
        PrimaryRMSX["RMSXSek"] = RMSXSek;
        map["PrimaryRMSX"] = PrimaryRMSX;

        QMap<QString, QVariant> PrimaryRMSN_fund;
        PrimaryRMSN_fund["RMSN1"] = RMSN1;
        PrimaryRMSN_fund["RMSN1Sek"] = RMSN1Sek;
        map["PrimaryRMSN_fundamental"] = PrimaryRMSN_fund;

        QMap<QString, QVariant> PrimaryRMSX_fund;
        PrimaryRMSX_fund["RMSX1"] = RMSX1;
        PrimaryRMSX_fund["RMSX1Sek"] = RMSX1Sek;
        map["PrimaryRMSX_fundamental"] = PrimaryRMSX_fund;

        map["UInCorrektionVector"] = jsonExport.addVektor(UInCorr);
        map["VektorN"] = jsonExport.addVektor(VekN);
        map["VektorNSek"] = jsonExport.addVektor(VekNSek);
        map["VektorX"] = jsonExport.addVektor(VekX);
        map["VektorXSek"] = jsonExport.addVektor(VekXSek);
        if(mWM3ki)
        {
            map["VektorDX"] = jsonExport.addVektor(VekDX);
            map["VektorDXSek"] = jsonExport.addVektor(VekDXSek);
        }

        map["AmplErrorIEC"] = AmplErrorIEC;
        map["AmplErrorANSI"] = AmplErrorANSI;
        map["AngleError"] = AngleError;
        map["RCF"] = RCF;
        map["PHIN"] = PHIN;
        map["PHIX"] = PHIX;

        str = jsonExport.variantToJson(map);

        return str;
    }
}

void cwmActValues::setWM3ki(bool b)
{
    mWM3ki = b;
}
