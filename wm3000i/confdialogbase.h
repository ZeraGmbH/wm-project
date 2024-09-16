/****************************************************************************
** Form interface generated from reading ui file 'confdialogbase.ui'
**
** Created: Mi Sep 19 14:11:07 2012
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CONFDIALOGBASE_H
#define CONFDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include <QRadioButton>
#include "qcombobox.h"
#include "qvalidator.h"
#include "confdata.h"
#include "wmkeyboardform.h"
#include "confguihelper.h"

namespace Ui {
    class ConfDialogBase;
}

class ConfDialogBase : public QDialog
{
    Q_OBJECT

public:
    explicit ConfDialogBase( QWidget* parent = 0);
    ~ConfDialogBase();
    virtual bool is_3( const QString & s );
    virtual bool is_w3( const QString & s );
    void setKeyboard(wmKeyboardForm* keyboard);

public slots:
    virtual void SetConfInfoSlot( cConfData * cd );
    virtual void SetConfListSlot( QStringList & NPItems, QStringList & NSItems );
    virtual void accept() override;
    void abortSlot();
    virtual void SetnConventMenu();
    virtual void RemoteCtrlInfoSlot( bool remote );

signals:
    void SendConfDataSignal(cConfData*);

protected:
    virtual void SetModeMenu();
    virtual void SetCmpMenu();
    virtual void SetSyncMenu();
    virtual void SetRatioMenu();
    virtual void SetLogMenu();
    virtual void SetMessungMenu();


private:
    Ui::ConfDialogBase *ui;
    cConfData m_ConfData, m_ConfDataTemp;
    bool m_bRemoteCtrl;
    QString m_sText;
    wmKeyboardForm* mWmKeyBoard;
    confGuiHelper *mGuiHelper;

    void init();
    void Actualize();
    void SuggestASDUs();
    void clearUnitComboBoxes();

    bool acceptEot();
    bool acceptRatio();
private slots:
    void ApplyDataSlot();
    virtual void FxRadioButtonChecked();
    virtual void SxRadioButtonChecked();
    void on_ConfTabWidget_currentChanged(int index);
};


#endif // CONFDIALOGBASE_H
