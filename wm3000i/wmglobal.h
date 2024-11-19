// globaleWM definitionen

#ifndef WMGLOBAL_H
#define WMGLOBAL_H

#include <QDir>

#include  "wmglobalcommon.h"

//#define DEBUG 1

//#define FVWM 1

// V1.00 setzt auf wm3000u V1.04
// V1.01 zusätzliche anzeige lastpunkt relativ zu X kanal eingebaut
//             andere kette an dsp übertragen um die benötigten werte zu erhalten
//             den html browser für die online doku entfernt
// V1.02 in confmenu alle /3 /w3 buttons verlinkt und den safety range für evt entfernt da separater eingang für evt geschaffen wurde. den validator (regexp) verändert für die ratio eingaben verbessert, und den winkelfehler in der xml ergebnisdatei mit rad ausgegeben. die lastpunktberechnung für x ist jetzt relativ zur primärgrösse von n.
// V1.03 19.12.2011 fehler in complex klasse behoben (operator*, *=, /, /=)
// V1.04 21.12.2011 fenster übersteuerung ist nicht mehr modal -> alles läuft weiter incl. externe schnittstelle (checkcon). es wird ein merker OverLoad gesetzt und der overload im statussystem eingetragen. OverLoad wird von der bereichauswahl (sync) geprüft und das schalten der bereiche wird verhindert, egal ob das schalten automatisch, manuell oder über schnittstelle erfolgen sollte. wird das fenster übersteuerung quittiert, wird es geschlossen und der merker OverLoad gelöscht. das kontextmenu für die vektoranzeige wurde erweitert, damit man vektoren sekundär angezeigt bekommt. dazu ist es erforderlich das wandlerübersetzungsverhältnis für x auch im nonconvent betrieb aktiv zu lassen.
//bei der dateiname eingabe für session management wird ein name vorbesetzt
//das gleiche gilt für die xml datei auswahl für messwegebnisse speichern
// dito für die eigenfehlertabellen
// leere eigefehlertabellen waren nicht editierbar , d.h. man konnte aus dem wm-programm heraus keine eigenfehler
// tabellen editieren. dies ist jetzt möglich und es wird eine beispiel eingabezeile eingefügt.
// das komplette übersteurungs und bereichhandling musste nochmals überarbeitet werden :
// es wurden kommandos eingeführt zum setzen, rücksetzen der sensor kurzschluss fets weil der hardware
// schutz die hardware nicht schützt.......lach :-)
// wenn die soft übersteuerung greift werden im falle einer übersteuerung im grössten bereich die kurzschlussfets geschaltet
// die komplette overload überwachung wird in rangeobsermatic behandelt, bereiche setzen z.b. nimmt keine übersteuerung zurück. ob die übersteuerung weg ist wird hinterher nachgemessen -> absolut sauber
// das gleiche gilt für overloadmax, nachdem quittiert wurde (per hand oder *rst) wird gemessen und wenn ok overloadmax rückgesetzt.
// RegExp für transformer ratio geändert -> es werden jetzt mV,V,kV und KEINE einheit akzeptiert, wichtig für checkcon
// es wurde die hauptstate machine wm3000 dahingehend geändert, dass keine gleichartigen events kumulieren. etwaige performanceprobleme (die beim xscale tatsächlich vorhanden sind) fallen damit nicht mehr auf.
// die ansi fehlerberechnung wurde korrigiert.
// in eparameter wurde die liste der möglichen strom einheiten überarbeitet. die liste für die winkel wurdeum "grad" erweitert
// xml datei name bei flash data import/export vorbesetzt
// information ob justiert oder nicht in statuszeile, in xml bei ergebnisexport, in status system questionable bit 11
// V1.05 23.02.2012 eine soft übersteuerung in  einem der beiden kanäle wird wie die hard übersteuerungauf beide kanäle übertragen
// V1.06 22.03.2012 bereichautomatik nochmal geändert: overloadmax ist dann wenn beide kanäle im grössten bereich waren als übersteuerung erkannt wurde
// V1.07 02.07.2012 auflösung winkel fehler in xml datei auf 5 nachkommastellen erhöht
// V1.08 06.09.2012 bei der istwertabfrage über scpi lastpunkte auf kanal x umgestellt.
// V1.09 17.09.2012 kontextmenu für istwertanzeige um mrad erweitert, bei start des programms immer bereichautomatik ein,
//                  bei ergebnisdatei, eigenfehlerdatei solte open anstatt save beim öffnen verwendet werden, nicht machbar unter qt3 wenn man default selection verwenden möchte.
// v1.10 19.09.2012 in eparameter scalefactors eingeführt sonst fehler wenn /3 oder /w3 button
// v1.11 19.11.2012 dsp loader file location changed (only qt4 port)
// v1.12 29.11.2012 absolut messung eingeführt
// v2.13 14.12.2012 new version for difference old/new device and difference mode removed
// v2.14 oetfiles, resultfiles and ses files moved to /usr/share/wm3000i
// v2.15 in fehlermessung lastpunkte in [A] und AbsMode in rangedialogbase implementiert
// v2.16 neues ethernet routing für en61850-9-2 dekodierung eingebaut, cancel in confdialogbase ist jetzt wirklich cancel.
// v2.17 11.12.2013 resetOVL nur wenn wm im grössten bereich ist, wir haben dann schon auf
//                  hw-übersteuerung und/oder maximum sucher reagiert.
//                  nach jeder "umkonfiguration" wird der fehlermesswertund der phasenwinkel
//                  "inaktiv" und erst wieder aktiviert wenn ein neuer messwert da ist
//                  wenn |samplefrequenz - signalfrequenz| > 1.0 dann hinweis in statuszeile
//                  nicht justiert rot in statuszeile
//                  jede änderung in der konfiguration oder der anzeigefenster wird sofort
//                  gespeichert, damit die letzte konf. auch nach abschalten erhalten wird
// v2.18 19.12.2013 programm erweitert für wm1000i (nur conventional und ect)
// v2.19 28.05.2014 fehler in eigenfehlerkorrektur beseitigt. die einträge in der tabelle wurden falsch soriert
//                  wodurch die interpolation im anschluss falsche werte lieferte
// v2.20 27.06.2014 fehler in scpi interface beseitigt, es wurde die signalfrequenz nicht umgeschaltet.
//                  im confmenu source und destination adressen für nconvent umbenannt
//                  selbsttest inaktiv gesetzt wenn simulator betrieb
//                  pfad angaben für log files geändert und verzeichnis home/operator/wm3000i/log angelegt
//                  im kontextmenu für istwerte darstellung des lastpunktes in %,A,kA statt %,A,A (fehler)
//                  messwerte export xml seg.fault nach ads diff umstellung beseitigt
//                  selbsttest bewertung geändert, erforderlich wegen hardware änderung
//                  nach selbsttest erscheint eine messagebox mit info dass Test beendet
//                  releaseinfo file path hinzugefügt für releaseausgabe
// v2.21 ??.??.???? einfach so
// v2.22 26.06.2015 eparameter test so geändert, dass fehler in der angehängten einheit bemerkt werden.
// v2.23 30.08.2016 phasenjustage korrigiert (ECT justage funktioniert wieder), polnische Sprache, statusbar permanent
// v2.24 29.09.2017 confmenu x ratio datenübernahme auch bei nConvent
// v2.25 14.02.2018 bugfix : die wm3000 hängt sich auf wenn via interface ein zu großer wert messperioden gesendet wird.
//                  das konnte passieren wenn die samplerate von 80 auf 256 gesetzt wurde. im gui wurde das automatisch
//                  korrigiert. jetzt wird zentral am gerät korrigiert und und gui und interface informiert.
// v2.26 25.04.2018 bugfix : wenn das sampling system gesetzt wird, z.B. nach *rst, kann es sein dass der dsp busy meldet.
//                  die wm3000 blieb bis jetzt dann mit fehlermeldung unexpected answer stehen . busy wird jetzt recovert.
//                  einbau berechnung rcf ratio correction factor. und grossanzeige um rcf erweitert. interface (measure)
//                  erweitert. kontext menu ergänzt für die nachkommastellenzahl von rcf zu editieren.
// v2.27 16.08.2018 Andreas Müller keine inhaltliche Änderung nur version hochgezählt.
// v2.28 05.11.2019 neue kommandos für first asdu und last asdu und der automatismus aus dem bildschirmdialog
//                  wurde übernommen dass wenn die anzahl der samples gesetzt wird ... dann ebenfalls die asdu's
//                  mit gesetzt werden. *tst? antwortet jetzt richtig mit +0 oder +1. *opc? snchronisiert jetzt
//                  auf fertig werden der konfiguration
// v2.29 09.12.2019 bugfix : rücksetzen noOperationCompleteFlag wenn conf. ready
// v2.30 19.11.2019 tausend und eine änderung für dc betrieb
// v2.31 23.09.2020 einfach hochgezählt nach gefühlt jahrzehnten bis zum merge für dc betrieb
// v2.32 16.11.2020 phasenjustage adc's wieder eingebaut und status gain, phase, offset vereinzelt.
// v2.33 15.04.2021 es wird nach pps nur noch 1x über die eingestellte perioden anzahl gemessen.
//                  hintergrund : es lässt sich so das rauschen des eingangssignals beurteilen.
//                  die minimale anzahl perioden ist jetzt 1 anstatt 4.
//                  die integrationszeit (sek.) ist jetzt als movingwindow filter ausgeführt.
//                  das interface läuft jetzt stabil bei voller geschwindigkeit.
//                  opc? liefert fertig erst nachdem alle conf. kommandos abgearbeitet wurden.
//                  eth kommando, query hinzugefügt um en61850:sync:strong setzen und abfragen zu können
// v2.34 10.05.2021 es sollen die neuen sample rate für die non conventional wandler integriert werden
//                  im groben: das konfigurations menu muss angepasst werden. es gibt verriegelungen
//                  zwischen samplerate und signalfrequenz. die anzahl der signalperioden ist samplerate
//                  abhängig. die anzahl der asdu's in einer apdu ist ebenfalls davon abhängig. das scpi
//                  interface musste angepasst werden. der automatische phasenabgleich war entsprechend
//                  zu erweitern. die neue version läuft nur mit dem leiterkarten server ab v2.09.
// V2.35 05.05.2023 changes see git@github.com:ZeraGmbH/wm-project.git
//                  WM got stuck on INIT Command when sync was set to extern
// V2.36 08.09.2023 Tons of changes documented in github
// V2.37 28.11.2023 cmpactvalues added under test
// V2.38 31.1.2024  changes cofdialogbase to delete dc correction on non dc WMs
// V2.39            Actual Values take 100 calculate last 20
// V2.40 12.6.24    Adds overload to special class cwmmessagebox
// 2.41 21.8.24     adds bugfix on scpi interface
// 2.42 29.8.24     adds dev:iface to export interface
// 2.43 16.9.24     adds onscreenkeyboard
// 2.44 19.11.24    adds disable Watchdogtimer

