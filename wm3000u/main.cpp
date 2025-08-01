// WM3000U Hauptprogramm

#include <qapplication.h>
#include <qobject.h>
#include <qtranslator.h> 
#include <qdir.h>
#include <qfontdatabase.h>
#include <qstring.h>
#include <qevent.h>
#include <qnamespace.h>
#include <qmenubar.h>
#include <QDesktopWidget>

#include "wmscreenshoterguibase.h"
#include "zerainfo.h"
#include "confdialogbase.h"
#include "dbushelper.h"
#include "rangedialogbase.h"
#include "wmmeasvaluesbase.h"
#include "wmoeviewbase.h"
#include "wmrawactualvalbase.h"
#include "wmviewbaseu.h"
#include "logfileview.h"
#include "wmglobal.h"
#include "wm3000u.h"
#include "en61850monitor.h"
#include "devserver.h"
#include "wm3000scpiface.h"
#include "versionviewbase.h"
#include "wmmeasconfigbase.h"
#include "releaseinfo.h"
#include "voltageunits.h"
#include "loadpointunits.h"
#include "wmuoffsetcustomlabels.h"
#include <wmoffsetvalbase.h>
#include <commandlineparameter.h>

wmKeyboardForm* g_KeyBoard;
cWM3000U* g_WMDevice;
WMViewBaseU *g_WMView;
wmscreenshoterguibase* g_WMScrShoGui;
screenshooter* g_WMScreenShooter;

