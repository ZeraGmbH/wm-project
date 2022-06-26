#include "logfileview.h"
#include <QString>
#include <QTime>
#include "sessionreadwrite.h"

CLogFileView::CLogFileView(const QString cap,
                           const long max,
                           QWidget *parent,
                           const char *wname,
                           QString machineName) :
    QDialog(parent, wname),
    m_sessionReadWrite(machineName)
{
    setCaption(cap);
    m_pText = new Q3TextEdit(this);
    m_pText->setTextFormat(Qt::LogText);
    m_pText->setMaxLogLines(max);
    setMinimumSize(200,200);
    LoadSession(".ses");
    m_timerDelayShow.start(2000);
    QObject::connect(&m_timerDelayShow,SIGNAL(timeout()),this,SLOT(showList()));
    connect(&m_geomHandler, SIGNAL(sigNeedsStreamWrite()), this, SLOT(saveConfiguration()));
}

CLogFileView::~CLogFileView()
{
    SaveSession(".ses");
}

void CLogFileView::ShowHideLogFileSlot(bool shw)
{
    m_geomHandler.handleVisibleChange(shw);
    if (shw)
        show();
    else
        close();
}

void CLogFileView::AddLogTextSlot(const QString& s)
{
    m_loglist.append(QTime::currentTime().toString("HH:mm:ss:zzz") + ": " + s.stripWhiteSpace()); // neue daten in die liste
}

void CLogFileView::showList()
{
    if (m_loglist.count()) {
        QString s = m_loglist.join("\n");
        m_loglist.clear();
        m_pText->append(s);
    }
}

void CLogFileView::saveConfiguration()
{
    SaveSession(".ses");
}

void CLogFileView::SaveSession(QString session)
{
    m_sessionReadWrite.writeSession(this, m_geomHandler.getGeometry(), session);
}

bool CLogFileView::LoadSession(QString session)
{
    WidgetGeometry tmpGeometry = m_sessionReadWrite.readSession(this, session);
    if(tmpGeometry.getSize().isValid()) {
        m_geomHandler.setGeometry(tmpGeometry);
        return true;
    }
    return false;
}

void CLogFileView::resizeEvent (QResizeEvent*)
{
    m_pText->resize(size());
    m_geomHandler.handleResize(size());
}

void CLogFileView::moveEvent(QMoveEvent*)
{
    m_geomHandler.handleMove(pos());
}

void CLogFileView::closeEvent (QCloseEvent* ce)
{
    m_geomHandler.handleVisibleChange(false);
    emit isVisibleSignal(false);
    ce->accept();
}
