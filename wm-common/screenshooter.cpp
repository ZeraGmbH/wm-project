#include "screenshooter.h"
#include "qapplication.h"
#include "qdebug.h"
#include "qdesktopwidget.h"
#include <QDir>

screenshooter::screenshooter(QObject *parent)
    : QObject{parent}
{
    mActualNumber = INVALID;
    mTimer = new QTimer;
    mTimer->setInterval(500);
    mTimer->setSingleShot(true);
#ifdef QT_DEBUG
    miVerboseLevel = 1;
#else
    miVerboseLevel = 0;
#endif
    connect(mTimer,SIGNAL(timeout()),this,SLOT(timerExpired()));
}

screenshooter::~screenshooter()
{
    delete mTimer;
}

void screenshooter::showUIbyExec(QMenu *uipoi)
{
    QPoint pos_UI, pos_Window, pos_Korr, pos_Aim;
    pos_UI = uipoi->pos();
    if (mOnTarget)
        pos_Korr = {2,26};
    else
        pos_Korr = {10,34};
    pos_Aim = pos_UI + pos_Korr;
    mActualNumber = INVALID;
    mTimer->start();
    uipoi->exec();
}

void screenshooter::useTimer(QWidget* poi,int nr)
{
    if ((miVerboseLevel & 1) == 1 )
        qWarning() << "timer start" ;
    mWidgetPoi = poi;
    mActualNumber = nr;
    mTimer->start();
}

void screenshooter::showed(bool bYes)
{
    mShowed = bYes;
}

bool screenshooter::showed()
{
    return mShowed;
}

void screenshooter::setWidgetPointer(QWidget *poi)
{
    mWidgetPoi = poi;
}

void screenshooter::setFolderName(const QString folderName)
{
    mFolderName = QDir::homePath() + "/pic/";
    if(!QDir(mFolderName).exists())
    {
        QDir().mkdir(mFolderName);
    }
    mFolderName += folderName + "/";
    if(!QDir(mFolderName).exists())
    {
        QDir().mkdir(mFolderName);
    }
}

void screenshooter::setFontSize(int size)
{
    if(size == 9) mOnTarget = true; else mOnTarget = false;
}

void  screenshooter::setConvent(bool convent)
{
    mWM1000 = convent;
}

void screenshooter::setJustage(bool just)
{
    mJustage = just;
}

bool screenshooter::isJustage()
{
    return mJustage;
}

void screenshooter::storeScreen(QString fileName)
{
    QPixmap map;
    WId intId = QDesktopWidget().internalWinId();
    map = map.grabWindow(intId);
    storeMap(&map,fileName);
}

uint screenshooter::storeScreenShot(QWidget* poi, QString fileName)
{
    QPixmap map;
    WId intId = poi->internalWinId();
    map = map.grabWindow(intId);
    storeMap(&map,fileName);

    return map.size().height();
}

void screenshooter::storeScreenShotW(QWidget* poi, QString fileName, int adjust)
{
    QPixmap map;
    QPoint pos;
    QRect rect;

    WId intId = QDesktopWidget().internalWinId();
    map = map.grabWindow(intId);
    pos = poi->pos();
    rect = poi->rect();
    rect.moveTo(pos);
    if (mOnTarget)
        rect.adjust(0,0,2,26);
    else
        rect.adjust(0,0,10,34);

    if (adjust != 0)
    {
        rect.adjust(0,0,0,adjust);
    }
    map = map.copy(rect);
    storeMap(&map,fileName);
}

bool screenshooter::storeMap(QPixmap *map, QString fileName)
{
    QString strFileName;
    strFileName= mFolderName + fileName +".png";
    if ((miVerboseLevel & 1) == 1 )
        qWarning() << "store" << fileName ;
    return map->save(strFileName);
}

void screenshooter::setTabWidgetPoi(QWidget* wpoi, QTabWidget *poi)
{
    mActualNumber = MODUS;
    mWidgetPoi = wpoi;
    mTabWidgetPoi = poi;
    mTabWidgetPoi->show();
    mTabWidgetPoi->setCurrentIndex(0);
    mTimer->start();
}

void screenshooter::setKeyBoardWidgetPoi(wmKeyboardForm *poi)
{
    mKeyboard = poi;
    mKeyboard->setHex(3);
    mKeyboard->show("0.042");
    mActualNumber = KEYBOARDFLOAT;
    mTimer->start();
}

