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
    QObject::connect(&m_PollTimer, SIGNAL(timeout()), this, SLOT(onPollTimer()));
    connect(&m_geomChangeTimer, SIGNAL(sigWriteStreamForGeomChange()), this, SLOT(onWriteStreamForGeomChange()));
    onLoadSession(".ses");
}

EN61850monitor::~EN61850monitor()
{
    delete ui;
}

void EN61850monitor::onShowHide(bool shw)
{
    m_geomChangeTimer.handleGeomChange();
    if (shw) {
        show();
        emit sigRequestInformation(); // anfrage an wm3000 die status infos zu besorgen
    }
    else {
        close();
        m_PollTimer.stop();
    }
}

void EN61850monitor::closeEvent( QCloseEvent *ce )
{
    m_geomChangeTimer.handleGeomChange();
    emit sigIsVisible(false);
    ce->accept();
}

void EN61850monitor::resizeEvent(QResizeEvent*)
{
    m_geomChangeTimer.handleGeomChange();
}

void EN61850monitor::moveEvent(QMoveEvent*)
{
    m_geomChangeTimer.handleGeomChange();
}

void EN61850monitor::onPollTimer()
{
    emit sigRequestInformation(); // anfrage an wm3000 die status infos zu besorgen
}

void EN61850monitor::onSaveConfig()
{
    onSaveSession(".ses");
}

void EN61850monitor::onWriteStreamForGeomChange()
{
    m_geomToFromStream = geometryFromWidget(this);
    onSaveConfig();
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

void EN61850monitor::prepareNextPoll()
{
    m_PollTimer.setSingleShot(true);
    m_PollTimer.start(1000);
}

void EN61850monitor::actualizeByteCount(cEN61850Info *ethInfo)
{
    double count = ethInfo->ByteCount[0]*4294967296.0+ethInfo->ByteCount[1];
    QString s = QString("%1").arg( count, 0, 'f', 0 ); // keine nachkommastellen
    uint i,p,l;
    p = l = s.length();
    i = 1;
    while (p>3) {
        s.insert(l-(i*3),'.');
        i++;
        p -= 3;
    }
    ui->ByteCountValLabel->setText(s);
}

void EN61850monitor::actualizeSyncLostCount(cEN61850Info *ethInfo)
{
    QString s = QString("%1").arg(ethInfo->SyncLostCount);
    ui->LostSyncValLabel->setText(s);
}

void EN61850monitor::onETHStatus(cEN61850Info *ethInfo)
{
    actualizeByteCount(ethInfo);
    actualizeSyncLostCount(ethInfo);

    ulong stat = ethInfo->ETHErrors;
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

    prepareNextPoll();
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
    emit sigIsVisible(false);
    QDialog::accept();
}

void EN61850monitor::reject()
{
    emit sigResetETHStatus();
}
