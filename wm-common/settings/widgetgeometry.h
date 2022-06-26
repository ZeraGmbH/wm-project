#ifndef WIDGETGEOMETRY_H
#define WIDGETGEOMETRY_H

#include <qpoint.h>
#include <qsize.h>
#include <qdatastream.h>

class WidgetGeometry
{
public:    
    friend QDataStream& operator << (QDataStream&, WidgetGeometry&);
    friend QDataStream& operator >> (QDataStream&, WidgetGeometry&);

    QSize getSize();
    void setSize(QSize s);
    QPoint getPoint();
    void setPoint(QPoint p);
    int getVisible();
    void setVisible(int visible);
    bool operator==(const WidgetGeometry &other);
private:
    QPoint m_Point;
    QSize m_Size;
    int m_visible = false;
};

#endif
