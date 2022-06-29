#include "settingschangetimer.h"

SettingsChangeTimer::SettingsChangeTimer(int geomTimerMs)
{
    m_settingsChangeTimer.setInterval(geomTimerMs);
    m_settingsChangeTimer.setSingleShot(false);
    connect(&m_settingsChangeTimer, SIGNAL(timeout()), this, SLOT(onGeomTimer()));
}

void SettingsChangeTimer::startDelayed()
{
    m_settingsChangeTimer.start();
}

void SettingsChangeTimer::onGeomTimer()
{
    m_settingsChangeTimer.stop();
    emit sigWriteStreamForGeomChange();
}
