#ifndef IWIDGETADAPTER_H
#define IWIDGETADAPTER_H

#include <QSize>
#include <QPoint>

class IWidgetAdapter
{
public:
    virtual bool isVisible() const = 0;
    virtual void show() = 0;
    virtual void hide() = 0;

    virtual QPoint pos() const = 0;
    virtual void move(const QPoint &) = 0;

    virtual QSize size() const = 0;
    virtual void resize(const QSize &) const = 0;

    virtual QString objectName() const = 0;
};

#endif // IWIDGETADAPTER_H
