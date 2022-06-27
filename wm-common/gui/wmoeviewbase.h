#ifndef WMOEVIEWBASE_H
#define WMOEVIEWBASE_H

#include "ownerrorviewdata.h"
#include "widgetgeometry.h"
#include "sessionstreamer.h"
#include "geometrychangetimer.h"
#include <QDialog>

namespace Ui {
    class WMOeViewBase;
}

class WMOeViewBase : public QDialog, public ISessionStreamImplementor
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
    virtual void readStream(QDataStream& stream) override;
    virtual void writeStream(QDataStream& stream) override;
    virtual void setDefaults() override;
    Ui::WMOeViewBase *ui;
    GeometryChangeTimer m_geomChangeTimer;
    WidgetGeometry m_geomToFromStream;
    SessionStreamer m_sessionStreamer;
};

#endif // WMOEVIEWBASE_H
