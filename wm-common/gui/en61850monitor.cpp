#include "en61850monitor.h"
#include "ui_en61850monitor.h"
#include "geometrytowidget.h"
#include <QCloseEvent>
#include <QTimer>

EN61850monitor::EN61850monitor(QWidget* parent, QString machineName):
    QDialog(parent),
    ui(new Ui::EN61850monitor),
    m_sessionStreamer(machineName, this)
{
    ui->setupUi(this);
    ETHStatus.ByteCount[0] = 0;
    ETHStatus.ByteCount[1] = 0;
    ETHStatus.SyncLostCount = 0;
    ETHStatus.ETHErrors = 0;

    m_pTimer = new QTimer();
    QObject::connect(m_pTimer,SIGNAL(timeout()),this,SLOT(TimerSlot()));
    connect(&m_geomChangeTimer, SIGNAL(sigWriteStreamForGeomChange()), this, SLOT(onWriteStreamForGeomChange()));
    onLoadSession(".ses");
}

EN61850monitor::~EN61850monitor()
{
    saveConfiguration();
    delete m_pTimer;
    delete ui;
}

void EN61850monitor::onShowHide(bool shw)
{
    m_geomChangeTimer.handleGeomChange();
    if (shw) {
        show();
        emit InformationRequest(); // anfrage an wm3000 die status infos zu besorgen
        m_pTimer->start(2000); // wenn sichtbar -> timer läuft
    }
    else {
        close();
        m_pTimer->stop();
    }
}

void EN61850monitor::closeEvent( QCloseEvent * ce )
{
    m_geomChangeTimer.handleGeomChange();
    emit isVisibleSignal(false);
    ce->accept();
}

void EN61850monitor::resizeEvent(QResizeEvent *)
{
    m_geomChangeTimer.handleGeomChange();
}

void EN61850monitor::moveEvent(QMoveEvent *)
{
    m_geomChangeTimer.handleGeomChange();
}

void EN61850monitor::TimerSlot()
{
    emit InformationRequest(); // anfrage an wm3000 die status infos zu besorgen
}

void EN61850monitor::saveConfiguration()
{
    onSaveSession(".ses");
}

void EN61850monitor::onWriteStreamForGeomChange()
{
    m_geomToFromStream = geometryFromWidget(this);
    saveConfiguration();
}

void EN61850monitor::readStream(QDataStream &stream)
{
    stream >> m_geomToFromStream;
    geometryToWidget(m_geomToFromStream, this);
}

void EN61850monitor::writeStream(QDataStream &stream)
{
    stream << m_geomToFromStream;
}

void EN61850monitor::setDefaults()
{
}

void EN61850monitor::SetETHStatusSlot( cEN61850Info *ethInfo )
{
    QString s;
    double count;
    ulong stat;

    ETHStatus = *ethInfo;
    count = ETHStatus.ByteCount[0]*4294967296.0+ethInfo->ByteCount[1];
    s = QString("%1").arg( count, 0, 'f', 0 ); // keine nachkommastellen
    uint i,p,l;
    p = l = s.length();
    i = 1;
    while (p>3) {
        s.insert(l-(i*3),'.');
        i++;
        p -= 3;
    }

    ui->ByteCountValLabel->setText(s);

    count = ETHStatus.SyncLostCount;
    s = QString("%1").arg( count, 0, 'f', 0 ); // keine nachkommastellen
    ui->LostSyncValLabel->setText(s);

    stat = ETHStatus.ETHErrors;

    ui->savPducheckBox->setChecked(stat & savPdu);
    ui->ASDUcheckBox->setChecked(stat & noASDU);
    ui->seqASDUcheckBox->setChecked(stat & seqASDU);
    ui->seqASDUncheckBox->setChecked(stat & seqASDUn);
    ui->serviceIdentcheckBox->setChecked(stat & svID);
    ui->smpCountcheckBox->setChecked(stat & smpCnt);
    ui->confRevcheckBox->setChecked(stat & confRev);
    ui->smpSynchcheckBox->setChecked(stat & smpSync);
    ui->seqDatacheckBox->setChecked(stat & seqData);

    ui->MACSyncLostcheckBox->setChecked(stat & macSyncLost);
    ui->DataSyncLostcheckBox->setChecked(stat & dataSyncLost);
    ui->nASDUcheckBox->setChecked(stat & ASDUnavail);
    ui->ETHMacAdrcheckBox->setChecked(stat & ETHMacAdressError);
    ui->ETHHeadercheckBox->setChecked(stat & ETHHeaderAppIdError);
    ui->PriorityTaggedcheckBox->setChecked(stat & ETHPriorityTaggedError);

    ui->FIFOOvfcheckBox->setChecked(stat & FifoOverflow);
    ui->CRCErrorcheckBox->setChecked(stat & CRCError);
    ui->AlignErrorcheckBox->setChecked(stat & AlignError);
    ui->PHysLayercheckBox->setChecked(stat & PhysicalLayer);
    ui->RWTOcheckBox->setChecked(stat & ReceiveWDogTimeout);
    ui->LateCollisioncheckBox->setChecked(stat & LateCollisionSeen);
    ui->RUNTFramecheckBox->setChecked(stat & RuntFrame);
}

bool EN61850monitor::onLoadSession(QString session)
{
    m_sessionStreamer.readSession(objectName(), session);
    return true;
}

void EN61850monitor::onSaveSession(QString session)
{
    m_sessionStreamer.writeSession(objectName(), session);
}

void EN61850monitor::accept()
{
    emit isVisibleSignal(false);
    QDialog::accept();
}

void EN61850monitor::reject()
{
    emit ResetETHStatus();
}
