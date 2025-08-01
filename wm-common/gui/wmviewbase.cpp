#include "wmviewbase.h"
#include "qdebug.h"
#include "wmeditor.h"
#include "widgetgeometry.h"
#include "ui_wmviewbase.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QTextEdit>
#include <QTextStream>

WMViewBase::WMViewBase(WMViewBase *parent) :
    QMainWindow(parent),
    ui(new Ui::WMViewBase),
    m_statusLabelContainer(this)
{
    ui->setupUi((QMainWindow*) this);
}


WMViewBase::~WMViewBase()
{
    destroy();
    delete wmEdit;
    delete wmEdit2;
    delete ui;
}

void WMViewBase::AutoScreenShoterTriggered()
{
    QPoint point;
    uint adjust;
    point = ui->MenuBarEditor->rect().bottomLeft();
    ui->Datei->exec(mapToGlobal(point));
    adjust = mScrShooter->storeScreenShot(ui->Datei,"07_Datei");
    ui->Datei->exec(mapToGlobal(point));
    mScrShooter->storeScreenShotW(this,"08_Datei",adjust);

    ui->Messung->exec(mapToGlobal(point+ QPoint(65,0)));
    adjust = mScrShooter->storeScreenShot(ui->Messung,"11_Messung");
    ui->Messung->exec(mapToGlobal(point+ QPoint(65,0)));
    mScrShooter->storeScreenShotW(this,"12_Messung",adjust);

    ui->Ansicht->exec(mapToGlobal(point+ QPoint(145,0)));
    adjust = mScrShooter->storeScreenShot(ui->Ansicht,"13_Ansicht");
    ui->Ansicht->exec(mapToGlobal(point+ QPoint(145,0)));
    mScrShooter->storeScreenShotW(this,"14_Ansicht",adjust);

    ui->Einstellungen->exec(mapToGlobal(point+ QPoint(205,0)));
    adjust = mScrShooter->storeScreenShot(ui->Einstellungen,"15_Einstell");
    ui->Einstellungen->exec(mapToGlobal(point+ QPoint(205,0)));
    mScrShooter->storeScreenShotW(this,"16_Einstell",adjust);

    ui->Hilfe->exec(mapToGlobal(point+ QPoint(305,0)));
    adjust = mScrShooter->storeScreenShot(ui->Hilfe,"24_Hilfe");
    ui->Hilfe->exec(mapToGlobal(point+ QPoint(305,0)));
    mScrShooter->storeScreenShotW(this,"25_Hilfe",adjust);

    ui->Sprache->exec(mapToGlobal(point+ QPoint(355,0)));
    adjust = mScrShooter->storeScreenShot(ui->Sprache,"26_Sprache");
    ui->Sprache->exec(mapToGlobal(point+ QPoint(355,0)));
    mScrShooter->storeScreenShotW(this,"27_Sprache",adjust);

    if (mScrShooter->isJustage())
    {
        ui->Justage->exec(mapToGlobal(point+ QPoint(425,0)));
        adjust = mScrShooter->storeScreenShot(ui->Justage,"28_Justage");
        ui->Justage->exec(mapToGlobal(point+ QPoint(425,0)));
        mScrShooter->storeScreenShotW(this,"29_Justage",adjust);
    }
    emit ScreenshooterTriggeredByUser();
}

void WMViewBase::setScreenShooter(screenshooter* poi)
{
    mScrShooter = poi;
}

void WMViewBase::takeScreenshoots()
{
    mScrShooter->showed(false);
    if (!this->isShown())
    {
        this->show();
        mScrShooter->showed(true);
    }
    mScrShooter->useTimer(this, 2);
}

void WMViewBase::takeScreenshootFinished()
{
    if(mScrShooter->showed())
    {
        this->hide();
    }
}

void WMViewBase::removeJustageItem()
{
    ui->MenuBarEditor->removeAction(ui->Justage->menuAction());
    ui->ansichtScopeAction->removeFrom(ui->Ansicht);
}

