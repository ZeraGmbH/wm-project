#ifndef GEOMETRYCHANGEHANDLER_H
#define GEOMETRYCHANGEHANDLER_H

#include <QTimer>

class SettingsChangeTimer : public QObject
{
    Q_OBJECT
public:
    SettingsChangeTimer(int geomTimerMs = 500);
    void startDelayed();
signals:
    void sigWriteStreamForGeomChange();
private slots:
    void onGeomTimer();
private:
    QTimer m_settingsChangeTimer;
};

#endif // GEOMETRYCHANGEHANDLER_H
