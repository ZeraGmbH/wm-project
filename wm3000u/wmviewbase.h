#ifndef WMVIEWBASE_H
#define WMVIEWBASE_H

#include <Q3MainWindow>
#include "maxrecentfiles.h"
#include "confdata.h"
#include "wmeditor.h"
#include "widgetgeometry.h"
#include "statuslabelcontainer.h"

namespace Ui {
    class WMViewBase;
}

class WMViewBase : public Q3MainWindow, public IStatusLabelXRangeParent
{
    Q_OBJECT

public:
    explicit WMViewBase( QWidget* parent = 0);
    ~WMViewBase();
    virtual QString strippedName( QString s );
    void removeJustageItem();
    void configureWM1000Items();
    void configureWMwoDC();

public slots:
    void SetViewConfDataInfoSlot( cConfData * cd );
    virtual void ActualizeStates();
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
    bool onLoadSession(QString session);
    void onSaveSession(QString session);
    virtual void RemoteCtrlInfoSlot( bool remote );
    virtual void SetJustifiedSlot( bool b );
    virtual void SetFreqStatSlot(bool b);
    virtual void SetPPSStatSlot(bool b);

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

protected:
    virtual void closeEvent(QCloseEvent * ce ) override;
    virtual void resizeEvent(QResizeEvent *) override;
    virtual void moveEvent(QMoveEvent *) override;


private:
    Ui::WMViewBase *ui;
    int m_nrecentSESFileIds[nmaxRecentSESFiles];
    WidgetGeometry m_widGeometry;
    QString SessionName;
    QStringList recentResultFiles;
    wmEditor* wmEdit;
    QStringList recentMVFiles;
    cConfData m_ConfData;
    int m_nrecentOEFileIds[nmaxRecentOEFiles];
    QStringList recentOETFiles;
    int m_nrecentMVFileIds[nmaxRecentMVFiles];
    wmEditor* wmEdit2;
    QStringList recentSESFiles;
    bool m_bJustified;
    bool m_bFreqQuestionable;
    bool m_bPPSQuestionable;
    StatusLabelContainer m_statusLabelContainer;

    void init();
    void destroy();
    void UpdateRecentSESList( QString ses );
    virtual void updateXRangeLabel(QLabel* xRangeLabel) override;

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

};

#endif // WMVIEWBASE_H
