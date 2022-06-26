#ifndef GEOMETRYTOWIDGET_H
#define GEOMETRYTOWIDGET_H

#include "widgetgeometry.h"
#include <QWidget>

void geometryToWidget(WidgetGeometry geometry, QWidget *widget)
{
    widget->hide();
    widget->resize(geometry.getSize());
    widget->move(geometry.getPoint());
    if (geometry.getVisible()) {
        widget->show();
    }
    widget->move(geometry.getPoint());
}

#endif // GEOMETRYTOWIDGET_H