#define WMVersion "V2.44"


// #define ServerCommLogFilePath "/usr/share/wm3000i/log/ServerComm.log"
#define ServerCommLogFilePath QDir::homePath()+"/wm3000i/log/ServerComm.log"
//#define SelftestLogFilePath "/usr/share/wm3000i/log/Selftest.log"
#define SelftestLogFilePath QDir::homePath()+"/wm3000i/log/Selftest.log"
//#define PhaseJustLogFilePath "/usr/share/wm3000u/log/PhaseJust.log"
#define PhaseJustLogFilePath QDir::homePath()+"/wm3000i/log/PhaseJust.log"
#define OffsetJustLogFilePath QDir::homePath()+"/wm3000i/log/OffsetJust.log"
#define OffsetJustDataFilePath QDir::homePath()+"/wm3000i/offsetdata"
#define NSAOffsetJustDataFilePath QDir::homePath()+"/wm3000i/.offsetinfo"

// alt jetzt sensemode enum tsmode {sensNsensX, adcNadcX, sensNadcX, sensXadcN, adcXadcN = 5, sensXadcNECT = 11}; // testmodi innerhalb der hardware
enum MeasMode {In_IxDiff,In_ECT,In_nConvent,In_IxAbs,maxMMode}; // messmodi, in_ixabs wird (wurde) nur für justage zwecke verwendet

class cJustMeasInfo : public cJustMeasInfoBase, public cJustMeasInfoBase2nd
{
public:
    cJustMeasInfo(const QString rngN, const QString rngX, const QString rngStore, SenseMode sm, MeasMode mm, JustMode jm, int nS, int nIgn, int nMeas ):
        cJustMeasInfoBase(rngN, rngX, rngStore, sm),
        cJustMeasInfoBase2nd(jm, nS, nIgn, nMeas),
        m_nMMode(mm) {}

    MeasMode m_nMMode; // in welchem messmodus
};

typedef std::vector<std::unique_ptr<cJustMeasInfo>> cPhaseNodeMeasInfoList;
typedef Q3PtrList<cJustMeasInfo> cOffsetMeasInfoList;

#endif