void WMViewBase::configureWM1000Items()
{
    ui->Ansicht->removeAction(ui->ansichtEN61850Action);
    m_isWm1000 = true;
}

void WMViewBase::setWMWindowTitle()
{
    QString s;
    s = QObject::tr("Wandlermesseinrichtung %1").arg(getDeviceName().toUpper());
    if (m_isWm1000) {
        s = s.replace("3","1");
    }
    setWindowTitle(s);
}


void WMViewBase::configureWMwoDC()
{
    ui->Messung->removeAction(ui->messungOffsetabgleichWM3000Action);
    ui->Messung->removeAction(ui->messungOffsetKanalNAction);
    ui->Messung->removeAction(ui->messungOffsetKanalXAction);
    ui->Ansicht->removeAction(ui->ansichtOffsetAction);
}


void WMViewBase::init()
{
    m_bJustified = false;
    m_bFreqQuestionable = false;
    m_bPPSQuestionable = false;

    for (int i =0 ; i < nmaxRecentOEFiles; i++) m_nrecentOEFileIds[i] = -1;
    for (int i =0 ; i < nmaxRecentMVFiles; i++) m_nrecentMVFileIds[i] = -1;
    for (int i =0 ; i < nmaxRecentSESFiles; i++) m_nrecentSESFileIds[i] = -1;
    CreateStatusBar();
    wmEdit = new wmEditor();
    wmEdit2 = new wmEditor();
    connect(wmEdit,SIGNAL(textMessage(QString)),this,SLOT(ReceiveOETFileSlot(QString)));
    connect(wmEdit2,SIGNAL(textMessage(QString)),this,SLOT(ReceiveResultFileSlot(QString)));
    ui->ansichtFehlerMessungAction->setChecked(false);
    ui->ansichtIstwerteAction->setChecked(false);
    ui->ansichtOffsetAction->setChecked(false);
    ui->ansichtDialogAction->setChecked(false);
    ui->ansichtEigenfehlerAction->setChecked(false);
    ui->ansichtEN61850Action->setChecked(false);
    ui->ansichtScopeAction->setChecked(false);

    onLoadSession(".ses");
    connect(ui->ansichtScopeAction,SIGNAL(toggled(bool)),this,SIGNAL(UIansichtScopeViewToggled(bool)));
    connect(this,SIGNAL(UIansichtScopeViewSet(bool)),ui->ansichtScopeAction,SLOT(setChecked(bool)));

    connect(ui->ansichtFehlerMessungAction,SIGNAL(toggled(bool)),this,SIGNAL(UIansichtFehlerMessungActionToggled(bool))); // öffnen der fehlermesswert anzeige
    connect(ui->ansichtFehlerMessungAction,SIGNAL(toggled(bool)),this,SLOT(SaveDefaultSessionSlot(bool))); // wir speichern die laufende sitzung immer
    connect(this,SIGNAL(UIansichtFehlerMessungActionSet(bool)),ui->ansichtFehlerMessungAction,SLOT(setChecked(bool)));

    connect(ui->ansichtEigenfehlerAction,SIGNAL(toggled(bool)),this,SIGNAL(UIansichtEigenfehlerActionToggled(bool))); // öffnen der eigenfehler anzeige
    connect(ui->ansichtEigenfehlerAction,SIGNAL(toggled(bool)),this,SLOT(SaveDefaultSessionSlot(bool))); // öffnen der eigenfehler anzeige
    connect(this,SIGNAL(UIansichtEigenfehlerActionSet(bool)),ui->ansichtEigenfehlerAction,SLOT(setChecked(bool)));

    connect(ui->ansichtIstwerteAction,SIGNAL(toggled(bool)),this,SIGNAL(UIansichtIstwerteActionToggled(bool))); // öffnen der eigenfehler anzeige
    connect(ui->ansichtIstwerteAction,SIGNAL(toggled(bool)),this,SLOT(SaveDefaultSessionSlot(bool))); // öffnen der eigenfehler anzeige
    connect(this,SIGNAL(UIansichtIstwerteActionSet(bool)),ui->ansichtIstwerteAction,SLOT(setChecked(bool)));

    connect(ui->ansichtOffsetAction,SIGNAL(toggled(bool)),this,SIGNAL(UIansichtOffsetActionToggled(bool))); // öffnen der eigenfehler anzeige
    connect(ui->ansichtOffsetAction,SIGNAL(toggled(bool)),this,SLOT(SaveDefaultSessionSlot(bool))); // öffnen der eigenfehler anzeige
    connect(this,SIGNAL(UIansichtOffsetActionSet(bool)),ui->ansichtOffsetAction,SLOT(setChecked(bool)));

    connect(ui->ansichtDialogAction,SIGNAL(toggled(bool)),this,SIGNAL(UIansichtDialogActionToggled(bool))); // öffnen der eigenfehler anzeige
    connect(ui->ansichtDialogAction,SIGNAL(toggled(bool)),this,SLOT(SaveDefaultSessionSlot(bool))); // öffnen der eigenfehler anzeige
    connect(this,SIGNAL(UIansichtDialogActionSet(bool)),ui->ansichtDialogAction,SLOT(setChecked(bool)));

    connect(ui->ansichtEN61850Action,SIGNAL(toggled(bool)),this,SIGNAL(UIansichtEN61850ActionToggled(bool))); // öffnen der eigenfehler anzeige
    connect(ui->ansichtEN61850Action,SIGNAL(toggled(bool)),this,SLOT(SaveDefaultSessionSlot(bool))); // öffnen der eigenfehler anzeige
    connect(this,SIGNAL(UIansichtEN61850ActionSet(bool)),ui->ansichtEN61850Action,SLOT(setChecked(bool)));

    connect(ui->einstellungenConfAction,SIGNAL(activated()),this,SIGNAL(UIeinstellungenConfActionActivated()));
    connect(ui->einstellungenBereichAction,SIGNAL(activated()),this,SIGNAL(UIeinstellungenBereichActionActivated()));
    connect(ui->JustageAmplitudeAction,SIGNAL(activated()),this,SIGNAL(UIJustageAmplitudeActionActivated()));
    connect(ui->JustagePhaseAction,SIGNAL(activated()),this,SIGNAL(UIJustagePhaseActionActivated()));
    connect(ui->JustageKoeffBerechnungAction,SIGNAL(activated()),this,SIGNAL(UIJustageKoeffBerechnungActionActivated()));
    connect(ui->JustageOffsetAction,SIGNAL(activated()),this,SIGNAL(UIJustageOffsetActionActivated()));
    connect(ui->JustageOffsetBerechnungAction,SIGNAL(activated()),this,SIGNAL(UIJustageOffsetBerechnungActionActivated()));
    connect(ui->messungOffsetabgleichWM3000Action,SIGNAL(activated()),this,SIGNAL(UIJustageOffsetVarActionActivated()));
    connect(ui->messungOffsetKanalNAction,SIGNAL(activated()),this,SIGNAL(UIMessungOffsetKanalNActivated()));
    connect(ui->messungOffsetKanalXAction,SIGNAL(activated()),this,SIGNAL(UIMessungOffsetKanalXActivated()));
    connect(ui->hilfeInfo_ber_QtAction,SIGNAL(activated()),this,SIGNAL(UIhilfeInfo_ber_QtActionActivated()));
    connect(ui->hilfeInfo_ber_ZeraAction,SIGNAL(activated()),this,SIGNAL(UIhilfeInfo_ber_ZeraActionActivated()));
    connect(ui->hilfeInfoAction,SIGNAL(activated()),this,SIGNAL(UIhilfeInfoActionActivated()));
    connect(ui->hilfeSelbsttestAction,SIGNAL(activated()),this,SIGNAL(UIhilfeSelbsttestActionActivated()));
    connect(ui->dateiBeendenAction,SIGNAL(activated()),this,SIGNAL(UIdateiBeendenActionActivated()));
    connect(ui->hilfeVersionAction,SIGNAL(activated()),this,SIGNAL(UIhilfeVersionActionActivated()));
    connect(ui->hilfeReleaseInfoAction,SIGNAL(activated()),this,SIGNAL(UIhilfeReleaseInfoActionActivated()));
}


