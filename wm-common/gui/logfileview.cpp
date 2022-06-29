#include "logfileview.h"
#include "geometrytowidget.h"
#include <QString>
#include <QTime>


CLogFileView::CLogFileView(const QString cap,
                           const long max,
                           QWidget *parent,
                           const char *wname,
                           QString machineName) :
    QDialog(parent, wname),
    m_sessionStreamer(machineName, this)
{
    setCaption(cap);
    m_pText = new Q3TextEdit(this);
    m_pText->setTextFormat(Qt::LogText);
    m_pText->setMaxLogLines(max);
    setMinimumSize(200,200);
    onLoadSession(".ses");
    m_timerDelayShow.start(2000);
    QObject::connect(&m_timerDelayShow,SIGNAL(timeout()),this,SLOT(showList()));
    connect(&m_settingsChangeTimer, SIGNAL(sigWriteStreamForGeomChange()), this, SLOT(onWriteStreamForGeomChange()));
}

void CLogFileView::onShowHide(bool shw)
{
    m_settingsChangeTimer.startDelayed();
    if (shw)
        show();
    else
        close();
}

void CLogFileView::onAddLogText(const QString& s)
{
    m_loglist.append(s);
}

void CLogFileView::showList()
{
    if (m_loglist.count()) {
        QString s = m_loglist.join("\n");
        m_loglist.clear();
        m_pText->append(s);
    }
}

void CLogFileView::onSaveConfig()
{
    onSaveSession(".ses");
}

void CLogFileView::onWriteStreamForGeomChange()
{
    m_geomToFromStream = geometryFromWidget(this);
    onSaveConfig();
}

void CLogFileView::readStream(QDataStream &stream)
{
    stream >> m_geomToFromStream;
    geometryToWidget(m_geomToFromStream, this);
}

void CLogFileView::writeStream(QDataStream &stream)
{
    stream << m_geomToFromStream;
}

void CLogFileView::setDefaults()
{
}

void CLogFileView::onSaveSession(QString session)
{
    m_sessionStreamer.writeSession(objectName(), session);
}

bool CLogFileView::onLoadSession(QString session)
{
    m_sessionStreamer.readSession(objectName(), session);
    return true;
}

void CLogFileView::resizeEvent (QResizeEvent*)
{
    m_pText->resize(size());
    m_settingsChangeTimer.startDelayed();
}

void CLogFileView::moveEvent(QMoveEvent*)
{
    m_settingsChangeTimer.startDelayed();
}

void CLogFileView::closeEvent (QCloseEvent* ce)
{
    m_settingsChangeTimer.startDelayed();
    emit sigIsVisible(false);
    ce->accept();
}
