#include "sessionreadwrite.h"
#include <QDir>

SessionReadWrite::SessionReadWrite(QString machineName, IStreamStrategy *streamStrategy) :
    m_SessionPath(QString("%1/.%2").arg(QDir::homePath(), machineName)),
    m_streamStrategy(streamStrategy)
{
}

SessionReadWrite::~SessionReadWrite()
{
    delete m_streamStrategy;
}

void SessionReadWrite::writeSession(QWidget *widget, cWidgetGeometry geometry, QString session)
{
    if(!QDir(m_SessionPath).exists()) {
        QDir().mkdir(m_SessionPath);
    }
    QFile file(getSessionFileName(widget, session));
    if(file.open(QIODevice::Unbuffered | QIODevice::WriteOnly)) {
        file.at(0);
        int vi = (widget->isVisible()) ? 1 : 0;
        if (vi)
            geometry.SetGeometry(widget->pos(),widget->size());
        geometry.SetVisible(vi);

        QDataStream stream( &file );
        stream << geometry;
        m_streamStrategy->write(stream);
        file.close();
    }
}

cWidgetGeometry SessionReadWrite::readSession(QWidget *widget, QString session)
{
    cWidgetGeometry geometry;
    QFile file(getSessionFileName(widget, session));
    if ( file.open( QIODevice::ReadOnly ) ) {
        QDataStream stream( &file );
        stream >> geometry;
        widget->hide();
        widget->resize(geometry.m_Size);
        widget->move(geometry.m_Point);
        if (geometry.vi) {
            widget->show();
        }
        widget->move(geometry.m_Point);
        m_streamStrategy->read(stream);
        file.close();
    }
    return geometry;
}

QString SessionReadWrite::getSessionFileName(QWidget *widget, QString session)
{
    QFileInfo fi(session);
    Q_ASSERT(widget->name() == widget->objectName());
    QString name = QString("%1/%2%3").arg(m_SessionPath).arg(widget->objectName()).arg(fi.fileName());
    return name;
}

