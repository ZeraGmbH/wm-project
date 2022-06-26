#include "geometrychangetimer.h"

GeometryChangeTimer::GeometryChangeTimer(int geomTimerMs)
{
    m_geomChangeTimer.setInterval(geomTimerMs);
    m_geomChangeTimer.setSingleShot(false);
    connect(&m_geomChangeTimer, SIGNAL(timeout()), this, SLOT(onGeomTimer()));
}

void GeometryChangeTimer::handleGeomChange()
{
    m_geomChangeTimer.start();
}

void GeometryChangeTimer::onGeomTimer()
{
    m_geomChangeTimer.stop();
    emit sigWriteStreamForGeomChange();
}
