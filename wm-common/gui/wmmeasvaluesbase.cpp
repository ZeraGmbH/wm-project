#include "wmmeasvaluesbase.h"
#include <QContextMenuEvent>
#include <QCloseEvent>
#include <Q3BoxLayout>
#include <QResizeEvent>
#include <QShowEvent>
#include <QFileInfo>
#include <QDir>
#include "ui_wmmeasvaluesbase.h"
#include "common-modes.h"
#include "loadpointunits.h"
#include "errorunits.h"
#include "angleunits.h"
#include "rcfunits.h"

WMMeasValuesBase::WMMeasValuesBase(QWidget *parent, QString machineName, QList<eUnit *> lpUnitList) :
    QDialog(parent),
    ui(new Ui::WMMeasValuesBase),
    m_sessionReadWrite(machineName, new SessionAppendCustom(this))
{
    ui->setupUi(this);
    init(lpUnitList);
}

WMMeasValuesBase::~WMMeasValuesBase()
{
    destroy();
    delete ui;
}


void WMMeasValuesBase::init(QList<eUnit *>lpUnitList)
{
    m_nDisplayMode = IEC; // wmglobal
    m_nLPDisplayMode = totalRms;
    m_pContextMenu = new WMMeasConfigBase(this, lpUnitList);
    m_Format[0] = cFormatInfo(7,3,LoadpointUnit[LPProzent]); // defaults
    m_Format[1] = cFormatInfo(7,3,ErrorUnit[ErrProzent]);
    m_Format[2] = cFormatInfo(7,4,AngleUnit[Anglegrad]);
    m_Format[3] = cFormatInfo(6,4,RCFUnit[nix]);
    m_Timer.setSingleShot(true);
    connect(this,SIGNAL(SendFormatInfoSignal(bool,int,int,int, cFormatInfo*)),m_pContextMenu,SLOT(ReceiveFormatInfoSlot(bool,int,int,int, cFormatInfo*)));
    connect(m_pContextMenu,SIGNAL(SendFormatInfoSignal(int,int,int, cFormatInfo*)),this,SLOT(ReceiveFormatInfoSlot(int,int,int, cFormatInfo*)));
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(saveConfiguration()));
    LoadSession(".ses");
}


void WMMeasValuesBase::destroy()
{
    SaveSession(".ses");
}


void WMMeasValuesBase::adjustBoxWidths()
{
    if (QLayout *lay=layout())
    {
        QLayoutIterator it = lay->iterator();
        QLayoutItem *child;
        int  w;
        while ( (child = it.current()) != 0 )
        {
            Q3BoxLayout *l = (Q3BoxLayout*) child->layout();
            w = l->minimumSize().width();
            ((Q3BoxLayout*) lay)->setStretchFactor(l,w);
            ++it;
        }
    }
}


void WMMeasValuesBase::closeEvent( QCloseEvent * ce)
{
    m_widGeometry.SetGeometry(pos(),size());
    m_widGeometry.SetVisible(0);
    emit isVisibleSignal(false);
    m_Timer.start(500);
    ce->accept();
}


void WMMeasValuesBase::ShowHideMVSlot(bool b)
{
    if (b)
        show();
    else
        close();
}


void WMMeasValuesBase::resizeEvent(QResizeEvent * e)
{
    adjustBoxWidths();
    this->QDialog::resizeEvent(e);
    m_Timer.start(500);
}

void WMMeasValuesBase::moveEvent(QMoveEvent *)
{
    m_Timer.start(500);
}


void WMMeasValuesBase::SetActualValuesSlot( cwmActValues * av)
{
    m_ActValues = *av;
    ActualizeDisplay(); // anzeige aktualisieren
}


void WMMeasValuesBase::ActualizeLPSlot( cwmActValues * av )
{
    m_ActValues = *av;
    ActualizeLoadPoint();
}


void WMMeasValuesBase::SetConfInfoSlot( cConfData * cd)
{
    m_ConfData = *cd;
    if (m_ConfData.m_bDCmeasurement)
    {
        ui->mBigAngleName->setVisible(false);
        ui->mBigAngleError->setVisible(false);
        ui->mBigAngleUnit->setVisible(false);
        m_nLPDisplayMode = totalRms;
        m_nDisplayMode = IEC; // wmglobal
    }
    else
    {
        ui->mBigAngleName->setVisible(true);
        ui->mBigAngleError->setVisible(true);
        ui->mBigAngleUnit->setVisible(true);
    }
    repaint();
}


