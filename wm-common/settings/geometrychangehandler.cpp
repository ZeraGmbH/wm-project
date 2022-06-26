#include "geometrychangehandler.h"

GeometryChangeHandler::GeometryChangeHandler(int geomTimerMs)
{
    m_geomChangeTimer.setInterval(geomTimerMs);
    m_geomChangeTimer.setSingleShot(false);
    connect(&m_geomChangeTimer, SIGNAL(timeout()), this, SLOT(OnGeomTimer()));
}

void GeometryChangeHandler::handleSizeChange(QSize size)
{
    m_geometry.setSize(size);
    m_geomChangeTimer.start();
}

void GeometryChangeHandler::handlePointChange(QPoint pt)
{
    m_geometry.setPoint(pt);
    m_geomChangeTimer.start();
}

void GeometryChangeHandler::handleVisibleChange(bool visible)
{
    m_geometry.setVisible(visible);
    m_geomChangeTimer.start();
}

QDataStream& operator << (QDataStream& stream, GeometryChangeHandler& geomChangeHandler)
{
    stream << geomChangeHandler.m_geometry;
    return stream;
}

QDataStream& operator >> (QDataStream& stream, GeometryChangeHandler& geomChangeHandler)
{
    stream >> geomChangeHandler.m_geometry;
    return stream;
}

WidgetGeometry GeometryChangeHandler::getGeometry()
{
    return m_geometry;
}

void GeometryChangeHandler::setGeometry(WidgetGeometry geometry)
{
    m_geometry = geometry;
}

void GeometryChangeHandler::OnGeomTimer()
{
    m_geomChangeTimer.stop();
    emit sigNeedsStreamWrite();
}
