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
    m_Timer.setSingleShot(true);
    setCaption(cap);
    m_pText = new Q3TextEdit(this);
    m_pText->setTextFormat(Qt::LogText);
    m_pText->setMaxLogLines(max);
    setMinimumSize(200,200);
    LoadSession(".ses");
    m_timerDelayShow.start(2000);
    QObject::connect(&m_timerDelayShow,SIGNAL(timeout()),this,SLOT(showList()));
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(saveConfiguration()));
}

CLogFileView::~CLogFileView()
{
    SaveSession(".ses");
}

void CLogFileView::ShowHideLogFileSlot(bool b)
{
    if (b)
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
    m_sessionReadWrite.writeSession(this, m_widGeometry, session);
}

bool CLogFileView::LoadSession(QString session)
{
    cWidgetGeometry tmpGeometry = m_sessionReadWrite.readSession(this, session);
    if(tmpGeometry.m_Size.isValid()) {
        m_widGeometry=tmpGeometry;
        return true;
    }
    return false;
}

void CLogFileView::resizeEvent (QResizeEvent * ) 
{
    m_pText->resize(size());
    m_Timer.start(500);
}

void CLogFileView::moveEvent(QMoveEvent *)
{
    m_Timer.start(500);
}

void CLogFileView::closeEvent (QCloseEvent* ce)
{
    m_widGeometry.SetGeometry(pos(),size());
    m_widGeometry.SetVisible(0);
    emit isVisibleSignal(false);
    m_Timer.start(500);
    ce->accept();
}   
