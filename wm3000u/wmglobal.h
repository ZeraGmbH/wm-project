// globaleWM definitionen

#ifndef WMGLOBAL_H
#define WMGLOBAL_H

#include <QDir>

#include  "wmglobalcommon.h"

//#define FVWM 1


// V1.01 erste lauffähige version
// V1.02 wm3000scpiface geändert -> konfiguration abfrage, setzen besser synchronisiert
// V1.03 wm3000u schrittkette umgebaut, so dass sk messperiode selbst verwaltet und
//       im dsp messperiode = signalperiode ist -> befehle werden schneller bearbeitet.
//       das bedeutet aber mehr speicherbedarf im userworkspace deshalb muss die anzahl
//       der messperioden auf 18 begrenzt werden wenn 256samples/per.
// V1.04 arbeitet mit der neuen version des leiterkartenservers V1.02 zusammen.
//       Das justage handling wurde hier grundlegend geändert. bei der abfrage des korrekturwerte
//       wird der hierzu erforderliche parameter mitgesendet, vorher wurde der parameter mit einem
//       separaten befehl gesetzt. Die berechnung der justage koeffizienten erfolgt jetzt direkt im
//       server und nicht mehr im device. das heisst dass auch im justageteil des devices einiges
//       geändert werden musste.
// V1.05 cethadress operator != verbessert,  
//            zusätzliche anzeige lastpunkt relativ zu X kanal eingebaut
//            andere kette an dsp übertragen um die benötigten werte zu erhalten
//            den html browser für die online doku entfernt
// V1.06 in confmenu alle /3 /w3 buttons verlinkt und den safety range für evt entfernt da separater eingang für evt geschaffen wurde. den validator (regexp) verändert für die ratio eingaben verbessert, und den winkelfehler in der xml ergebnisdatei mit rad ausgegeben. berechnung der lastpunkte auf primärgrössen umgestellt, für X wird der Lastpunkt über die über N abgeleitete primärgrösse bestimmt.
// V1.07 durch wegfall des safety range wurde der evt 15v bereich nicht mehr phasenjustiert. 
// dies wurde korrigiert
// V1.08 19.12.2011 complex klasse geändert. fehler in der mult. und div. routine
// V1.09 17.01.2012 	accelerator keys geändert weil  f4 von mindowmanager beeinflusst wurde
//	        	bei der dateiname eingabe für session management wird ein name vorbesetzt
//		das gleiche gilt für die xml datei auswahl für messwegebnisse speichern
// 		und ebenfalls für die eigenfehlertabelle
// leere eigefehlertabellen waren nicht editierbar , d.h. man konnte aus dem wm-programm heraus keine eigenfehler
// tabellen editieren. dies ist jetzt möglich und es wird eine beispiel eingabezeile eingefügt.
// RegExp für transformer ratio geändert -> es werden jetzt mV,V,kV und KEINE einheit akzeptiert, wichtig für checkcon 
// vektoranzeige contextmenu ergänzt umschaltung primär/sekundär , hierzu mussten auch die istwerte entsprechend berechnet werden, bei nConvent bleibt für x das wandlerverhältnis eingabefeld aktiv , weil wir jetzt hier die sekundär grössen berechnen müssen
// V1.10 30.01.2012 bereich handling komplett überarbeitet incl. schutzfunktion bei spannungen > 120% * 480V
// es wurde die hauptstate machine wm3000 dahingehend geändert, dass keine gleichartigen events kumulieren. etwaige performanceprobleme (die beim xscale tatsächlich vorhanden sind) fallen damit nicht mehr auf
// die ansi fehlerberechnung wurde korrigiert
// die max. anzahl messperiode für 256 samples musste auf 14 perioden reduziert werden (speichermangel im dsp)
// information ob justiert oder nicht in statuszeile, in xml bei ergebnisexport, in status system questionable bit 11
// V1.11 26.06.2012 winkel auflösung in xml datei auf 5 stellen erhöht
// V1.12 06.09.2012 bei der istwertabfrage über scpi lastpunkte auf kanal x umgestellt
// V1.13 17.09.2012 kontextmenu für istwertanzeige um mrad erweitert, bei start des programms immer bereichautomatik ein,
//                  bei ergebnisdatei, eigenfehlerdatei solte open anstatt save beim öffnen verwendet werden, nicht machbar unter qt3 wenn man default selection verwenden möchte.
// V1.14 20.09.2012 in eparameter scalefactors eingeführt sonst fehler wenn keine einheit und /3 oder /w3 button
// V1.15 19.11.2012 dsp loader file location changed
// V2.15 14.12.2012 version number changed for difference old/new device
// V2.16 11.04.2013 en61850-9-2 decoder: ethernetframe sendet 8 samples/frame wenn 256samples/periode. 2 eingabefelder erforderlich
//                  für asdu nummern. first asdu, last asdu. es wird versucht von der 1. bis zur letzten asdu alles zuzuordnen.
//                  sync. überwachung läuft auch mit. es ist dsp version ab 3.09 erforderlich für diese funktion.
// v2.17 11.12.2013 nach jeder "umkonfiguration" wird der fehlermesswertund der phasenwinkel
//                  "inaktiv" und erst wieder aktiviert wenn ein neuer messwert da ist
//                  wenn |samplefrequenz - signalfrequenz| > 1.0 dann hinweis in statuszeile
//                  nicht justiert rot in statuszeile
//                  jede änderung in der konfiguration oder der anzeigefenster wird sofort
//                  gespeichert, damit die letzte konf. auch nach abschalten erhalten wird
// v2.18 19.12.2013 programm erweitert für wm1000u (nur conventional und ect)
// v2.19 28.05.2014 fehler in eigenfehlerkorrektur beseitigt. die einträge in der tabelle wurden falsch soriert
//                  wodurch die interpolation im anschluss falsche werte lieferte
// v2.20 13.06.2014 fehler in scpi interface beseitigt, es wurde die signalfrequenz nicht umgeschaltet.
//                  im confmenu source und destination adressen für nconvent umbenannt
//                  selbsttest inaktiv gesetzt wenn simulator betrieb
//                  pfad angaben für log files geändert.
//                  nach selbsttest erscheint eine messagebox mit info dass Test beendet
//                  releaseinfo file path hinzugefügt für releaseausgabe
// v2.21 ??.??.???? einfach so
// v2.22 25.06.2015 eparameter test so geändert, dass fehler in der angehängten einheit bemerkt werden.
// v2.23 31.08.2016 polinische Sprache hinzugefügt, statusbar permanent
// v2.24 02.03.2017 dc messmöglichkeit und offset justage eingebaut
//                  konfiguration signal auswahl ac/dc
//                  fenster für vektoren und fehlerwerte passen sich in abhängigkeit ac/dc an
//                  permanente dc justage in justage menu
//                  temporäre dc justage in messung menu
//                  rms werte in dsp ... es werden die dc werte abgezogen wenn ac betrieb
// v2.25 07.11.2017 fehlerwerte anzeige korrigiert dass nach startup die messwerte korrekt dargestellt werden.
//                  kontextmenu für vector bzw. fehleranzeige in abhängigkeit von dc bzw. ac angepasst
// v2.26 13.12.2017 offset messung und visualisierung für kanal n und x eingebaut. interface so erweitert dass
//                  diese messungen von extern gestartet werden können und die offsetwerte abgefragt werden können.
// v2.27 14.02.2018 bugfix : die wm3000 hängt sich auf wenn via interface ein zu großer wert messperioden gesendet wird.
//                  das konnte passieren wenn die samplerate von 80 auf 256 gesetzt wurde. im gui wurde das automatisch
//                  korrigiert. jetzt wird zentral am gerät korrigiert und und gui und interface informiert.
// v2.28 24.04.2018 bugfix : wenn das sampling system gesetzt wird, z.B. nach *rst, kann es sein dass der dsp busy meldet.
//                  die wm3000 blieb bis jetzt dann mit fehlermeldung unexpected answer stehen . busy wird jetzt recovert.
//                  einbau berechnung rcf ratio correction factor. und grossanzeige um rcf erweitert. interface (measure)
//                  erweitert. kontext menu ergänzt für die nachkommastellenzahl von rcf zu editieren.
// v2.29 11.07.2018 bei der frequenzberechnung den sollwert im falle von dc betrieb fest auf 0 gesetzt. hier muss später
//                  hubertus nochmal ran, weil wir sonst im falle eines signales mit f > 0 trotzdem 0 anzeigen.
// v2.30 31.07.2018 hardware misst jetzt frequenz bis runter zu 5.96 hz. dies ist jetzt richtig berücksichtigt.
// v2.31 21.11.2018 dc messwerte werden jetzt mit vorzeichen im interface gesendet. der ac button im konfigurations menu ist
//                  disabled wenn wm3000 nicht mit dc option gestartet wurde. es wurde eine funktion eingebaut um die justage
//                  daten version abzufragen. hintergrund ist dass geräte die noch einen alten justage daten stand haben
//                  (also vor dc) trotzdem als justiert gelten solange die software ohne dc option gestartet wird.
//                  es wurden neue übersetzungs files für englisch und polnisch hinzugefügt.
//                  die ad-wandler werden beim phasenabgleich nicht mehr berücksichtigt.
//
// v2.32 10.10.2019 einige korrekturen im zusammenhang mit dc messung
//                  neue kommandos für first asdu und last asdu
// v2.33 05.11.2019 bugfix : der automatismus aus dem bildschirmdialog  wurde übernommen dass wenn die anzahl
//                  der samples gesetzt wird ... dann ebenfalls die asdu's mit gesetzt werden
//                  *tst? antwortet jetzt richtig mit +0 oder +1
//                  *opc? snchronisiert auf fertig werden der konfiguration
// v2.34 29.11.2019 Versionsnummer geändert, um den Update-Prozess nicht zu gefährden
// v2.35 09.12.2019 bugfix : rücksetzen noOperationCompleteFlag wenn conf. ready
// v2.36 01.02.2021 phasenjustage adc's wieder eingebaut und status gain, phase, offset vereinzelt.
//                  die "alten" justagedaten und die zeiger darauf an den bereichen entfernt.
// v2.37 15.04.2021 es wird nach pps nur noch 1x über die eingestellte perioden anzahl gemessen.
//                  hintergrund : es lässt sich so das rauschen des eingangssignals beurteilen.
//                  die minimale anzahl perioden ist jetzt 1 anstatt 4.
//                  die integrationszeit (sek.) ist jetzt als movingwindow filter ausgeführt.
//                  das interface läuft jetzt stabil bei voller geschwindigkeit.
//                  opc? liefert fertig erst nachdem alle conf. kommandos abgearbeitet wurden.
//                  eth kommando, query hinzugefügt um en61850:sync:strong setzen und abfragen zu können
// v2.38 19.05.2021 es sollen die neuen sample rate für die non conventional wandler integriert werden
//                  im groben: das konfigurations menu muss angepasst werden. es gibt verriegelungen
//                  zwischen samplerate und signalfrequenz. die anzahl der signalperioden ist samplerate
//                  abhängig. die anzahl der asdu's in einer apdu ist ebenfalls davon abhängig. das scpi
//                  interface musste angepasst werden. der automatische phasenabgleich war entsprechend
//                  zu erweitern. die neue version läuft nur mit dem leiterkarten server ab v2.09.
// V2.39 8.9.2023   Tons of changes documented in github
// V2.40 28.11.2023 cmpactvalues added under test
// V2.41 31.1.2024  changes cofdialogbase to delete dc correction on non dc WMs
// V2.42            Actual Values take 100 calculate last 20
// V2.43 12.6.24    Adds overload to special class cwmmessagebox
// V2.44 29.7.24    Bugfix setting samplerates from interface
// 2.45 21.8.24     adds bugfix on scpi interface
// 2.47 29.8.24     adds dev:iface to export interface
// 2.48 16.9.24     adds onscreenkeyboard
// 2.49 19.11.24    adds disable Watchdogtimer
// 2.50 17.2.25     adds releaseinfo to version widget
// 2.51 7.10.25     improves translations
// 2.52 12.8.25     Ratio Widget
// 2.53 5.12.25     bugfix on ratio scpi interface
// 2.54 18.2.26     resize vector widget and bugfixes on scpi and Transformer Widget
// 2.55 24.2.26     bugfix RegExp RatioValidator

#define WMVersion "V2.55"

// #define ServerCommLogFilePath "/usr/share/wm3000u/log/ServerComm.log"
#define ServerCommLogFilePath QDir::homePath()+"/wm3000u/log/ServerComm.log"
//#define SelftestLogFilePath "/usr/share/wm3000u/log/Selftest.log"
#define SelftestLogFilePath QDir::homePath()+"/wm3000u/log/Selftest.log"
//#define PhaseJustLogFilePath "/usr/share/wm3000u/log/PhaseJust.log"
#define PhaseJustLogFilePath QDir::homePath()+"/wm3000u/log/PhaseJust.log"
#define OffsetJustLogFilePath QDir::homePath()+"/wm3000u/log/OffsetJust.log"
#define OffsetJustDataFilePath QDir::homePath()+"/wm3000u/offsetdata"
#define NSAOffsetJustDataFilePath QDir::homePath()+"/wm3000u/.offsetinfo"

enum MeasMode {Un_UxAbs,Un_EVT,Un_nConvent,maxMMode}; // messmodi der wm3000u#

class cJustMeasInfo : public cJustMeasInfoBase , public cJustMeasInfoBase2nd
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
