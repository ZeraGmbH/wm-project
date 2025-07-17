#ifndef SCREENSHOOTER_H
#define SCREENSHOOTER_H

#include "qevent.h"
#include "qmenu.h"
#include "qtabwidget.h"
#include "qtimer.h"
#include "wmkeyboardform.h"
#include "QEvent"
#include <QObject>

class screenshooter : public QObject
{
    Q_OBJECT
public:
    explicit screenshooter(QObject *parent = nullptr);
    ~screenshooter();

    void showUIbyExec(QMenu* uipoi);
    void setTabWidgetPoi(QWidget* wpoi, QTabWidget* poi );
    void setKeyBoardWidgetPoi(wmKeyboardForm *poi);
    void storeScreen(QString fileName);
    uint storeScreenShot(QWidget *poi, QString fileName);
    void storeScreenShotW(QWidget *poi, QString fileName,int adjust = 0);
    void useTimer(QWidget *poi, int nr = INVALID);
    void showed(bool bYes);
    bool showed();
    void setWidgetPointer(QWidget* poi);
    void setFolderName(const QString folderName);
signals:
    void keyboardScreenShot();
    void keyboardScreenShotFinished();
    void screenShotFehlerFinished();
    void screenShotFehlerEinstellFinished();
    void screenShotVektorFinished();
    void screenShotVektorEinstellFinished();
    void screenShotEigenFinished();
    void screenShotEtherMonFinished();
    void screenShotMessBerFinished();
    void screenShotHauptFinished();
    void screenShotInfoFinished();
    void screenShotVersionFinished();
    void update(uint nr, QString text);
private:
    bool storeMap(QPixmap* map, QString fileName);
//    void initMouseEvent();
    bool mShowed;
    int mActualNumber;
    QTimer* mTimer;
    QWidget* mWidgetPoi;
    QTabWidget* mTabWidgetPoi;
    wmKeyboardForm* mKeyboard;
    QString mFolderName;
    enum PictureNumber{
        INVALID = 0,
        MAIN = 2,
        MODUS = 17,
        BEREICH,
        MESSUNG,
        SYNCHR,
        TEILER,
        NCONV,
        LOGFIL,
        FEHLERMESS = 31,
        FEHLERMESSEINSTELL,
        VEKTOREN,
        VEKTORENEINSTELL,
        EIGENFEHLER,
        ENMONITOR,
        MESSBEREICHE,
        KEYBOARDFLOAT =38,
        KEYBOARDHEX,
        INFO,
        VERSION
    };
private slots:
    void timerExpired();


};

#endif // SCREENSHOOTER_H
