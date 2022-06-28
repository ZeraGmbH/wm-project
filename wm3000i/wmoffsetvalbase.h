#ifndef WMOFFSETVALBASE_H
#define WMOFFSETVALBASE_H

#include "widgetgeometry.h"
#include "confdata.h"
#include "geometrychangetimer.h"
#include <wmoffsetcustomlabels.h>
#include <justValues.h>
#include <QDialog>
#include <QLabel>

namespace Ui {
    class WMOffsetValBase;
}

class WMOffsetValBase : public QDialog
{
    Q_OBJECT
public:
    explicit WMOffsetValBase(IWmOffsetCustomLabels *customLabels, QWidget* parent = 0);
    ~WMOffsetValBase();
public slots:
    virtual void onShowHide(bool shw);
    virtual void ReceiveJustDataSlot(tJustValues *JustValues);
    virtual void SetConfInfoSlot(cConfData *cd);
    bool onLoadSession(QString session);
    void onSaveSession(QString session);
signals:
    void sigIsVisible(bool);
protected:
    virtual void closeEvent(QCloseEvent *ce) override;
    virtual void resizeEvent(QResizeEvent *) override;
    virtual void moveEvent(QMoveEvent *) override;
private slots:
    void onSaveConfig();
    void onWriteStreamForGeomChange();
private:
    void actualizeDisplay(Ui::WMOffsetValBase* ui, cConfData* conf, tJustValues* just);
    Ui::WMOffsetValBase *ui;
    tJustValues m_JustValues;
    WidgetGeometry m_geomToFromStream;
    cConfData m_ConfData;
    IWmOffsetCustomLabels *m_customLabels;
    GeometryChangeTimer m_geomChangeTimer;
};

#endif // WMOFFSETVALBASE_H