void WMMeasValuesBase::ActualizeLoadPoint()
{
    double AnzeigeWertN, AnzeigeWertX;

    if (m_nLPDisplayMode == totalRms)
    {
        if (m_Format[0].UnitInfo.Name == "%")
        {
            AnzeigeWertN = m_ActValues.LoadPoint;
            AnzeigeWertX = m_ActValues.LoadPointX;
        }
        else
        {
            AnzeigeWertN = m_ActValues.RMSN / m_Format[0].UnitInfo.fak;
            AnzeigeWertX = m_ActValues.RMSX / m_Format[0].UnitInfo.fak;
        }
    }
    else
    {
        if (m_Format[0].UnitInfo.Name == "%")
        {
            AnzeigeWertN = m_ActValues.LoadPoint1;
            AnzeigeWertX = m_ActValues.LoadPoint1X;
        }
        else
        {
            AnzeigeWertN = m_ActValues.RMSN1 / m_Format[0].UnitInfo.fak;
            AnzeigeWertX = m_ActValues.RMSX1 / m_Format[0].UnitInfo.fak;
        }
    }

    ui->mBigLoadpointN->display(QString("%1").arg(AnzeigeWertN,m_Format[0].FieldWidth,'f',m_Format[0].Resolution));
    ui->mBigLPNUnit->display(m_Format[0].UnitInfo.Name);
    ui->mBigLoadpointX->display(QString("%1").arg(AnzeigeWertX,m_Format[0].FieldWidth,'f',m_Format[0].Resolution));
    ui->mBigLPXUnit->display(m_Format[0].UnitInfo.Name);

}


void WMMeasValuesBase::ActualizeDisplay()
{
    double AnzeigeWert;
    double normphi = 57.295779; // 360/(2*PI) winkel sind im bogenmass (rad)

    if (m_nDisplayMode == IEC)
        AnzeigeWert = m_ActValues.AmplErrorIEC;
    else
        AnzeigeWert = m_ActValues.AmplErrorANSI;

    AnzeigeWert = AnzeigeWert / ( 100.0 * m_Format[1].UnitInfo.fak );
    ui->mBigAmplError->display(QString("%1").arg(AnzeigeWert,m_Format[1].FieldWidth,'f',m_Format[1].Resolution));
    ui->mBigErrorUnit->display(m_Format[1].UnitInfo.Name);

    AnzeigeWert = m_ActValues.AngleError * normphi; // jetzt is et in °
    AnzeigeWert /= m_Format[2].UnitInfo.fak;
    ui->mBigAngleError->display(QString("%1").arg(AnzeigeWert,m_Format[2].FieldWidth,'f',m_Format[2].Resolution));
    ui->mBigAngleUnit->display(m_Format[2].UnitInfo.Name);

    AnzeigeWert = m_ActValues.RCF;
    ui->mBigRCF->display(QString("%1").arg(AnzeigeWert,m_Format[3].FieldWidth,'f',m_Format[3].Resolution));

    if (m_nDisplayMode == ANSI || !m_ActValues.bvalid || m_ConfData.m_bDCmeasurement)
    {
        ui->mBigAngleName->setEnabled(false);
        ui->mBigAngleError->setEnabled(false);
        ui->mBigAngleUnit->setEnabled(false);
    }
    else
    {
        ui->mBigAngleName->setEnabled(true);
        ui->mBigAngleError->setEnabled(true);
        ui->mBigAngleUnit->setEnabled(true);
    }

    if (m_ActValues.bvalid)
    {
        ui->mBigAmplError->setEnabled(true);
        ui->mBigErrorName->setEnabled(true);
        ui->mBigErrorUnit->setEnabled(true);
        ui->mBigRCF->setEnabled(true);
        ui->mBigRCFName->setEnabled(true);
        ui->mBigRCFUnit->setEnabled(true);
    }
    else
    {
        ui->mBigAmplError->setEnabled(false);
        ui->mBigErrorName->setEnabled(false);
        ui->mBigErrorUnit->setEnabled(false);
        ui->mBigRCF->setEnabled(false);
        ui->mBigRCFName->setEnabled(false);
        ui->mBigRCFUnit->setEnabled(false);
    }
}


bool WMMeasValuesBase::LoadSession(QString session)
{
    cWidgetGeometry tmpGeometry = m_sessionReadWrite.readSession(this, session);
    if(tmpGeometry.m_Size.isValid())
    {
        m_widGeometry = tmpGeometry;
        return true;
    }
    else
    {
        return false;
    }
}


void WMMeasValuesBase::SaveSession(QString session)
{
    m_sessionReadWrite.writeSession(this, m_widGeometry, session);
}


void WMMeasValuesBase::contextMenuEvent( QContextMenuEvent * )
{
    emit SendFormatInfoSignal(m_ConfData.m_bDCmeasurement, m_nDisplayMode,m_nLPDisplayMode, 4, m_Format);
    m_pContextMenu->show();
}

void WMMeasValuesBase::transferSessionCustom(QDataStream &stream, bool write)
{
    if(write) {
        for (int i = 0; i < 4; i++)
            stream << m_Format[i];

        stream << m_nDisplayMode;
        stream << m_nLPDisplayMode;
    }
    else {
        for (int i = 0; i< 4; i++)
            stream >> m_Format[i];

        stream >> m_nDisplayMode;
        stream >> m_nLPDisplayMode;
    }
}


void WMMeasValuesBase::ReceiveFormatInfoSlot(int m, int m2, int n, cFormatInfo* fi)
{
    int i;
    for(i = 0; i < n; i++, fi++)
        m_Format[i] = *fi;

    m_nDisplayMode = m;
    m_nLPDisplayMode = m2;
}


void WMMeasValuesBase::saveConfiguration()
{
    SaveSession(".ses");
}