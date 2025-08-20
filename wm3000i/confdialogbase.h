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
#include <wmconfdialogbase.h>
#include "confdata.h"
#include "screenshooter.h"
#include "wmkeyboardform.h"
#include "confguihelper.h"
#include "wmmessagebox.h"

namespace Ui {
    class ConfDialogBase;
}

class ConfDialogBase : public wmconfdialogbase
{
    Q_OBJECT

public:
    explicit ConfDialogBase(QWidget* parent = 0, QString machineName ="wm3000", bool onlyRatio = false);
    ~ConfDialogBase();
    void setKeyboard(wmKeyboardForm* keyboard);

public slots:
    virtual void SetConfInfoSlot( cConfData * cd );
    virtual void SetConfListSlot( QStringList & NPItems, QStringList & NSItems );
    virtual void cancel();
    virtual void accept();
    virtual void abortSlot();
    virtual void SetnConventMenu();
    virtual void RemoteCtrlInfoSlot( bool remote );
    void showRatio();

signals:
    void SendConfDataSignal(cConfData*);

protected:
    virtual void SetModeMenu();
    virtual void SetCmpMenu();
    virtual void SetSyncMenu();
    virtual void SetRatioMenu();
    virtual void SetLogMenu();
    virtual void SetMessungMenu();
    virtual QWidget* getRatioWidgetThis() override;
    virtual QTabWidget* getConfWidgetThis() override;


private:
    Ui::ConfDialogBase *ui;
    bool m_bOnlyRatio;

    void init();
    void Actualize();
    void SuggestASDUs();
    void clearUnitComboBoxes();

    bool acceptEot();
    bool acceptRatio();
    void resizeRatioTab();
private slots:
    void ApplyDataSlot();
    virtual void FxRadioButtonChecked();
    virtual void SxRadioButtonChecked();
    void on_ConfTabWidget_currentChanged(int index);
};


#endif // CONFDIALOGBASE_H
