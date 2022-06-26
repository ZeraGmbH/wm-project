#include "geometrychangehandler.h"

GeometryChangeHandler::GeometryChangeHandler(int geomTimerMs)
{
    m_geomChangeTimer.setInterval(geomTimerMs);
    m_geomChangeTimer.setSingleShot(false);
    connect(&m_geomChangeTimer, SIGNAL(timeout()), this, SLOT(onGeomTimer()));
}

void GeometryChangeHandler::handleGeomChange()
{
    m_geomChangeTimer.start();
}

void GeometryChangeHandler::onGeomTimer()
{
    m_geomChangeTimer.stop();
    emit sigWriteStreamForGeomChange();
}
