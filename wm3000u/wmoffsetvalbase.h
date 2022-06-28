#ifndef WMOFFSETVALBASE_H
#define WMOFFSETVALBASE_H

#include "widgetgeometry.h"
#include "confdata.h"
#include "wmuoffsetcustomlabels.h"
#include <justValues.h>
#include <QDialog>
#include <QTimer>
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
private:
    void actualizeDisplay(Ui::WMOffsetValBase* ui, cConfData* conf, tJustValues* just);
    Ui::WMOffsetValBase *ui;
    tJustValues m_JustValues;
    WidgetGeometry m_widGeometry;
    cConfData m_ConfData;
    QTimer m_Timer;
    IWmOffsetCustomLabels *m_customLabels;
};

#endif // WMOFFSETVALBASE_H