void WMViewBase::ActualizeStates()
{
    if (m_ConfData.m_bRunning)
    {
        ui->messungStartAction->setMenuText(tr("S&top"));
        ui->messungStartAction->setStatusTip(tr("Messung anhalten"));
    }
    else
    {
        ui->messungStartAction->setMenuText(tr("S&tart"));
        ui->messungStartAction->setStatusTip(tr("Messung starten"));
    }

    ui->messungSimulationAction->setOn(m_ConfData.m_bSimulation);
    ui->hilfeSelbsttestAction->setDisabled(m_ConfData.m_bSimulation);

    m_statusLabelContainer.updateLabels(&m_ConfData, m_bJustified, m_bFreqQuestionable, m_bPPSQuestionable);

    UpdateRecentFileList(recentOETFiles, m_ConfData.m_sOETFile);
    UpdateRecentFileList(recentResultFiles, m_ConfData.m_sResultFile);
}

void WMViewBase::destroy()
{
    onSaveSession(".ses");
}


void WMViewBase::SetViewConfDataInfoSlot( cConfData *cd )
{
    m_ConfData=*cd;
    ActualizeStates();
}


void WMViewBase::OpenOETFileSlot() // zum laden einer eigenfehlertabelle
{
    QFileDialog *OETFileDialog=new QFileDialog(this,tr("Eigenfehlertabellen"), QDir::homePath(),tr("Eigenfehlertabellen (*.oet)"));
    OETFileDialog->setMode( QFileDialog::AnyFile);
    //OETFileDialog->setSelection("./Transformer.oet");
    if ( OETFileDialog->exec() == QDialog::Accepted ) {
        QString OETFile = OETFileDialog->selectedFile();
        UpdateRecentFileList(recentOETFiles,OETFile);
        m_ConfData.m_sOETFile = OETFile;
        m_ConfData.m_bOENewLoad = true; // 1x analysieren
        emit SendConfDataSignal(&m_ConfData);
    }
    delete OETFileDialog;
}


