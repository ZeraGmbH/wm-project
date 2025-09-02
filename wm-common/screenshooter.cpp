#include "screenshooter.h"
#include "qapplication.h"
#include "qdebug.h"
#include "qdesktopwidget.h"
#include "versserial.h"
#include <QDir>
#include <QtXml/qdom.h>

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
    mXmlExport.setPicFolderName("/pic/" + folderName);
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
    mXmlExport.addFoto(fileName);
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
        storeScreenShotW(mWidgetPoi, "03_HauptFenster");
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
    case ANLEITUNGWIDGET:
        storeScreenShotW(mWidgetPoi, "43_Anleitung");
        mActualNumber = INVALID;
        emit screenShotAnleiFinished();
        break;

    default:
        break;

    }
}

void screenshooter::exportXML()
{
    mXmlExport.storeXml();
}
/*
<DOKULIST>
    <DEVICE>WM3000I</DEVICE>
    <TYP>3k</TYP>
    <LANGUAGE>de</LANGUAGE>
    <PICFOLDER>../xml-definition/pic/dewm3000Idc/</PICFOLDER>
    <PROSAFOLDER>html-prosa/</PROSAFOLDER>
    <RELEASE>snapshot-2023-07-06</RELEASE>
    */

xmlScreenShooterExport::xmlScreenShooterExport()
{
    createDokuDefinition();
}

void xmlScreenShooterExport::addFoto(QString foto)
{
    QString str = foto.mid(0,2);
    int nr = str.toInt();
    mFotos.append(nr);
}

void xmlScreenShooterExport::storeXml()
{
    if(!mPicFolder.isEmpty() && !mType.isEmpty() &&! mLanguage.isEmpty() && !mDevice.isEmpty() && !mFileName.isEmpty() && !mRelease.isEmpty() &&!mProsaFolder.isEmpty() )
    {
        QDomDocument picDoc("MSWDokuDefinition");
        QDomElement rootTag;
        rootTag = picDoc.createElement( "DOKULIST");
        picDoc.appendChild(rootTag);
        QDomElement tag = picDoc.createElement( "DEVICE");
        rootTag.appendChild(tag);
        QDomText t = picDoc.createTextNode(mDevice);
        tag.appendChild( t );

        tag = picDoc.createElement( "TYP");
        rootTag.appendChild(tag);
        t = picDoc.createTextNode(mType);
        tag.appendChild( t );

        tag = picDoc.createElement( "LANGUAGE");
        rootTag.appendChild(tag);
        t = picDoc.createTextNode(mLanguage);
        tag.appendChild( t );

        tag = picDoc.createElement( "PICFOLDER");
        rootTag.appendChild(tag);
        t = picDoc.createTextNode(mPicFolder);
        tag.appendChild( t );

        tag = picDoc.createElement( "PROSAFOLDER");
        rootTag.appendChild(tag);
        t = picDoc.createTextNode(mProsaFolder);
        tag.appendChild( t );

        tag = picDoc.createElement( "RELEASE");
        rootTag.appendChild(tag);
        t = picDoc.createTextNode(mRelease);
        tag.appendChild( t );

/*        <ENTRIES>
            <ENTRY>
            <ITEM>File Menu</ITEM>
            <DESCRCOM>FileMenu.html</DESCRCOM>
            <FOTOLEFT>07_Datei.png</FOTOLEFT>
            <FOTORIGHT>08_Datei.png</FOTORIGHT>
            <DESCRCOMTEXT>FileMenuText.html</DESCRCOMTEXT>
            </ENTRY>*/

        QDomElement entries = picDoc.createElement( "ENTRIES" );
        rootTag.appendChild( entries );

        foreach(auto dokuItem, mDokuDefinition)
        {
            foreach(int nr, mFotos)
            {
                if (dokuItem.m_Nr == nr)
                {
                    QDomElement entry = picDoc.createElement( "ENTRY" );
                    entries.appendChild( entry );

                    QDomElement item = picDoc.createElement( "ITEM" );
                    entry.appendChild(item);
                    t = picDoc.createTextNode(dokuItem.mItem);
                    item.appendChild(t);
                    if (!dokuItem.mDescrCom.isEmpty())
                    {
                        item = picDoc.createElement( "DESCRCOM" );
                        entry.appendChild(item);
                        t = picDoc.createTextNode(dokuItem.mDescrCom);
                        item.appendChild(t);
                    }
                    if (!dokuItem.mFotoLeft.isEmpty())
                    {
                        item = picDoc.createElement( "FOTOLEFT" );
                        entry.appendChild(item);
                        t = picDoc.createTextNode(dokuItem.mFotoLeft);
                        item.appendChild(t);
                    }
                    if(!dokuItem.mFotoRight.isEmpty())
                    {
                        item = picDoc.createElement( "FOTORIGHT" );
                        entry.appendChild(item);
                        t = picDoc.createTextNode(dokuItem.mFotoRight);
                        item.appendChild(t);
                    }
                    if (!dokuItem.mFoto.isEmpty())
                    {
                        item = picDoc.createElement( "FOTO" );
                        entry.appendChild(item);
                        t = picDoc.createTextNode(dokuItem.mFoto);
                        item.appendChild(t);
                    }
                    if(!dokuItem.mDescrComText.isEmpty())
                    {
                    item = picDoc.createElement( "DESCRCOMTEXT" );
                    entry.appendChild(item);
                    t = picDoc.createTextNode(dokuItem.mDescrComText);
                    item.appendChild(t);
                    }
                    if(!dokuItem.mDescrTyp.isEmpty())
                    {
                    item = picDoc.createElement( "DESCRTYP" );
                    entry.appendChild(item);
                    t = picDoc.createTextNode(dokuItem.mDescrTyp);
                    item.appendChild(t);
                    }
                    if(!dokuItem.mDescr.isEmpty())
                    {
                    item = picDoc.createElement( "DESCR" );
                    entry.appendChild(item);
                    t = picDoc.createTextNode(dokuItem.mDescr);
                    item.appendChild(t);
                    }

                }
            }

        }

        QFile picFile(mFileName);
        if (picFile.open( QIODevice::WriteOnly ) ) {
            QString xml = picDoc.toString();
            QTextStream stream( &picFile );
            stream << xml;
            picFile.close();
        }
    }
}

