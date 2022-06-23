#include "widgetgeometry.h"

QDataStream& operator << (QDataStream& stream, WidgetGeometry& wg)
{
    stream <<
        wg.m_visible <<
        wg.m_Point.x() <<
        wg.m_Point.y() <<
        wg.m_Size.width() <<
        wg.m_Size.height();
    return stream;
}

QDataStream& operator >> (QDataStream& stream, WidgetGeometry& wg)
{
    stream >>
        wg.m_visible >>
        wg.m_Point.rx() >>
        wg.m_Point.ry() >>
        wg.m_Size.rwidth() >>
        wg.m_Size.rheight();
    return stream;
}

void WidgetGeometry::setGeometry(QPoint p, QSize s)
{
    m_Point = p;
    m_Size = s;
}

void WidgetGeometry::setVisible(int visible)
{
    m_visible = visible;
}

int WidgetGeometry::getVisible()
{
    return m_visible;
}

QSize WidgetGeometry::getSize()
{
    return m_Size;
}

QPoint WidgetGeometry::getPoint()
{
    return m_Point;
}