void WMViewBase::UpdateRecentFileList(QStringList& sl, QString f)
{
    if (f != "") {
        sl.remove(f); // wenn es den eintrag schon gibt -> löschen
        sl.push_front(f); // dann neuen an den anfang der liste setzen
    }

    while ((int) recentOETFiles.size()>nmaxRecentOEFiles)
        recentOETFiles.pop_back();
    int n = (int) recentOETFiles.size();

    for (int i=0; i<n; i++)
    {
        QString text = QString ("&%1 %2").arg(i+1).arg(strippedName(recentOETFiles[i]));
        if (m_nrecentOEFileIds[i]==-1)
        {
            m_nrecentOEFileIds[i]=ui->Datei->insertItem(text,this,SLOT(OpenRecentOETFileSlot(int)),0,i,1+i);
            ui->Datei->setItemParameter(m_nrecentOEFileIds[i],i);
        }
        else
        {
            ui->Datei->changeItem(m_nrecentOEFileIds[i],text);
        }
    }

    while ((int) recentResultFiles.size()>nmaxRecentMVFiles)
        recentResultFiles.pop_back();

    for (int i=0; i< (int) recentResultFiles.size(); i++)
    {
        QString text = QString ("&%1 %2").arg(i+n+1).arg(strippedName(recentResultFiles[i]));
        if (m_nrecentMVFileIds[i]==-1)
        {
            m_nrecentMVFileIds[i]=ui->Datei->insertItem(text,this,SLOT(OpenRecentResultFileSlot(int)),0,nmaxRecentOEFiles+i,ui->Datei->count()-2); // wir nehmen eine id > nmaxRecentOEFiles -1 ---> damit eindeutigkeit herrscht
            ui->Datei->setItemParameter(m_nrecentMVFileIds[i],i);
        }
        else
        {
            ui->Datei->changeItem(m_nrecentMVFileIds[i],text);
        }
    }
}


