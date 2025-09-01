#ifndef WMMANUALVIEW_H
#define WMMANUALVIEW_H

#include <QWidget>
#include <QtWebKit/QWebElement>
#include <QtWebKit/QtWebKit>

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

public slots:
    void myExecute();

private:
    Ui::wmManualView *ui;
    QString mPath, mLanguage,mTyp;
    bool mbDC, mbConventional;

    void setPath();
    QString getDevice();
};

#endif // WMMANUALVIEW_H
