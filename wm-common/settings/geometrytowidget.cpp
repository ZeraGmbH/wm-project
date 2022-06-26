#include "geometrytowidget.h"

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