QString WMViewBase::strippedName(QString s)
{
    QFileInfo fi(s);
    return(fi.fileName());
}


void WMViewBase::OpenRecentOETFileSlot(int index)
{
    QString s = recentOETFiles[index];
    UpdateRecentFileList(recentOETFiles,s); // liste event. kürzen und einträge ins menu
    m_ConfData.m_sOETFile = s;
    m_ConfData.m_bOENewLoad = true;
    emit SendConfDataSignal(&m_ConfData);
}


void WMViewBase::SimulationSlot()
{
    m_ConfData.m_bSimulation = !m_ConfData.m_bSimulation;
    emit SendConfDataSignal(&m_ConfData);
    ActualizeStates();
}


void WMViewBase::StartSlot()
{
    m_ConfData.m_bRunning = !m_ConfData.m_bRunning;
    emit SendConfDataSignal(&m_ConfData);
    ActualizeStates();
}


void WMViewBase::CreateStatusBar()
{
    m_statusLabelContainer.CreateStatusBar(statusBar(), this);
}


void WMViewBase::CloseOETFileSlot()
{
    m_ConfData.m_sOETFile = "";
    m_ConfData.m_bOENewLoad = true;
    emit SendConfDataSignal(&m_ConfData);
}


void WMViewBase::EditOETFileSlot()
{
    if (m_ConfData.m_sOETFile=="")  // wir haben noch keine eigenfehlertabelle geöffnet
        OpenOETFileSlot();
    if (m_ConfData.m_sOETFile=="") return; // nichts ausgewählt -> fertig

    wmEdit->setCaption(strippedName(m_ConfData.m_sOETFile));
    QFile file(m_ConfData.m_sOETFile); // text einlesen
    if ( file.open( IO_ReadWrite ) ) {
        QTextStream stream( &file );
        QString text = stream.read();
        if (text.isEmpty())
            text = "15A;15A;100%;0.01%;0.0grad";
        wmEdit->setPlainText( text );
        file.close();
        wmEdit->show();
    } // wenn der editor geschlossen wird -> signal mit neuem text (geht auf den slot eins tiefer)
}


void WMViewBase::ReceiveOETFileSlot(QString nText)
{
    wmEdit->hide();
    if (wmEdit->document()->isModified()) { // der text hat sich geändert
        m_ConfData.m_sOETFile = QFileDialog::getSaveFileName( this,
                                                             tr("Speichern unter"),
                                                             m_ConfData.m_sOETFile,
                                                               tr("Eigenfehlertabelle (*oet)")
                                                               );
        m_ConfData.m_bOENewLoad = true;
    }

    QFile file(m_ConfData.m_sOETFile);
    file.remove();
    if ( file.open( IO_WriteOnly ) ) {
        QTextStream stream( &file );
        stream << nText;
        file.close();
        emit SendConfDataSignal(&m_ConfData);
    }
}


