#ifndef WMKEYBOARDFORM_H
#define WMKEYBOARDFORM_H

#include <QWidget>

namespace Ui {
class wmKeyboardForm;
}

class wmKeyboardForm : public QWidget
{
    Q_OBJECT

public:
    explicit wmKeyboardForm(QWidget *parent = nullptr);
    ~wmKeyboardForm();
    void setHex(bool show);
    void setParent(QWidget *parent);
private slots:
    void on_pushButton1_clicked();
    void on_pushButton2_clicked();
    void on_pushButton3_clicked();
    void on_pushButton4_clicked();
    void on_pushButton5_clicked();
    void on_pushButton6_clicked();
    void on_pushButton7_clicked();
    void on_pushButton8_clicked();
    void on_pushButton9_clicked();
    void on_pushButton0_clicked();
    void on_pushButtonKomma_clicked();
    void on_pushButtonA_clicked();
    void on_pushButtonB_clicked();
    void on_pushButtonC_clicked();
    void on_pushButtonD_clicked();
    void on_pushButtonE_clicked();
    void on_pushButtonF_clicked();

private:
    QWidget* mPoi = nullptr;
    Ui::wmKeyboardForm *ui;
    void postEvent(const int iKey, const QString strKey);
};

#endif // WMKEYBOARDFORM_H
