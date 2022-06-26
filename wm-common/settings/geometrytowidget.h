#ifndef GEOMETRYTOWIDGET_H
#define GEOMETRYTOWIDGET_H

#include "widgetgeometry.h"
#include <QWidget>
#include <QTimer>

void geometryToWidget(WidgetGeometry geometry, QWidget *widget);
WidgetGeometry geometryFromWidget(QWidget *widget);

#endif // GEOMETRYTOWIDGET_H