void WMViewBase::OpenResultFileSlot()
{
    QFileDialog *ResultFileDialog=new QFileDialog ( this,tr("Ergebnisdateien"), ".",tr("Ergebnisdateien (*.xml)"));
    ResultFileDialog->setDir(QDir(QString("%1%2").arg(QDir::homePath()).arg("results")));
    ResultFileDialog->setMode( QFileDialog::AnyFile);
    //ResultFileDialog->setSelection("results.xml");
    if ( ResultFileDialog->exec() == QDialog::Accepted ) {
        QString rFile;
        rFile=ResultFileDialog->selectedFile();
        UpdateRecentFileList(recentResultFiles,rFile); // liste event. kürzen und einträge ins menu
        m_ConfData.m_sResultFile = rFile;
        emit SendConfDataSignal(&m_ConfData);
    }
    delete ResultFileDialog;
}


void WMViewBase::EditResultFileSlot()
{
    if (m_ConfData.m_sResultFile=="")  // wir haben noch keine ergebnisdatei geöffnet
        OpenResultFileSlot();
    if (m_ConfData.m_sResultFile=="") return; // nichts ausgewählt -> fertig

    wmEdit2->setCaption(strippedName(m_ConfData.m_sResultFile));
    QFile file(m_ConfData.m_sResultFile); // text einlesen
    if ( file.open( IO_ReadOnly ) ) {
        QTextStream stream( &file );
        wmEdit2->setPlainText( stream.read() );
        file.close();
        wmEdit2->show();
    } // wenn der editor geschlossen wird -> signal mit neuem text (geht auf den slot eins tiefer)
}


void WMViewBase::ReceiveResultFileSlot(QString nText)
{
    wmEdit2->hide();
    QFile file(m_ConfData.m_sResultFile);
    file.remove();
    if ( file.open( IO_WriteOnly ) ) {
        QTextStream stream( &file );
        stream << nText;
        file.close();
    }
}


void WMViewBase::CloseResultFileSlot()
{
    m_ConfData.m_sResultFile = "";
    emit SendConfDataSignal(&m_ConfData);
}


void WMViewBase::StoreResultSlot()
{
    if (m_ConfData.m_sResultFile=="") { // wir haben noch keine ergebnisdatei geöffnet
        QString s = QFileDialog::getSaveFileName(
                this,
                tr("Ergebnisdateien"),
                QString("%1/%2/%3").arg(QDir::homePath()).arg(getDeviceName()).arg("results/results.xml"),
                "Ergebnisdateien (*.xml)"
                );
        if (s == "") return; // nichts ausgewählt -> fertig
        m_ConfData.m_sResultFile = s;
        emit SendConfDataSignal(&m_ConfData);
    }
    emit StoreResultSignal(); // signal an wm3000 messergebnisse zu speichern
}


void WMViewBase::OpenRecentResultFileSlot(int index)
{
    QString s = recentResultFiles[index];
    UpdateRecentFileList(recentResultFiles,s); // liste event. kürzen und einträge ins menu
    m_ConfData.m_sResultFile = s;
    emit SendConfDataSignal(&m_ConfData);
}


bool WMViewBase::onLoadSession(QString session)
{
    QFileInfo fi(session);
    getDeviceName();
    QString ls = QString("%1/.%2/%3%4").arg(QDir::homePath()).arg(getDeviceName()).arg(name()).arg(fi.fileName());
    QFile file(ls);
    if ( file.open( IO_ReadOnly ) ) {
        QDataStream stream( &file );
        int mA, iA, oA, dA, eA, enA;
        stream >> mA >> iA >> oA >> dA >> eA >> enA;
        stream >> m_widGeometry;
        file.close();

        ui->ansichtFehlerMessungAction->setChecked(mA);
        ui->ansichtIstwerteAction->setChecked(iA);
        ui->ansichtOffsetAction->setChecked(oA);
        ui->ansichtDialogAction->setChecked(dA);
        ui->ansichtEigenfehlerAction->setChecked(eA);
        ui->ansichtEN61850Action->setChecked(enA);

        hide();
        resize(m_widGeometry.getSize());
        move(m_widGeometry.getPoint());
        show();
        // FVWM und Gnome verhalten sich anders
#ifndef FVWM
        move(m_widGeometry.getPoint());
#endif

        return true;
    }
    return false;
}


