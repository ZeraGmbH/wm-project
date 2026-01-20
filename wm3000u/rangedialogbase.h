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

public slots:
    virtual void SetRangeListSlot( cWMRangeList & nx, cWMRangeList & evt );
    void SetConfInfoSlot( cConfData * cd );
    void SetNRangeSlot( const QString & nr );
    void SetXRangeSlot( const QString & xr );
    void SetEVTRangeSlot( const QString & er );
    virtual void RemoteCtrlInfoSlot( bool );

signals:
    void SendRange(cConfData*);
    void isVisible(bool);
    void SendRangeInfoReqSignal();

protected:
    virtual QDialog* getChildThis() override;

private:
    Ui::RangeDialogBase *ui;	
    cConfData m_ConfData;
    QStringList m_sNXItemList;
    QStringList m_sEVTItemList;

    void ActualizeDialog();
};

#endif // RANGEDIALOGBASE_H
