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
    void load(const QString adr);
    ~wmManualView();

    void myExecute();
private:
    Ui::wmManualView *ui;
};

#endif // WMMANUALVIEW_H
