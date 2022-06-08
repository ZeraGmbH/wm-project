#include "sessionreadwrite.h"
#include <QDir>

SessionReadWrite::SessionReadWrite(QString machineName, SessionAppendStrategy *appendStrategy) :
    m_SessionPath(QString("%1/.%2").arg(QDir::homePath(), machineName)),
    m_appendStrategy(appendStrategy)
{
}

SessionReadWrite::~SessionReadWrite()
{
    delete m_appendStrategy;
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
        m_appendStrategy->writeSessionAppend(stream);
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
        m_appendStrategy->readSessionAppend(stream);
        file.close();
    }
    return geometry;
}

QString SessionReadWrite::getSessionFileName(QWidget *widget, QString session)
{
    QFileInfo fi(session);
    QString name = QString("%1/%2%3").arg(m_SessionPath).arg(widget->name()).arg(fi.fileName());
    return name;
}