void screenshooter::timerExpired()
{
    switch (mActualNumber){
    case INVALID:
        break;
    case MAIN:
        storeScreenShotW(mWidgetPoi, "2_HauptFenster");
        mActualNumber = INVALID;
        emit screenShotHauptFinished();
        break;
    case MODUS :
        storeScreenShotW(mWidgetPoi, "17_Modus");
        mActualNumber = BEREICH;
        mTabWidgetPoi->setCurrentIndex(1);
        mTimer->start();
        break;
    case BEREICH:
        storeScreenShotW(mWidgetPoi, "18_Berechnung");
        mActualNumber = MESSUNG;
        mTabWidgetPoi->setCurrentIndex(2);
        mTimer->start();
        break;
    case MESSUNG :
        storeScreenShotW(mWidgetPoi, "19_Messung");
        if (mWM1000)
        {
            mActualNumber =TEILER;
        }
        else
        {
            mActualNumber =SYNCHR;
        }
        mTabWidgetPoi->setCurrentIndex(3);
        mTimer->start();
        break;
    case SYNCHR:
        storeScreenShotW(mWidgetPoi, "20_Synchr");
        mActualNumber =TEILER;
        mTabWidgetPoi->setCurrentIndex(4);
        mTimer->start();
        break;
    case TEILER:
        storeScreenShotW(mWidgetPoi, "21_Teiler");
        if (mWM1000)
        {
            mActualNumber =LOGFIL;
            mTabWidgetPoi->setCurrentIndex(4);
        }
        else
        {
            mActualNumber =NCONV;
            mTabWidgetPoi->setCurrentIndex(5);
        }
        mTimer->start();
        break;
    case NCONV:
        storeScreenShotW(mWidgetPoi, "22_nConv");
        mActualNumber =LOGFIL;
        mTabWidgetPoi->setCurrentIndex(6);
        mTimer->start();
        break;
    case LOGFIL:
        storeScreenShotW(mWidgetPoi, "23_Logfile");
        mActualNumber = INVALID;
        mTabWidgetPoi->setCurrentIndex(0);
        mWidgetPoi->hide();
        emit keyboardScreenShot();
        break;
    case FEHLERMESS:
        storeScreenShotW(mWidgetPoi, "31_Fehler");
        mActualNumber = INVALID;
        emit screenShotFehlerFinished();
        break;
    case FEHLERMESSEINSTELL:
        storeScreenShotW(mWidgetPoi, "32_FehlerEinstell");
        mActualNumber = INVALID;
        emit screenShotFehlerEinstellFinished();
        break;
    case VEKTOREN:
        storeScreenShotW(mWidgetPoi, "33_Vektoren");
        mActualNumber = INVALID;
        emit screenShotVektorFinished();
        break;
    case VEKTORENEINSTELL:
        storeScreenShotW(mWidgetPoi, "34_Vektoren");
        mActualNumber = INVALID;
        emit screenShotVektorEinstellFinished();
        break;
    case EIGENFEHLER:
        storeScreenShotW(mWidgetPoi, "35_Eigen");
        mActualNumber = INVALID;
        emit screenShotEigenFinished();
        break;
    case ENMONITOR:
        storeScreenShotW(mWidgetPoi, "36_EthernMon");
        mActualNumber = INVALID;
        emit screenShotEtherMonFinished();
        break;
    case MESSBEREICHE:
        storeScreenShotW(mWidgetPoi, "37_Messbereiche");
        mActualNumber = INVALID;
        emit screenShotMessBerFinished();
        break;
    case KEYBOARDFLOAT:
        storeScreenShotW(mKeyboard, "38_KeyboardFloat");
        mActualNumber= KEYBOARDHEX;
        mKeyboard->hide();
        mKeyboard->setHex(0);
        mKeyboard->show("0x42");
        mTimer->start();
        break;
    case KEYBOARDHEX:
        storeScreenShotW(mKeyboard, "39_KeyboardHex");
        mActualNumber = INVALID;
        mKeyboard->hide();
        emit keyboardScreenShotFinished();
        break;
    case INFO:
        storeScreenShotW(mWidgetPoi, "40_Info");
        mActualNumber = INVALID;
        emit screenShotInfoFinished();
        break;
    case VERSION:
        storeScreenShotW(mWidgetPoi, "41_Version");
        mActualNumber = INVALID;
        emit screenShotVersionFinished();
        break;
    case RATIOWIDGET:
        storeScreenShotW(mWidgetPoi, "42_RatioWidget");
        mActualNumber = INVALID;
        emit screenShotRatioFinished();
        break;
    default:
        break;

    }
}
