#ifndef QWIDGETADAPTER_H
#define QWIDGETADAPTER_H

#include "iwidgetadapter.h"
#include <QWidget>

class QWidgetAdapter : public IWidgetAdapter
{
public:
    QWidgetAdapter(QWidget* realSubject) : m_realSubject(realSubject) {}

    bool isVisible() const override { return m_realSubject->isVisible(); }
    QPoint pos() const override { return m_realSubject->pos(); }
    void move(const QPoint &to) override { m_realSubject->move(to); }
    QSize size() const override { return m_realSubject->size(); }
    void resize(const QSize &size) const override { m_realSubject->resize(size); }
    void show() override { m_realSubject->show(); }
    void hide() override { m_realSubject->hide(); }

    QString objectName() const override { return m_realSubject->objectName(); }

private:
    QWidget* m_realSubject;
};

#endif // QWIDGETADAPTER_H
