#include "cwmlineedit.h"

cWmLineEdit::cWmLineEdit(QGroupBox *parent)
{}

cWmLineEdit::cWmLineEdit(QLineEdit *parent)
    : QLineEdit{parent}
{}

void cWmLineEdit::setKeyboard(wmKeyboardForm *poi)
{
    mKeyBoard = poi;
}

void cWmLineEdit::keyPressEvent(QKeyEvent *event)
{
    QString text;
    if(event->key() != 0 ){
        int key = event->key();
        text = keyText(key);
        text = text.prepend(this->text());
        text = text.mid(1,inputMask().length());
        setText(text);
    }
}

void cWmLineEdit::focusInEvent(QFocusEvent *event)
{
    if (mKeyBoard != nullptr){
        mKeyBoard->setParent(this);
        mKeyBoard->setHex(inputMask().contains("H"));
        mKeyBoard->show();
    }
}

void cWmLineEdit::focusOutEvent(QFocusEvent *event)
{
//    if (mKeyBoard != nullptr)
//        mKeyBoard->hide();
}

QString cWmLineEdit::keyText(int key)
{
    QString str;
    switch (key){
    case Qt::Key_1 : str = "1";
        break;
    case Qt::Key_2 : str = "2";
        break;
    case Qt::Key_3 : str = "3";
        break;
    case Qt::Key_4 : str = "4";
        break;
    case Qt::Key_5 : str = "5";
        break;
    case Qt::Key_6 : str = "6";
        break;
    case Qt::Key_7 : str = "7";
        break;
    case Qt::Key_8 : str = "8";
        break;
    case Qt::Key_9 : str = "9";
        break;
    case Qt::Key_0 : str = "0";
        break;
    case Qt::Key_A : str = "A";
        break;
    case Qt::Key_B : str = "B";
        break;
    case Qt::Key_C : str = "C";
        break;
    case Qt::Key_D : str = "D";
        break;
    case Qt::Key_E : str = "E";
        break;
    case Qt::Key_F : str = "F";
        break;
    case Qt::Key_Comma : str = ".";
        break;

    default: str = "";
    }
    return str;
}
