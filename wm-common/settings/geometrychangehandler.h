#ifndef GEOMETRYCHANGEHANDLER_H
#define GEOMETRYCHANGEHANDLER_H

#include "widgetgeometry.h"
#include <QTimer>
#include <QDataStream>

class GeometryChangeHandler : public QObject
{
    Q_OBJECT
public:
    GeometryChangeHandler(int geomTimerMs = 500);
    void handleGeomChange();
signals:
    void sigWriteStreamForGeomChange();
private slots:
    void onGeomTimer();
private:
    QTimer m_geomChangeTimer;
};

#endif // GEOMETRYCHANGEHANDLER_H
