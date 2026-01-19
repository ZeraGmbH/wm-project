#include "settingschangetimer.h"

SettingsChangeTimer::SettingsChangeTimer(int geomTimerMs)
{
    m_settingsChangeTimer.setInterval(geomTimerMs);
    m_settingsChangeTimer.setSingleShot(false);
    connect(&m_settingsChangeTimer, SIGNAL(timeout()), this, SLOT(onGeomTimer()));
    m_dontStop = false;
}

void SettingsChangeTimer::startDelayed(bool dontStop)
{
    m_settingsChangeTimer.start();
    m_dontStop = dontStop;
}

void SettingsChangeTimer::onGeomTimer()
{
    if (!m_dontStop)
        m_settingsChangeTimer.stop();
    emit sigWriteStreamForGeomChange();
}

void SettingsChangeTimer::onGeomTimer_()
{
    emit sigWriteStreamForGeomChange();
}