void xmlScreenShooterExport::setPicFolderName(const QString name)
{   // name = "/pic/gbwm3000idcjust"
    // name = "/pic/dewm3000u"
    // name = "/pic/dewm3000u"
    QString temp;
    temp = name.simplified();
    mPicFolder = "../xml-definition" + temp + "/";
    if (temp.contains("wm3000")) mType = "3k"; else mType = "1k";
    if (temp.contains("/de")) mLanguage = "de";
    if (temp.contains("/gb")) mLanguage = "en";
    mDevice = temp.mid(7,7).toUpper();
    mFileName = QDir::homePath() + temp + ".xml";
    mProsaFolder = "html-prosa/";
    tVersSerial serialNr;
    mRelease = serialNr.GetReleaseVersion();

   // storeXml();
}

void xmlScreenShooterExport::createDokuDefinition()
{
    mDokuDefinition.clear();
    autoDokuDefinition entry;
    entry.m_Nr = 2;
    entry.mItem = "Main Screen";
    entry.mDescrCom = "AllesMenu.html";
    entry.mFoto = "02_Alles.png";
    entry.mDescrComText = "AllesText.html";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 7;
    entry.mItem = "File Menu";
    entry.mDescrCom = "FileMenu.html";
    entry.mFotoLeft = "07_Datei.png";
    entry.mFotoRight = "08_Datei.png";
    entry.mDescrComText = "FileMenuText.html";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 11;
    entry.mItem = "Meassurement Menu";
    entry.mDescrCom = "MeasMenu.html";
    entry.mFotoLeft = "11_Messung.png";
    entry.mFotoRight = "12_Messung.png";
    entry.mDescrTyp = "MeasMenuText.html";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 13;
    entry.mItem = "View Menu";
    entry.mDescrCom = "ViewMenu.html";
    entry.mFotoLeft = "13_Ansicht.png";
    entry.mFotoRight = "14_Ansicht.png";
    entry.mDescrTyp = "ViewMenuText.html";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 31;
    entry.mItem = "ERRORVALUES Menu";
    entry.mDescrCom = "ErrWind.html";
    entry.mFoto = "31_Fehler.png";
    entry.mDescrComText = "ErrWindText.html";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 32;
    entry.mItem = "ERRORVALUES SETTINGS";
    entry.mDescrCom = "ErrSettWind.html";
    entry.mFoto = "32_FehlerEinstell.png";
    entry.mDescrComText = "ErrSettWindText.html";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 33;
    entry.mItem = "Vektoren Main";
    entry.mDescrCom = "VekWind.html";
    entry.mFotoLeft = "33_Vektoren.png";
    entry.mDescrComText = "VekWindText.html";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 34;
    entry.mItem = "VektorenSettings Main";
    entry.mDescrCom = "VekSettWind.html";
    entry.mFotoLeft = "34_Vektoren.png";
    entry.mDescrComText = "VekSettWindText.html";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 35;
    entry.mItem = "Eigenfehler Main";
    entry.mDescrCom = "EigenWind.html";
    entry.mFotoLeft = "35_Eigen.png";
    entry.mDescrComText = "EigenWindText.html";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 36;
    entry.mItem = "Ethernet Menu";
    entry.mDescrCom = "EtherWind.html";
    entry.mFoto = "36_EthernMon.png";
    entry.mDescrComText = "EtherWindText.html";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 15;
    entry.mItem = "Settings Menu";
    entry.mDescrCom = "SettMenu.html";
    entry.mFotoLeft = "15_Einstell.png";
    entry.mFotoRight = "16_Einstell.png";
    entry.mDescrComText = "SettMenuText.html";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 17;
    entry.mItem = "Configuration Mode";
    entry.mDescrCom = "ConfModeWind.html";
    entry.mFoto = "17_Modus.png";
    entry.mDescr = "KonfModus.html";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 18;
    entry.mItem = "Configuration Calc";
    entry.mDescrCom = "ConfCalcWind.html";
    entry.mFoto = "18_Berechnung.png";
    entry.mDescrTyp = "ConfCalcText.html";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 19;
    entry.mItem = "Configuration Meas";
    entry.mDescrCom = "ConfMeasWind.html";
    entry.mFoto = "19_Messung.png";
    entry.mDescrComText = "ConfMeasText.html";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 20;
    entry.mItem = "Configuration Sync";
    entry.mDescrCom = "ConfSyncWind.html";
    entry.mFoto = "20_Synchr.png";
    entry.mDescrComText = "ConfSyncText.html";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 21;
    entry.mItem = "Configuration Divi";
    entry.mDescrCom = "ConfDiviWind.html";
    entry.mFoto = "21_Teiler.png";
    entry.mDescr = "ConfDiviText.html";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 22;
    entry.mItem = "Configuration nCon";
    entry.mDescrCom = "ConfnConWind.html";
    entry.mFoto = "22_nConv.png";
    entry.mDescrComText = "ConfnConText.html";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 23;
    entry.mItem = "Configuration Log";
    entry.mDescrCom = "ConfLogWind.html";
    entry.mFoto = "23_Logfile.png";
    entry.mDescrComText = "ConfLogText.html";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 38;
    entry.mItem = "keyboardhex";
    entry.mDescrCom = "keyhexWind.html";
    entry.mFotoLeft = "39_KeyboardHex.png";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 39;
    entry.mItem = "keyboardfloat";
    entry.mDescrCom = "keyfloatWind.html";
    entry.mFotoLeft = "38_KeyboardFloat.png";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 37;
    entry.mItem = "Ranges Main";
    entry.mDescrCom = "rangesWind.html";
    entry.mFotoLeft = "37_Messbereiche.png";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 37;
    entry.mItem = "Divider Main";
    entry.mDescrCom = "dividerWind.html";
    entry.mFotoLeft = "42_RatioWidget.png";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 37;
    entry.mItem = "Help Menu";
    entry.mDescrCom = "HelpMenu.html";
    entry.mFotoLeft = "24_Hilfe.png";
    entry.mFotoRight = "25_Hilfe.png";
    entry.mDescrComText = "HelpMenuText.html";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 41;
    entry.mItem = "version Main";
    entry.mDescrCom = "versionWind.html";
    entry.mFoto = "41_Version.png";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 43;
    entry.mItem = "Anleitung Main";
    entry.mDescrCom = "AnleitungWind.html";
    entry.mFoto = "43_Anleitung.png";
    mDokuDefinition.append(entry);
    entry.clear();
    entry.m_Nr = 26;
    entry.mItem = "Language Main";
    entry.mDescrCom = "LangMenu.html";
    entry.mFotoLeft = "26_Sprache.png";
    entry.mFotoRight = "27_Sprache.png";
    entry.mDescrComText = "LangMenuText.html";
    mDokuDefinition.append(entry);
}

