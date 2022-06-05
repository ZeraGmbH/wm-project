#include "sessionhelper.h"
#include <QDir>

SessionHelper::SessionHelper(QString machineName) :
    m_SessionPath(QString("%1/.%2/").arg(QDir::homePath(), machineName))
{
}

void SessionHelper::writeSession(QWidget *widget, cWidgetGeometry geometry, QString session)
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
        file.close();
    }
}

cWidgetGeometry SessionHelper::readSession(QWidget *widget, QString session)
{
    cWidgetGeometry geometry;
    QFile file(getSessionFileName(widget, session));
    if ( file.open( QIODevice::ReadOnly ) ) {
        QDataStream stream( &file );
        stream >> geometry;
        file.close();
        widget->hide();
        widget->resize(geometry.m_Size);
        widget->move(geometry.m_Point);
        if (geometry.vi) {
            widget->show();
        }
        widget->move(geometry.m_Point);
    }
    return geometry;
}

QString SessionHelper::getSessionFileName(QWidget *widget, QString session)
{
    return QString("%1%2%3").arg(m_SessionPath).arg(widget->name()).arg(session);
}
