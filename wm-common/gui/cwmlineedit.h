#ifndef CWMLINEEDIT_H
#define CWMLINEEDIT_H

#include <QLineEdit>
#include <QGroupBox>
#include <QWidget>
#include "qevent.h"
#include "wmkeyboardform.h"

enum inputMode
{
    HEXINPUT = 0,   // hex input like nconvent settings (mac adress) lenght = inputmask lenght
    NUMINPUT,       // num input like nconvent settings              lenght = inputmask lenght
    FIXEDNUMINPUT,  // num input like nconvent settings              lenght = 5
    FLOATINPUT      // float input                                   lenght = 5
};

class cWmLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit cWmLineEdit(QGroupBox *parent = nullptr);
    explicit cWmLineEdit(QLineEdit *parent = nullptr);
    void setKeyboard(wmKeyboardForm* poi);
    void setInputMode(int mode);
    void inputFromSoftKeyBord(const int key);
signals:
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent* event) override;
    void mouseReleaseEvent(QMouseEvent *) override;

private:
    void keyPressedHex(QString str);
    void keyPressedNum(QString str);
    void keyPressedNumFixed(QString str);
    void keyPressedFloat(QString str);
    QString keyText(int key);
    wmKeyboardForm *mKeyBoard = nullptr;
    int mMode;
    QGroupBox* mGroup;
    QLineEdit* mLine;
};

#endif // CWMLINEEDIT_H
