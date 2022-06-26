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

QSize WidgetGeometry::getSize()
{
    return m_Size;
}

void WidgetGeometry::setSize(QSize s)
{
    m_Size = s;
}

QPoint WidgetGeometry::getPoint()
{
    return m_Point;
}

void WidgetGeometry::setPoint(QPoint p)
{
    m_Point = p;
}

void WidgetGeometry::setVisible(int visible)
{
    m_visible = visible;
}

int WidgetGeometry::getVisible()
{
    return m_visible;
}

bool WidgetGeometry::operator==(const WidgetGeometry &other)
{
    return
        m_Point == other.m_Point &&
        m_Size == other.m_Size &&
        m_visible == other.m_visible;
}
