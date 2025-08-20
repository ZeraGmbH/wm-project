#ifndef RANGEDIALOGBASE_H
#define RANGEDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <wmrangedialogbase.h>
#include "range.h"
#include "confdata.h"
#include "screenshooter.h"


namespace Ui {
    class RangeDialogBase;
}

class RangeDialogBase : public wmRangeDialogBase
{
    Q_OBJECT

public:
    explicit RangeDialogBase(QWidget* parent = 0, QString machineName = "");
    ~RangeDialogBase();
    void setScreenShooter(screenshooter* poi);

public slots:
    virtual void SetRangeListSlot( cWMRangeList & nx, cWMRangeList & evt );
    void SetConfInfoSlot( cConfData * cd );
    void SetNRangeSlot( const QString & nr );
    void SetXRangeSlot( const QString & xr );
    void SetEVTRangeSlot( const QString & er );
    virtual void RemoteCtrlInfoSlot( bool );
    void takeScreenshoots();
    void takeScreenshootFinished();

signals:
    void SendRange(cConfData*);
    void isVisible(bool);
    void SendRangeInfoReqSignal();

protected:
    virtual void showEvent( QShowEvent * );
    virtual void closeEvent (QCloseEvent * );

private:
    Ui::RangeDialogBase *ui;	
    cConfData m_ConfData;
    QStringList m_sNXItemList;
    QStringList m_sEVTItemList;
    screenshooter* mScrShooter;

    void ActualizeDialog();

};

#endif // RANGEDIALOGBASE_H
