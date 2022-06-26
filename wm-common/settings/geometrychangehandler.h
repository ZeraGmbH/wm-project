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
    friend QDataStream& operator << (QDataStream& stream, GeometryChangeHandler& geomChangeHandler);
    friend QDataStream& operator >> (QDataStream& stream, GeometryChangeHandler& geomChangeHandler);
    void handleResize(QSize size);
    void handleMove(QPoint pt);
    void handleVisibleChange(bool visible);
    WidgetGeometry getGeometry();
    void setGeometry(WidgetGeometry geometry);
signals:
    void sigNeedsStreamWrite();
private slots:
    void OnGeomTimer();
private:
    WidgetGeometry m_geometry;
    QTimer m_geomChangeTimer;
};

#endif // GEOMETRYCHANGEHANDLER_H
