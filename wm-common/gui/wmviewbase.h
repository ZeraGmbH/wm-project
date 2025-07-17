/****************************************************************************
** Form interface generated from reading ui file 'wmviewbase.ui'
**
** Created: Mi Sep 19 14:11:07 2012
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef WMVIEWBASE_H
#define WMVIEWBASE_H

#include <QMainWindow>
#include "maxrecentfiles.h"
#include "confdata.h"
#include "screenshooter.h"
#include "wmeditor.h"
#include "widgetgeometry.h"
#include "statuslabelcontainer.h"
#include "wmglobalcommon.h"

namespace Ui {
    class WMViewBase;
}

class WMViewBase : public QMainWindow, public IStatusLabelXRangeParent
{
    Q_OBJECT

public:
    explicit WMViewBase(WMViewBase* parent = 0);
    ~WMViewBase();
    virtual QString strippedName( QString s );
    void removeJustageItem();
    void configureWM1000Items();
    void setWMWindowTitle();
    void configureWMwoDC();
    virtual void ActualizeStates();
    void setScreenShooter(screenshooter *poi);

public slots:
    void SetViewConfDataInfoSlot( cConfData * cd );
    virtual void OpenOETFileSlot();
    virtual void UpdateRecentFileList( QStringList & sl, QString f );
    virtual void OpenRecentOETFileSlot( int index );
    virtual void SimulationSlot();
    virtual void StartSlot();
    virtual void CreateStatusBar();
    virtual void CloseOETFileSlot();
    virtual void EditOETFileSlot();
    virtual void ReceiveOETFileSlot( QString nText );
    virtual void OpenResultFileSlot();
    virtual void EditResultFileSlot();
    virtual void ReceiveResultFileSlot( QString nText );
    virtual void CloseResultFileSlot();
    virtual void StoreResultSlot();
    virtual void OpenRecentResultFileSlot( int index );
    void onSaveSession(QString session);
    bool onLoadSession(QString session);
    virtual void RemoteCtrlInfoSlot( bool remote );
    virtual void SetJustifiedSlot( bool b );
    virtual void SetFreqStatSlot(bool b);
    virtual void SetPPSStatSlot(bool b);
    void takeScreenshoots();
    void takeScreenshootFinished();

signals:
    void SendConfDataSignal(cConfData*);
    void onSaveSessionSignal(QString);
    void onLoadSessionSignal(QString);
    void StoreResultSignal();
    void StartRangeObsermatic();
    void JustFlashProgSignal();
    void JustFlashImportSignal(QString);
    void JustFlashExportSignal(QString);
    void UIansichtScopeViewToggled(bool);
    void UIansichtScopeViewSet(bool);
    void UIansichtFehlerMessungActionToggled(bool);
    void UIansichtFehlerMessungActionSet(bool);
    void UIansichtEigenfehlerActionToggled(bool);
    void UIansichtEigenfehlerActionSet(bool);
    void UIansichtIstwerteActionToggled(bool);
    void UIansichtIstwerteActionSet(bool);
    void UIansichtOffsetActionToggled(bool);
    void UIansichtOffsetActionSet(bool);
    void UIansichtDialogActionToggled(bool);
    void UIansichtDialogActionSet(bool);
    void UIansichtEN61850ActionToggled(bool);
    void UIansichtEN61850ActionSet(bool);
    void UIeinstellungenConfActionActivated();
    void UIeinstellungenBereichActionActivated();
    void UIJustageAmplitudeActionActivated();
    void UIJustagePhaseActionActivated();
    void UIJustageKoeffBerechnungActionActivated();
    void UIJustageOffsetActionActivated();
    void UIJustageOffsetBerechnungActionActivated();
    void UIJustageOffsetVarActionActivated();
    void UIMessungOffsetKanalNActivated();
    void UIMessungOffsetKanalXActivated();
    void UIhilfeInfo_ber_QtActionActivated();
    void UIhilfeInfo_ber_ZeraActionActivated();
    void UIhilfeInfoActionActivated();
    void UIhilfeSelbsttestActionActivated();
    void UIdateiBeendenActionActivated();
    void UIhilfeVersionActionActivated();
    void UIhilfeReleaseInfoActionActivated();
    void ScreenshooterTriggeredByUser();

protected:
    virtual void closeEvent(QCloseEvent *ce) override;
    virtual void resizeEvent (QResizeEvent *) override;
    virtual void moveEvent(QMoveEvent *) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    cConfData m_ConfData;
    Ui::WMViewBase *ui;
    bool m_bJustified;
    bool m_bFreqQuestionable;
    bool m_bPPSQuestionable;
    StatusLabelContainer m_statusLabelContainer;

    QStringList recentResultFiles;
    QStringList recentOETFiles;
    void setDeviceName(QString);
    void init();

private:
    int m_nrecentSESFileIds[nmaxRecentSESFiles];
    WidgetGeometry m_widGeometry;
    QString SessionName;
    wmEditor* wmEdit;
    QStringList recentMVFiles;
    int m_nrecentOEFileIds[nmaxRecentOEFiles];
    int m_nrecentMVFileIds[nmaxRecentMVFiles];
    wmEditor* wmEdit2;
    QStringList recentSESFiles;
    QString m_deviceName;
    bool m_isWm1000=false;
    screenshooter* mScrShooter;

    void destroy();
    void UpdateRecentSESList( QString ses );
    QString getDeviceName();

private slots:
    virtual void StoreSessionSlot();
    virtual void onLoadSessionSlot();
    void OpenRecentSESFileSlot( int index );
    virtual void SetGermanSlot();
    virtual void SetEnglishSlot();
    virtual void SetPolishSlot();
    virtual void JustFlashProgSlot();
    virtual void JustFlashExportSlot();
    virtual void JustFlashImportSlot();
    virtual void SaveDefaultSessionSlot(bool);
    void AutoScreenShoterTriggered();
};

#endif // WMVIEWBASE_H