autoDokuDefinition::autoDokuDefinition()
{
    m_Nr = 0;
    mItem = "";
    mDescrCom = "";
    mFotoLeft = "";
    mFotoRight = "";
    mFoto = "";
    mDescrComText = "";
    mDescrTyp = "";
    mDescr = "";
}

autoDokuDefinition::autoDokuDefinition(const autoDokuDefinition &t)
{

    this->m_Nr = t.m_Nr;
    this->mItem = t.mItem;
    this->mDescrCom = t.mDescrCom;
    this->mFotoLeft = t.mFotoLeft;
    this->mFotoRight = t.mFotoRight;
    this->mFoto = t.mFoto;
    this->mDescrComText = t.mDescrComText;
    this->mDescrTyp = t.mDescrTyp;
    this->mDescr = t.mDescr;

}

autoDokuDefinition &autoDokuDefinition::operator=(const autoDokuDefinition &t)
{
    autoDokuDefinition *local = new autoDokuDefinition;
    local->m_Nr = t.m_Nr;
    local->mItem = t.mItem;
    local->mDescrCom = t.mDescrCom;
    local->mFotoLeft = t.mFotoLeft;
    local->mFotoRight = t.mFotoRight;
    local->mFoto = t.mFoto;
    local->mDescrComText = t.mDescrComText;
    local->mDescrTyp = t.mDescrTyp;
    local->mDescr = t.mDescr;
    return *local;
}

void autoDokuDefinition::clear()
{
    m_Nr = 0;
    mItem = "";
    mDescrCom = "";
    mFotoLeft = "";
    mFotoRight = "";
    mFoto = "";
    mDescrComText = "";
    mDescrTyp = "";
    mDescr = "";
}
