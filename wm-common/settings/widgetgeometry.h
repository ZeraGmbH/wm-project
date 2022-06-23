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
    void setGeometry(QPoint p,QSize s);
    void setVisible(int visible);
    int getVisible();
    QSize getSize();
    QPoint getPoint();
private:
    QPoint m_Point;
    QSize m_Size;
    int m_visible = false;
};

#endif
