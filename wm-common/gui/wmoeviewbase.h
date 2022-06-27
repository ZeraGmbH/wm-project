#ifndef WMOEVIEWBASE_H
#define WMOEVIEWBASE_H

#include "ownerrorviewdata.h"
#include "widgetgeometry.h"
#include "geometrychangetimer.h"
#include "sessionreadwrite.h"
#include <QDialog>
#include <QTimer>

namespace Ui {
    class WMOeViewBase;
}

class WMOeViewBase : public QDialog
{
    Q_OBJECT

public:
    explicit WMOeViewBase(QWidget* parent, QString machineName);
    ~WMOeViewBase();
public slots:
    virtual void ReceiveOEViewDataSlot(cOwnErrorViewData *oe);
    virtual void onShowHide(bool shw);
    void onSaveSession(QString session);
    bool onLoadSession(QString session);
signals:
    void sigIsVisible(bool);
protected:
    virtual void closeEvent(QCloseEvent *ce) override;
    virtual void resizeEvent (QResizeEvent*) override;
    virtual void moveEvent(QMoveEvent*) override;
private slots:
    void onSaveConfig();
    void onWriteStreamForGeomChange();
private:
    Ui::WMOeViewBase *ui;
    cOwnErrorViewData m_OwnErrorView;
    GeometryChangeTimer m_geomChangeTimer;
    WidgetGeometry m_geomToFromStream;
    SessionReadWrite m_sessionReadWrite;
};

#endif // WMOEVIEWBASE_H
