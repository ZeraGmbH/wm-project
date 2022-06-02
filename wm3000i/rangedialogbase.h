/****************************************************************************
** Form interface generated from reading ui file 'rangedialogbase.ui'
**
** Created: Mi Sep 19 14:11:07 2012
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef RANGEDIALOGBASE_H
#define RANGEDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include "wmglobal.h"


namespace Ui {
    class RangeDialogBase;
}

class RangeDialogBase : public QDialog
{
    Q_OBJECT

public:
    explicit RangeDialogBase( QWidget* parent = 0);
    ~RangeDialogBase();

public slots:
    virtual void SetRangeListSlot( cWMRangeList & nx, cWMRangeList & evt );
    void SetConfInfoSlot( cConfData * cd );
    void SetNRangeSlot( const QString & nr );
    void SetXRangeSlot( const QString & xr );
    void SetECTRangeSlot( const QString & er );
    virtual void RemoteCtrlInfoSlot( bool );

signals:
    void SendRange(cConfData*);
    void isVisible(bool);
    void SendRangeInfoReqSignal();

protected:
    virtual void showEvent( QShowEvent * );


private:
    Ui::RangeDialogBase *ui;
    cConfData m_ConfData;
    QStringList m_sNXItemList;
    QStringList m_sECTItemList;

    void ActualizeDialog();

};

#endif // RANGEDIALOGBASE_H
