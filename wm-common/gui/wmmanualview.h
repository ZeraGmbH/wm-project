#ifndef WMMANUALVIEW_H
#define WMMANUALVIEW_H

#include "screenshooter.h"

#include <QWidget>
#if (QT_VERSION < 0x050000)
#include <QWebView>
#else
#include <QtWebKit/QWebElement>
#include <QtWebKit/QtWebKit>
#endif

namespace Ui {
class wmManualView;
}

class wmManualView : public QWidget
{
    Q_OBJECT

public:
    explicit wmManualView(QWidget *parent = nullptr);
    ~wmManualView();
    void setLanguage(const QString str);
    void setTyp(const QString str);
    void setConventional(bool b);
    void setDC(bool b);

    void setScreenShooter(screenshooter *poi);
public slots:
    void myExecute();
    void show();
    void takeScreenshoots();
    void takeScreenshootFinished();
private:
    Ui::wmManualView *ui;
    QString mPath, mLanguage,mTyp;
    bool mbDC, mbConventional;
    screenshooter *mScrShooter;

    void setPath();
    QString getDevice();
};

#endif // WMMANUALVIEW_H