int main(int argc, char *argv[])
{
    QApplication::setDesktopSettingsAware(true);
    QApplication app(argc, argv);
    QFont f = app.font();
    qDebug("default font %s pitch %d\n",f.family().latin1(),f.pointSize());
#ifdef  FVWM   
    QFont f2("Sans-Serif",14);
#else
    QFont f2("Sans-Serif",10);
#endif    
    app.setFont(f2);

    CommandLineParameter mCmdLPar;
    mCmdLPar.Parse(argc, argv);
    mCmdLPar.setFileName("/etc/WMConfFile");
    mCmdLPar.ParseFile();

    g_WMDevice = new cWM3000U; //  die eigentliche Messeinrichtung wird später dynamisch je nach aufruf erzeugt
    g_WMDevice->setIpAddress(mCmdLPar.GetIpAdress());

    QString qmPath = "/usr/share/wm3000u";
    QString qmFileCom, qmFileWm;
    mCmdLPar.setDevice("U");

    switch (g_WMDevice->m_ConfData.Language)
    {
    case de:
        qmFileWm = "";
        qmFileCom = "wm-common_de.qm";
        mCmdLPar.setLanguage("de");
        break;
    case gb:
        qmFileWm = "wm3000u_gb.qm";
        qmFileCom = "wm-common_gb.qm";
        mCmdLPar.setLanguage("gb");
        break;
    case pl:
        qmFileWm = "wm3000u_pl.qm";
        qmFileCom = "wm-common_pl.qm";
        mCmdLPar.setLanguage("pl");
        break;
    }

    if (!qmFileWm.isEmpty())
    {
        QTranslator* appTranslator = new QTranslator(&app);
        if (!appTranslator->load(qmFileWm,qmPath))
            qWarning() << qPrintable(qmFileWm) << " translation file not found";
        else
            app.installTranslator(appTranslator);
    }
    if (!qmFileCom.isEmpty())
    {
        QTranslator* qtTranslator = new QTranslator(&app);
        if (!qtTranslator->load(qmFileCom,qmPath))
            qWarning() << qPrintable(qmFileCom) << " translation file not found ";
        else
            app.installTranslator(qtTranslator);
    }

    g_WMView = new WMViewBaseU(); // erst mal hauptfenster erzeugen
    app.setMainWidget(g_WMView); // hauptfenster der applikation mitteilen

    g_WMDevice->setPPSWatchDog(mCmdLPar.GetPpsWatchDog());
    if (!mCmdLPar.GetJustage()) {
        g_WMView->removeJustageItem();
        g_WMDevice->setJustage();
    }

    g_WMDevice->setConventional(mCmdLPar.GetConvent());
    if (mCmdLPar.GetConvent())
        g_WMView->configureWM1000Items();

    g_WMDevice->setDC(mCmdLPar.GetDC());
    if (!mCmdLPar.GetDC())
        g_WMView->configureWMwoDC();

    g_WMDevice->setNewSamplerates(mCmdLPar.GetNewSampleRates());

    if(mCmdLPar.GetScreenShoter()){
        g_WMScrShoGui = new wmscreenshoterguibase(g_WMView);
        g_WMScreenShooter = new screenshooter;
        g_WMScrShoGui->show();
        g_WMScreenShooter->setFontSize(f.pointSize());
        g_WMScreenShooter->setConvent(mCmdLPar.GetConvent());
        g_WMScreenShooter->setJustage(mCmdLPar.GetJustage());
    }

    g_WMView->setWMWindowTitle();
    QString machineName = "wm3000u";

    cReleaseInfo releaseInfo;
    QObject::connect(g_WMView, SIGNAL(UIhilfeReleaseInfoActionActivated()), &releaseInfo, SLOT(show()));

    cZeraInfo *g_WMInfo;
    if(g_WMDevice->isConventional())
        g_WMInfo = new cZeraInfo(wmInfoWm1000U);
    else
        g_WMInfo = new cZeraInfo(wmInfoWm3000U);

    g_KeyBoard = new wmKeyboardForm();  // onScreenkeyboard
    g_KeyBoard->setAvailGeometry(app.desktop()->availableGeometry(g_WMView));

    QList<eUnit *>lpUnitList;
    lpUnitList.append(LoadpointUnit + LPProzent);
    lpUnitList.append(VoltageUnit + VoltV);
    lpUnitList.append(VoltageUnit + VoltkV);
    WMMeasValuesBase *g_WMErrMeasValView = new WMMeasValuesBase(g_WMView, machineName, lpUnitList); // fehlermesswerte anzeige erzeugen
    QObject::connect(g_WMView,SIGNAL(UIansichtFehlerMessungActionToggled(bool)),g_WMErrMeasValView,SLOT(onShowHide(bool))); // öffnen der fehlermesswert anzeige
    QObject::connect(g_WMErrMeasValView,SIGNAL(sigIsVisible(bool)),g_WMView,SIGNAL(UIansichtFehlerMessungActionSet(bool))); //schliessen der fehlermesswert anzeige
    QObject::connect(g_WMView,SIGNAL(UIansichtScopeViewToggled(bool)),g_WMDevice,SLOT(wmScopeViewEnabled(bool)));
    QObject::connect(g_WMView,SIGNAL(onSaveSessionSignal(QString)),g_WMErrMeasValView,SLOT(onSaveSession(QString))); // fenster grösse und position einrichten
    QObject::connect(g_WMView,SIGNAL(onLoadSessionSignal(QString)),g_WMErrMeasValView,SLOT(onLoadSession(QString))); // fenster grösse und position einrichten

    QObject::connect(g_WMDevice,SIGNAL(SendActValuesSignal(cwmActValues*)),g_WMErrMeasValView,SLOT(SetActualValuesSlot( cwmActValues*))); // messwerte an fehlermesswert anzeige senden
    QObject::connect(g_WMDevice,SIGNAL(SendLPSignal(cwmActValues*)),g_WMErrMeasValView,SLOT(ActualizeLPSlot( cwmActValues*))); // neuen lastpunkt an fehlermesswert anzeige senden
    QObject::connect(g_WMDevice,SIGNAL(SendConfDataSignal(cConfData*)),g_WMErrMeasValView,SLOT(SetConfInfoSlot( cConfData*))); // konfiguration an fehlermesswert anzeige senden
    QObject::connect(g_WMView,SIGNAL(StoreResultSignal()),g_WMDevice,SLOT(StoreResultsSlot())); // ergebnisse in xml file speichern
    QObject::connect(g_WMView,SIGNAL(StartRangeObsermatic()),g_WMDevice,SLOT(RangeObsermaticSlot())); // für testzwecke bereichautomatik von hand triggern
    WMOeViewBase *g_WMOeView = new WMOeViewBase(g_WMView, machineName); // eigenfehleranzeige erzeugen
    QObject::connect(g_WMView,SIGNAL(UIansichtEigenfehlerActionToggled(bool)),g_WMOeView,SLOT(onShowHide(bool))); // öffnen der eigenfehleranzeige
    QObject::connect(g_WMOeView,SIGNAL(sigIsVisible(bool)),g_WMView,SIGNAL(UIansichtEigenfehlerActionSet(bool))); //schliessen der eigenfehleranzeige
    QObject::connect(g_WMView,SIGNAL(onSaveSessionSignal(QString)),g_WMOeView,SLOT(onSaveSession(QString))); // fenster grösse und position einrichten
    QObject::connect(g_WMView,SIGNAL(onLoadSessionSignal(QString)),g_WMOeView,SLOT(onLoadSession(QString))); // fenster grösse und position einrichten
    QObject::connect(&g_WMDevice->m_ownError,SIGNAL(SendOEViewData(cOwnErrorViewData*)),g_WMOeView,SLOT(ReceiveOEViewDataSlot(cOwnErrorViewData*)));

    WMRawActualValBase *g_WMActValView = new WMRawActualValBase(g_WMView, machineName);   //  istwertanzeige erzeugen
    QObject::connect(g_WMView,SIGNAL(UIansichtIstwerteActionToggled(bool)),g_WMActValView,SLOT(onShowHide(bool))); // öffnen der istwertanzeige
    QObject::connect(g_WMActValView,SIGNAL(sigIsVisible(bool)),g_WMView,SIGNAL(UIansichtIstwerteActionSet(bool))); //schliessen der istwertanzeige
    QObject::connect(g_WMView,SIGNAL(onSaveSessionSignal(QString)),g_WMActValView,SLOT(onSaveSession(QString))); // fenster grösse und position einrichten
    QObject::connect(g_WMView,SIGNAL(onLoadSessionSignal(QString)),g_WMActValView,SLOT(onLoadSession(QString))); // fenster grösse und position einrichten
    QObject::connect(g_WMDevice,SIGNAL(SendConfDataSignal(cConfData*)),g_WMActValView,SLOT(SetConfInfoSlot(cConfData*))); // device sendet konfigurationsdaten an rawactualanzei
    QObject::connect(g_WMDevice,SIGNAL(SendActValuesSignal(cwmActValues*)),g_WMActValView,SLOT(ReceiveAVDataSlot( cwmActValues*))); // senden von istwerten

    WMOffsetValBase *g_WMOffsetView = new WMOffsetValBase(machineName, new WmuOffsetCustomLabels, g_WMView);
    QObject::connect(g_WMView,SIGNAL(UIansichtOffsetActionToggled(bool)),g_WMOffsetView,SLOT(onShowHide(bool)));
    QObject::connect(g_WMOffsetView,SIGNAL(sigIsVisible(bool)),g_WMView,SIGNAL(UIansichtOffsetActionSet(bool))); //schliessen der offsetanzeige
    QObject::connect(g_WMView,SIGNAL(onSaveSessionSignal(QString)),g_WMOffsetView,SLOT(onSaveSession(QString))); // fenster grösse und position einrichten
    QObject::connect(g_WMView,SIGNAL(onLoadSessionSignal(QString)),g_WMOffsetView,SLOT(onLoadSession(QString))); // fenster grösse und position einrichten
    QObject::connect(g_WMDevice,SIGNAL(SendJustValuesSignal(tJustValues*)),g_WMOffsetView,SLOT(ReceiveJustDataSlot(tJustValues*))); // device sendet konfigurationsdaten an rawactualanzei

    CLogFileView* g_WMSCPILogFileView;
    if (mCmdLPar.GetConvent())
        g_WMSCPILogFileView =
                new CLogFileView(QObject::tr("WM1000U SCPI Kommunikation"), 100, g_WMView, "WMSCPILogView", machineName);
    else
        g_WMSCPILogFileView =
                new CLogFileView(QObject::tr("WM3000U SCPI Kommunikation"), 100, g_WMView, "WMSCPILogView", machineName);

    QObject::connect(g_WMView,SIGNAL(UIansichtDialogActionToggled(bool)),g_WMSCPILogFileView,SLOT(onShowHide(bool))); // öffnen der kommunikation anzeige
    QObject::connect(g_WMView,SIGNAL(onSaveSessionSignal(QString)),g_WMSCPILogFileView,SLOT(onSaveSession(QString))); // fenster grösse und position einrichten
    QObject::connect(g_WMView,SIGNAL(onLoadSessionSignal(QString)),g_WMSCPILogFileView,SLOT(onLoadSession(QString))); // fenster grösse und position einrichten
    QObject::connect(g_WMSCPILogFileView,SIGNAL(sigIsVisible(bool)),g_WMView,SIGNAL(UIansichtDialogActionSet(bool))); //schliessen der kommunikation anzeige

    EN61850monitor *g_ETHMonitor = new EN61850monitor(g_WMView, machineName);
    QObject::connect(g_WMView,SIGNAL(UIansichtEN61850ActionToggled(bool)),g_ETHMonitor,SLOT(onShowHide(bool))); // öffnen der eth status anzeige
    QObject::connect(g_ETHMonitor,SIGNAL(sigIsVisible(bool)),g_WMView,SIGNAL(UIansichtEN61850ActionSet(bool))); //schliessen der eth status anzeige
    QObject::connect((QObject*)g_WMDevice,SIGNAL(EN61850StatusSignal(cEN61850Info*)),g_ETHMonitor,SLOT(onETHStatus( cEN61850Info*))); // setzen der eth status info
    QObject::connect(g_ETHMonitor,SIGNAL(sigRequestInformation()),(QObject*)g_WMDevice,SLOT(EN61850InfoRequestSlot())); // anforderung der eth status info
    QObject::connect(g_ETHMonitor,SIGNAL(sigResetETHStatus()),(QObject*)g_WMDevice,SLOT(EN61850ResetStatusSlot())); // rücksetzen eth status info

    CLogFile LogFile(QDir(ServerCommLogFilePath).absPath(), g_WMDevice->m_ConfData.m_nLogFileMax); // kommunikation logfile erzeugen
    QObject::connect(&LogFile, SIGNAL(SendLogDataSignal(const QString&)), g_WMSCPILogFileView, SLOT(onAddLogText(const QString&))); // logfile sendet logdaten an logfileview
    QObject::connect(g_WMDevice->PCBIFace->iFaceSock, SIGNAL(SendLogData(const QString&)), &LogFile, SLOT(onAddLogText(const QString&))); // der socket sendet output/input an das logfile
    QObject::connect(g_WMDevice->DspIFace->iFaceSock, SIGNAL(SendLogData(const QString&)), &LogFile, SLOT(onAddLogText(const QString&))); // der auch
    LogFile.SendLogSlot(); // alte log daten an view

    ConfDialogBase *g_WMConfDialog = new ConfDialogBase(g_WMView); // confdialog erzeugen
    g_WMConfDialog->setKeyboard(g_KeyBoard);
    QObject::connect(g_WMView,SIGNAL(UIeinstellungenConfActionActivated()),g_WMConfDialog,SLOT(show())); // öffnen der konfigurations dialoges vom hauptfenster
    QObject::connect(g_WMConfDialog,SIGNAL(SendConfDataSignal(cConfData*)),g_WMDevice,SLOT(SetConfDataSlot(cConfData*))); // confdialog sendet konfigurationsdaten an device
    QObject::connect(g_WMDevice,SIGNAL(SendConfDataSignal(cConfData*)),g_WMConfDialog,SLOT(SetConfInfoSlot(cConfData*))); // device sendet konfigurationsdaten an confdialog
    QObject::connect(g_WMDevice,SIGNAL(SendConfDialogInfoSignal(QStringList&,QStringList&)),g_WMConfDialog,SLOT(SetConfListSlot( QStringList&, QStringList&))); // device sendet wandler primär/sekundär stufen und nennwerte

    RangeDialogBase *g_WMRangeDialog = new RangeDialogBase(g_WMView);   // bereichauswahlmenu erzeugen
    QObject::connect(g_WMView,SIGNAL(UIeinstellungenBereichActionActivated()),g_WMRangeDialog,SLOT(show()));  // öffnen des bereichauswahl dialoges vom hauptfenster
    QObject::connect(g_WMDevice,SIGNAL(SendRangeListSignal( cWMRangeList&, cWMRangeList&)),g_WMRangeDialog,SLOT(SetRangeListSlot( cWMRangeList&, cWMRangeList&))); // device sendet bereich informationen an rangedialog
    QObject::connect(g_WMDevice,SIGNAL(SendConfDataSignal(cConfData*)),g_WMRangeDialog,SLOT(SetConfInfoSlot(cConfData*))); // device sendet konfiguration an rangedialog
    QObject::connect(g_WMRangeDialog,SIGNAL(SendRange(cConfData*)),g_WMDevice,SLOT(SetRangeSlot(cConfData*))); // rangedialog sendet neue bereiche an device


    QObject::connect(g_WMView,SIGNAL(UIJustageAmplitudeActionActivated()),g_WMDevice,SLOT(JustageAmplitudeSlot())); // welchsel in den amplituden justage modus wenn jumper
    QObject::connect(g_WMView,SIGNAL(UIJustagePhaseActionActivated()),g_WMDevice,SLOT(JustagePhaseSlot())); // automatischer phasenabgleich wenn jumper
    QObject::connect(g_WMView,SIGNAL(UIJustageKoeffBerechnungActionActivated()),g_WMDevice,SLOT(JustagePhaseBerechnungSlot())); // automatischer phasenabgleich wenn jumper
    QObject::connect(g_WMView,SIGNAL(UIJustageOffsetActionActivated()),g_WMDevice,SLOT(JustageOffsetSlot())); // automatischer phasenabgleich wenn jumper
    QObject::connect(g_WMView,SIGNAL(UIJustageOffsetBerechnungActionActivated()),g_WMDevice,SLOT(JustageOffsetBerechnungSlot())); // automatischer phasenabgleich wenn jumper
    QObject::connect(g_WMView,SIGNAL(UIJustageOffsetVarActionActivated()),g_WMDevice,SLOT(JustageOffsetVarSlot())); // automatischer phasenabgleich wenn jumper

    QObject::connect(g_WMView,SIGNAL(UIMessungOffsetKanalNActivated()),g_WMDevice,SLOT(OffsetMessungChannelNSlot())); // ermitteln des offset von prüfling kanal n
    QObject::connect(g_WMView,SIGNAL(UIMessungOffsetKanalXActivated()),g_WMDevice,SLOT(OffsetMessungChannelXSlot()));

    QObject::connect((QObject*)g_WMView,SIGNAL(JustFlashProgSignal()),g_WMDevice,SLOT(JustageFlashProgSlot())); // welchsel in den amplituden justage modus wenn jumper
    QObject::connect((QObject*)g_WMView,SIGNAL(JustFlashExportSignal(QString)),g_WMDevice,SLOT(JustageFlashExportSlot(QString))); // automatischer phasenabgleich wenn jumper
    QObject::connect((QObject*)g_WMView,SIGNAL(JustFlashImportSignal(QString)),g_WMDevice,SLOT(JustageFlashImportSlot(QString))); // automatischer phasenabgleich wenn jumper

    QObject::connect(g_WMView,SIGNAL(UIhilfeInfo_ber_QtActionActivated()), &app, SLOT(aboutQt())); // informationen zu Qt
    QObject::connect(g_WMView,SIGNAL(UIhilfeInfo_ber_ZeraActionActivated()),g_WMInfo,SLOT(AboutZeraSlot())); // informationen zu Zera
    QObject::connect(g_WMView,SIGNAL(UIhilfeInfoActionActivated()),g_WMInfo,SLOT(AboutWM3000Slot())); // informationen zu WM3000
    QObject::connect(g_WMView,SIGNAL(UIhilfeSelbsttestActionActivated()),g_WMDevice,SLOT(SelfTestManuell())); // manuellen selbststest starten

    QObject::connect(g_WMDevice,SIGNAL(SendConfDataSignal(cConfData*)),g_WMView,SLOT(SetViewConfDataInfoSlot(cConfData*))); //  device sendet configurationsdaten an das hauptfenster
    QObject::connect(g_WMDevice,SIGNAL(JustifiedSignal(bool)),g_WMView,SLOT(SetJustifiedSlot(bool))); //  device sendet info ob justiert oder nicht an das hauptfenster
    QObject::connect(g_WMDevice,SIGNAL(FreqQuestionable(bool)),g_WMView,SLOT(SetFreqStatSlot(bool))); //  device sendet info ob freq ok oder nicht an das hauptfenster
    QObject::connect(g_WMDevice,SIGNAL(PPSQuestionable(bool)),g_WMView,SLOT(SetPPSStatSlot(bool)));
    QObject::connect(g_WMView,SIGNAL(SendConfDataSignal(cConfData*)),g_WMDevice,SLOT(SetConfDataSlot(cConfData*))); // hauptfenster sendet neue ergebnisdatei an device
    QObject::connect(g_WMView,SIGNAL(onSaveSessionSignal(QString)),g_WMDevice,SLOT(WriteSettings(QString))); // fenster grösse und position einrichten
    QObject::connect(g_WMView,SIGNAL(onLoadSessionSignal(QString)),g_WMDevice,SLOT(LoadSettings(QString))); // fenster grösse und position einrichten
    QObject::connect(g_WMView,SIGNAL(UIdateiBeendenActionActivated()), &app,SLOT(quit())); // beendet das programm
    QObject::connect(g_WMDevice,SIGNAL(AbortProgramSignal()), &app,SLOT(quit()));

    cwm3000DeviceServer* wm3000DeviceServer = new cwm3000DeviceServer(6315); // ein wm3000 device server auf port 6315
    QObject::connect(g_WMDevice,SIGNAL(SendConfDataSignal(cConfData*)),wm3000DeviceServer,SLOT(ReceiveDeviceConfiguration(cConfData*))); // wm3000 sendet konfiguration ans interface
    QObject::connect(wm3000DeviceServer,SIGNAL(SendDeviceConfiguration(cConfData*)),g_WMDevice,SLOT(SetConfDataSlot(cConfData*))); // interface sendet konfiguration an wm3000
    QObject::connect(wm3000DeviceServer,SIGNAL(RemoteCtrlInfo(bool)),g_WMView,SLOT(RemoteCtrlInfoSlot(bool))); // server sendet remote ctrl info an view
    QObject::connect(wm3000DeviceServer,SIGNAL(RemoteCtrlInfo(bool)),g_WMRangeDialog,SLOT(RemoteCtrlInfoSlot(bool))); // server sendet remote ctrl info an view
    QObject::connect(wm3000DeviceServer,SIGNAL(RemoteCtrlInfo(bool)),g_WMConfDialog,SLOT(RemoteCtrlInfoSlot(bool))); // server sendet remote ctrl info an view
    QObject::connect(wm3000DeviceServer,SIGNAL(startDeviceJustagePhase()),g_WMDevice,SLOT(JustagePhaseSlot()));
    QObject::connect(wm3000DeviceServer,SIGNAL(startDeviceJustageOffset()),g_WMDevice,SLOT(JustageOffsetSlot()));
    QObject::connect((QObject*)g_WMDevice,SIGNAL(EN61850StatusSignal(cEN61850Info*)),wm3000DeviceServer,SLOT(ReceiveETHStatus( cEN61850Info*))); // setzen der eth status info
    QObject::connect(wm3000DeviceServer,SIGNAL(RequestEN61850Info()),(QObject*)g_WMDevice,SLOT(EN61850InfoRequestSlot())); // anforderung der eth status info

    QObject::connect((QObject*)g_WMDevice,SIGNAL(SelftestDone(int)),wm3000DeviceServer,SLOT(ReceiveSelftestResult(int))); // setzen des selbsttest ergebnisses
    QObject::connect(wm3000DeviceServer,SIGNAL(RequestSelftest()),(QObject*)g_WMDevice,SLOT(SelfTestRemote())); // selbsttest (remote) starten

    QObject::connect((QObject*)g_WMDevice,SIGNAL(OffsetValue(double)),wm3000DeviceServer,SLOT(ReceiveOffsetResult(double))); // rückgabe des offset wertes an interface
    QObject::connect(wm3000DeviceServer,SIGNAL(RequestChannelNOffsetMeasure()),(QObject*)g_WMDevice,SLOT(OffsetMessungChannelNRemote())); // selbsttest (remote) starten
    QObject::connect(wm3000DeviceServer,SIGNAL(RequestChannelXOffsetMeasure()),(QObject*)g_WMDevice,SLOT(OffsetMessungChannelXRemote())); // selbsttest (remote) starten

    QObject::connect((QObject*)g_WMDevice,SIGNAL(AffectStatus(uchar, ushort)),wm3000DeviceServer,SLOT(ReceiveAffectStatus(uchar, ushort))); // setzen des selbsttest ergebnisses

    QObject::connect(wm3000DeviceServer,SIGNAL(sigResetETHStatus()),(QObject*)g_WMDevice,SLOT(EN61850ResetStatusSlot())); // rücksetzen eth status info
    QObject::connect(wm3000DeviceServer,SIGNAL(SendRangeCommand(cConfData*)),(QObject*)g_WMDevice,SLOT(SetRangeSlot(cConfData*))); // bereiche setzen
    QObject::connect((QObject*)g_WMDevice,SIGNAL(SendRangeListSignal(cWMRangeList&,cWMRangeList&)),wm3000DeviceServer,SLOT(SetRangeListSlot( cWMRangeList&,  cWMRangeList&))); // bereichlisten übertragen
    QObject::connect((QObject*)g_WMDevice,SIGNAL(SendVersionInfo(tVersSerial*)),wm3000DeviceServer,SLOT(ReceiveVersionInfo(tVersSerial*))); // version info übertragen
    QObject::connect((QObject*)g_WMDevice,SIGNAL(actualJustStateString(QString)),wm3000DeviceServer,SLOT(receiveActualJustStateString(QString)));

    QObject::connect(wm3000DeviceServer,SIGNAL(SendDefaultConfigCommand()),(QObject*)g_WMDevice,SLOT(DefaultSettingsMeasurementSlot())); // default konfig setzen
    QObject::connect((QObject*)g_WMDevice,SIGNAL(RangeAutomaticDone()),wm3000DeviceServer,SLOT(ReceiveRangeAutomaticDone())); // bereich automatic fertig
    QObject::connect((QObject*)g_WMDevice,SIGNAL(SendActValuesSignal(cwmActValues*)),wm3000DeviceServer,SLOT(ReceiveActualValues(cwmActValues*))); // messwerte übergabe

    QObject::connect((QObject*)g_WMDevice,SIGNAL(SendLPSignal(cwmActValues*)),wm3000DeviceServer,SLOT(ReceiveLPValue( cwmActValues*))); // neuen lastpunkt an fehlermesswert anzeige senden

    VersionsViewBase *g_VersionsView = new VersionsViewBase(g_WMView);
    QObject::connect(g_WMView,SIGNAL(UIhilfeVersionActionActivated()),g_VersionsView,SLOT(ShowVersionSlot())); // anzeige aller system versionen
    QObject::connect((QObject*)g_WMDevice,SIGNAL(SendVersionInfo(tVersSerial*)),g_VersionsView,SLOT(ReceiveVersionData(tVersSerial*))); // übergabe der system informationen
    g_VersionsView->setOptionStr(mCmdLPar.GetOptionString());

    g_WMDevice->InitWM3000();
    g_WMView->show();  // zeige das hauptfenster

    if(mCmdLPar.GetScreenShoter()){
        g_WMScreenShooter->setFolderName(mCmdLPar.GetOptionStringForFolders());
        g_WMView->setScreenShooter(g_WMScreenShooter);
        g_WMConfDialog->setScreenShooter(g_WMScreenShooter);
        g_WMErrMeasValView->setScreenShooter(g_WMScreenShooter);
        g_WMActValView->setScreenShooter(g_WMScreenShooter);
        g_WMOeView->setScreenShooter(g_WMScreenShooter);
        g_WMRangeDialog->setScreenShooter(g_WMScreenShooter);
        g_VersionsView->setScreenShooter(g_WMScreenShooter);
        if (g_WMInfo->isWM3000())
        {
            g_ETHMonitor->setScreenShooter(g_WMScreenShooter);
            QObject::connect(g_WMScreenShooter,SIGNAL(screenShotMessBerFinished()),g_ETHMonitor,SLOT(takeScreenshoots()));
            QObject::connect(g_WMScreenShooter,SIGNAL(screenShotEtherMonFinished()),g_ETHMonitor,SLOT(takeScreenshootFinished()));
            QObject::connect(g_WMScreenShooter,SIGNAL(screenShotEtherMonFinished()),g_WMScrShoGui,SLOT(show()));
        }
        else
            QObject::connect(g_WMScreenShooter,SIGNAL(screenShotMessBerFinished()),g_WMScrShoGui,SLOT(show()));

        QObject::connect(g_WMScrShoGui,SIGNAL(screenShooterStart()),g_WMView,SLOT(AutoScreenShoterTriggered()));
        QObject::connect(g_WMView,SIGNAL(ScreenshooterTriggeredByUser()),g_WMView,SLOT(takeScreenshoots()));
        QObject::connect(g_WMScreenShooter,SIGNAL(screenShotHauptFinished()),g_WMView,SLOT(takeScreenshootFinished()));

        QObject::connect(g_WMScreenShooter,SIGNAL(screenShotHauptFinished()),g_WMConfDialog,SLOT(screenshooterTriggered()));
        QObject::connect(g_WMScreenShooter,SIGNAL(keyboardScreenShotFinished()),g_WMErrMeasValView,SLOT(takeScreenshoots()));
        QObject::connect(g_WMScreenShooter,SIGNAL(screenShotFehlerFinished()),g_WMErrMeasValView,SLOT(takeScreenshootSetting()));
        QObject::connect(g_WMScreenShooter,SIGNAL(screenShotFehlerEinstellFinished()),g_WMErrMeasValView,SLOT(takeScreenshootSettingFinished()));

        QObject::connect(g_WMScreenShooter,SIGNAL(screenShotFehlerEinstellFinished()),g_WMActValView,SLOT(takeScreenshoots()));
        QObject::connect(g_WMScreenShooter,SIGNAL(screenShotVektorFinished()),g_WMActValView,SLOT(takeScreenshootSetting()));
        QObject::connect(g_WMScreenShooter,SIGNAL(screenShotVektorEinstellFinished()),g_WMActValView,SLOT(takeScreenshootSettingFinished()));

        QObject::connect(g_WMScreenShooter,SIGNAL(screenShotVektorEinstellFinished()),g_WMOeView,SLOT(takeScreenshoots()));
        QObject::connect(g_WMScreenShooter,SIGNAL(screenShotEigenFinished()),g_WMOeView,SLOT(takeScreenshootFinished()));

        QObject::connect(g_WMScreenShooter,SIGNAL(screenShotEigenFinished()),g_VersionsView,SLOT(takeScreenshoots()));
        QObject::connect(g_WMScreenShooter,SIGNAL(screenShotVersionFinished()),g_VersionsView,SLOT(takeScreenshootFinished()));

        QObject::connect(g_WMScreenShooter,SIGNAL(screenShotVersionFinished()),g_WMRangeDialog,SLOT(takeScreenshoots()));
        QObject::connect(g_WMScreenShooter,SIGNAL(screenShotMessBerFinished()),g_WMRangeDialog,SLOT(takeScreenshootFinished()));
    }


    //add dbus controls for visibility of windows
    WM3000UDBusHelper dbushelper;
    dbushelper.registerWidget(g_WMErrMeasValView);
    dbushelper.registerWidget(g_WMOeView);
    dbushelper.registerWidget(g_WMActValView);
    dbushelper.registerWidget(g_WMSCPILogFileView);
    dbushelper.registerWidget(g_ETHMonitor);
    dbushelper.registerWidget(g_WMConfDialog);
    dbushelper.registerWidget(g_WMRangeDialog);
    dbushelper.registerWidget(g_WMOffsetView);

    int ret = app.exec();

    delete g_WMInfo;
    delete wm3000DeviceServer;
    delete g_WMDevice;
    delete g_WMView;

    return(ret);
}   
