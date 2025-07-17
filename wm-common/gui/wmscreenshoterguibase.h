#ifndef WMSCREENSHOTERGUIBASE_H
#define WMSCREENSHOTERGUIBASE_H

#include <QDialog>

namespace Ui {
class wmscreenshoterguibase;
}

class wmscreenshoterguibase : public QDialog
{
    Q_OBJECT

public:
    explicit wmscreenshoterguibase(QWidget *parent = nullptr);
    ~wmscreenshoterguibase();
signals:
    void screenShooterStart();
public slots:
    void update(const uint nr, const QString text);
private:
    Ui::wmscreenshoterguibase *ui;
private slots:
    void startButtonPushed();
};

#endif // WMSCREENSHOTERGUIBASE_H
