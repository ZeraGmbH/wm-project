#ifndef WMOFFSETVALBASE_H
#define WMOFFSETVALBASE_H

#include "widgetgeometry.h"
#include "confdata.h"
#include "settingschangetimer.h"
#include "sessionstreamer.h"
#include <wmoffsetcustomlabels.h>
#include <justValues.h>
#include <QDialog>
#include <QLabel>

namespace Ui {
    class WMOffsetValBase;
}

class WMOffsetValBase : public QDialog, public ISessionStreamImplementor
{
    Q_OBJECT
public:
    explicit WMOffsetValBase(QString machineName, IWmOffsetCustomLabels *customLabels, QWidget* parent = 0);
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
    virtual void readStream(QDataStream& stream) override;
    virtual void writeStream(QDataStream& stream) override;
    virtual void setDefaults() override;
    void actualizeDisplay(Ui::WMOffsetValBase* ui, cConfData* conf, tJustValues* just);
    Ui::WMOffsetValBase *ui;
    tJustValues m_JustValues;
    WidgetGeometry m_geomToFromStream;
    cConfData m_ConfData;
    IWmOffsetCustomLabels *m_customLabels;
    SettingsChangeTimer m_settingsChangeTimer;
    SessionStreamer m_sessionStreamer;
};

#endif // WMOFFSETVALBASE_H
