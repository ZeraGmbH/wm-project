#include "geometrychangehandler.h"

GeometryChangeHandler::GeometryChangeHandler(int geomTimerMs)
{
    m_geomChangeTimer.setInterval(geomTimerMs);
    m_geomChangeTimer.setSingleShot(false);
    connect(&m_geomChangeTimer, SIGNAL(timeout()), this, SLOT(OnGeomTimer()));
}

QDataStream& operator << (QDataStream& stream, GeometryChangeHandler& geomChangeHandler)
{
    stream << geomChangeHandler.getGeometry();
    return stream;
}

QDataStream& operator >> (QDataStream& stream, GeometryChangeHandler& geomChangeHandler)
{
    stream >> geomChangeHandler.getGeometry();
    return stream;
}

void GeometryChangeHandler::handleGeometryChange(WidgetGeometry geometry)
{
    m_geometry = geometry;
    m_geomChangeTimer.start();
}

WidgetGeometry &GeometryChangeHandler::getGeometry()
{
    return m_geometry;
}

void GeometryChangeHandler::OnGeomTimer()
{
    m_geomChangeTimer.stop();
    emit sigNeedsStreamWrite();
}
