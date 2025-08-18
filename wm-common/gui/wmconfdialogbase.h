#ifndef WMCONFDIALOGBASE_H
#define WMCONFDIALOGBASE_H

#include "confdata.h"
#include "confguihelper.h"
#include "qdialog.h"
#include "screenshooter.h"
#include "wmmessagebox.h"
#include <QObject>

class wmconfdialogbase : public QDialog
{
    Q_OBJECT
public:
    wmconfdialogbase(QWidget* parent);
    ~wmconfdialogbase();
    bool is_3( const QString & s );
    bool is_w3( const QString & s );
    void setScreenShooter(screenshooter *poi);

protected:
    screenshooter* mScrShooter;
    cConfData m_ConfData, m_ConfDataTemp;
    bool m_bRemoteCtrl;
    QString m_sText;
    wmKeyboardForm* mWmKeyBoard;
    confGuiHelper *mGuiHelper;
    wmMessageBox mWmMsgBox;

    void showRatio(QWidget *poi);
    void SuggestASDUs();
private:

};

#endif // WMCONFDIALOGBASE_H
