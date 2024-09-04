#ifndef CWMLINEEDIT_H
#define CWMLINEEDIT_H

#include <QLineEdit>
#include <QGroupBox>
#include <QWidget>
#include "qevent.h"
#include "wmkeyboardform.h"

class cWmLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit cWmLineEdit(QGroupBox *parent = nullptr);
    explicit cWmLineEdit(QLineEdit *parent = nullptr);
    void setKeyboard(wmKeyboardForm* poi);
signals:
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;

private:
    wmKeyboardForm *mKeyBoard = nullptr;
    QString keyText(int key);
};

#endif // CWMLINEEDIT_H