void WMViewBase::onSaveSession(QString session)
{
    if(!QDir(QString("%1/.%2/").arg(QDir::homePath()).arg(getDeviceName())).exists())
    {
        //create temporary object that gets deleted when leaving the control block
        QDir().mkdir(QString("%1/.%2/").arg(QDir::homePath()).arg(getDeviceName()));
    }
    QFileInfo fi(session);
    QString ls = QString("%1/.%2/%3%4").arg(QDir::homePath()).arg(getDeviceName()).arg(name()).arg(fi.fileName());
    QFile file(ls);
    //    file.remove();
    if ( file.open( QIODevice::Unbuffered | QIODevice::WriteOnly ) ) {
        file.at(0);

        int vi;

        vi = (isVisible()) ? 1 : 0;
        if (vi) {
            m_widGeometry.setPoint(pos());
            m_widGeometry.setSize(size());
        }
        m_widGeometry.setVisible(vi);

        QDataStream stream( &file );
        stream << (int)ui->ansichtFehlerMessungAction->isChecked()
               << (int)ui->ansichtIstwerteAction->isChecked()
               << (int)ui->ansichtOffsetAction->isChecked()
               << (int)ui->ansichtDialogAction->isChecked()
               << (int)ui->ansichtEigenfehlerAction->isChecked()
               << (int)ui->ansichtEN61850Action->isChecked();

        stream << m_widGeometry;
        file.close();
    }
}

void WMViewBase::StoreSessionSlot()
{
    SessionName = QFileDialog::getSaveFileName( this,
                                               tr("Sitzung speichern"),
                                               QString("%1/.%2/%3").arg(QDir::homePath()).arg(getDeviceName()).arg("/Session.ses"),
                                                 tr("Sitzung Name (*.ses)")
                                                );


    QFile file(SessionName);
    if ( file.open( IO_WriteOnly ) ) {
        file.close();
        onSaveSession(SessionName); // eigene session speichern
        emit onSaveSessionSignal(SessionName); // die anderen
    }
}


void WMViewBase::onLoadSessionSlot()
{
    QFileDialog *SessionFileDialog=new QFileDialog ( this,
                                                     tr("Sitzung laden"),
                                                     QString("%1/.%2").arg(QDir::homePath()).arg(getDeviceName()),
                                                       tr("Sitzung Name (*.ses)")
                                                       );
    SessionFileDialog->setMode( QFileDialog::ExistingFile);
    if ( SessionFileDialog->exec() == QDialog::Accepted ) {
        SessionName = SessionFileDialog->selectedFile();
        UpdateRecentSESList(SessionName);
        onLoadSession(SessionName); // eigene session laden
        emit onLoadSessionSignal(SessionName); // jetzt die anderen
    }
    delete SessionFileDialog;
}



void WMViewBase::closeEvent(QCloseEvent* ce)
{
    m_widGeometry.setPoint(pos());
    m_widGeometry.setSize(size());
    m_widGeometry.setVisible(0);
    onSaveSession(".ses");
    ce->accept();
}


void WMViewBase::resizeEvent(QResizeEvent *)
{
    onSaveSession(".ses");
}


void WMViewBase::moveEvent(QMoveEvent *)
{
    onSaveSession(".ses");
}

void WMViewBase::setDeviceName(QString str)
{
    m_deviceName = str;
}



void WMViewBase::UpdateRecentSESList( QString ses )
{
    recentSESFiles.remove(ses); // wenn es den eintrag schon gibt -> löschen
    recentSESFiles.push_front(ses); // dann neuen an den anfang der liste setzen

    while ((int) recentSESFiles.size()>nmaxRecentSESFiles)
        recentSESFiles.pop_back();
    int n = (int) recentSESFiles.size();

    for (int i=0; i<n; i++)
    {
        QString text = QString ("&%1 %2").arg(i+1).arg(strippedName(recentSESFiles[i]));
        if (m_nrecentSESFileIds[i]==-1)
        {
            m_nrecentSESFileIds[i]=ui->Messung->insertItem(text,this,SLOT(OpenRecentSESFileSlot(int)),0,i,3+i);
            ui->Messung->setItemParameter(m_nrecentSESFileIds[i],i);
        }
        else
        {
            ui->Messung->changeItem(m_nrecentSESFileIds[i],text);
        }
    }
}

