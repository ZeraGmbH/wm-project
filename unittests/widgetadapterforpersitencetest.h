#ifndef WIDGETADAPTERFORPERSITENCETEST_H
#define WIDGETADAPTERFORPERSITENCETEST_H

#include "iwidgetadapter.h"
#include <QString>

class WidgetAdapterForPersitenceTest : public IWidgetAdapter
{
public:
    WidgetAdapterForPersitenceTest();

    bool isVisible() const override;
    void show() override;
    void hide() override;
    QPoint pos() const override;
    void move(const QPoint &to) override;

    QSize size() const override;
    void resize(const QSize &size) const override;

    QString objectName() const override;
    void setObjectName(const QString &name);
private:
    QPoint m_pos;
    QSize m_size;
};

#endif // WIDGETADAPTERFORPERSITENCETEST_H
