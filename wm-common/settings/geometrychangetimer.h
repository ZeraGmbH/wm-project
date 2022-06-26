#ifndef GEOMETRYCHANGEHANDLER_H
#define GEOMETRYCHANGEHANDLER_H

#include <QTimer>

class GeometryChangeTimer : public QObject
{
    Q_OBJECT
public:
    GeometryChangeTimer(int geomTimerMs = 500);
    void handleGeomChange();
signals:
    void sigWriteStreamForGeomChange();
private slots:
    void onGeomTimer();
private:
    QTimer m_geomChangeTimer;
};

#endif // GEOMETRYCHANGEHANDLER_H