QString WMViewBase::getDeviceName()
{
    return m_deviceName;
}


void WMViewBase::OpenRecentSESFileSlot(int index)
{
    QString s = recentSESFiles[index];
    UpdateRecentSESList(s); // liste event. kürzen und einträge ins menu
    onLoadSession(s); // eigene session laden
    emit onLoadSessionSignal(s); // jetzt die anderen
}


void WMViewBase::SetGermanSlot()
{
    m_ConfData.Language = de;
    emit SendConfDataSignal(&m_ConfData);
    QCoreApplication::instance()->exit(10);
}


void WMViewBase::SetEnglishSlot()
{
    m_ConfData.Language = gb;
    emit SendConfDataSignal(&m_ConfData);
    QCoreApplication::instance()->exit(10);
}


void WMViewBase::SetPolishSlot()
{
    m_ConfData.Language = pl;
    emit SendConfDataSignal(&m_ConfData);
    QCoreApplication::instance()->exit(10);
}


void WMViewBase::JustFlashProgSlot()
{
    emit JustFlashProgSignal(); // signal an wm3000 das flash zu programmieren
}


void WMViewBase::JustFlashExportSlot()
{
    QString File = QFileDialog::getSaveFileName( this,
                                                tr("Justagedaten exportieren"),
                                                 QString("%1/%2/%3").arg(QDir::homePath()).arg(getDeviceName()).arg("JData.xml"),
                                                 tr("Datei Name (*.xml)")
                                                 );
    if (File != "") // wenn ""  -> es war cancel
        emit JustFlashExportSignal(File);
}


void WMViewBase::JustFlashImportSlot()
{
    QString File = QFileDialog::getOpenFileName(this,
                                                tr("Justagedaten importieren"),
                                                QString("%1/%2/%3").arg(QDir::homePath()).arg(getDeviceName()).arg("JData.xml"),
                                                 tr("Datei Name (*.xml)")
                                                 );

    if (File != "") // wenn ""  -> es war cancel
        emit JustFlashImportSignal(File);
}


void WMViewBase::SaveDefaultSessionSlot(bool)
{
    onSaveSession(".ses");
    emit onSaveSessionSignal(".ses"); // die anderen
}

void WMViewBase::RemoteCtrlInfoSlot(bool remote )
{
    uint i;

    for (i = 0; i < ui->Datei->count(); i++) // wenn remote betrieb alle items disablen
        ui->Datei->setItemEnabled (ui->Datei->idAt(i) , !remote) ;
    for (i = 0; i < ui->Messung->count(); i++) // wenn remote betrieb alle items disablen
        ui->Messung->setItemEnabled (ui->Messung->idAt(i) , !remote) ;
    for (i = 0; i < ui->Sprache->count(); i++) // wenn remote betrieb alle items disablen
        ui->Sprache->setItemEnabled (ui->Sprache->idAt(i) , !remote) ;
    ui->Hilfe->setItemEnabled(ui->Hilfe->idAt(7), !remote); // kein selbsttest wenn remote
}


void WMViewBase::SetJustifiedSlot( bool b )
{
    m_bJustified = b;
    ActualizeStates();
}


void WMViewBase::SetFreqStatSlot(bool b)
{
    m_bFreqQuestionable = b;
    ActualizeStates();
}

void WMViewBase::SetPPSStatSlot(bool b)
{
    m_bPPSQuestionable = b;
    ActualizeStates();
}
