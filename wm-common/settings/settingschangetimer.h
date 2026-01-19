#ifndef GEOMETRYCHANGEHANDLER_H
#define GEOMETRYCHANGEHANDLER_H

#include <QTimer>

class SettingsChangeTimer : public QObject
{
    Q_OBJECT
public:
    SettingsChangeTimer(int geomTimerMs = 500);
    void startDelayed(bool dontStop = false);
signals:
    void sigWriteStreamForGeomChange();
private slots:
    void onGeomTimer();
    void onGeomTimer_();
private:
    QTimer m_settingsChangeTimer;
    bool m_dontStop;
};

#endif // GEOMETRYCHANGEHANDLER_H
