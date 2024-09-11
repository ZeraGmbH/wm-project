#include "cwmlineedit.h"

cWmLineEdit::cWmLineEdit(QGroupBox *parent)
{
     mMode = HEXINPUT;
}

cWmLineEdit::cWmLineEdit(QLineEdit *parent)
    : QLineEdit{parent}
{
    mMode = HEXINPUT;
}

void cWmLineEdit::setKeyboard(wmKeyboardForm *poi)
{
    mKeyBoard = poi;
}

void cWmLineEdit::setInputMode(int mode)
{
    mMode = mode;
}

void cWmLineEdit::keyPressEvent(QKeyEvent *event)
{
    QString text;
    int key = event->key();
    if(((key <= Qt::Key_F) && (key >= Qt::Key_0)) || (key == Qt::Key_Comma)) {
        text = keyText(key);
        switch (mMode)
        {
        case HEXINPUT:
            keyPressedHex(text);
            break;

        case NUMINPUT:
            keyPressedNum(text);
            break;

        case FIXEDNUMINPUT:
            keyPressedNumFixed(text);
            break;

        case FLOATINPUT:
            keyPressedFloat(text);
            break;
        }


        mKeyBoard->show(this->text());
    }
    QLineEdit::keyPressEvent(event);
}

void cWmLineEdit::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    if (mKeyBoard != nullptr){
        mKeyBoard->setParent(this);
        mKeyBoard->setHex(mMode);
        mKeyBoard->show(this->text());
        //this->setCursorPosition(0);
        //this->setSelection(0,0);
    }
}

void cWmLineEdit::mouseReleaseEvent(QMouseEvent *)
{
    this->setCursorPosition(0);
    this->setSelection(0,0);
    if ((mMode == FLOATINPUT) || (mMode == FIXEDNUMINPUT))
        this->setText("");
}

void cWmLineEdit::keyPressedHex(QString str)
{
    QString text;
    int len;
    text = str;
    len = inputMask().length();
    text = text.prepend(this->text());
    text = text.mid(1,len);
    setText(text);

}

void cWmLineEdit::keyPressedNum(QString str)
{
    QString text;
    int len;
    text = str;
    len = this->text().length();
    text = text.prepend(this->text());
    text = text.mid(1,len);
    setText(text);

}

void cWmLineEdit::keyPressedNumFixed(QString str)
{
    QString text;
    int len(5);
    text = str;
    text = text.prepend(this->text());
    text = text.mid(0,len);
    setText(text);

}

void cWmLineEdit::keyPressedFloat(QString str)
{
    QString text;
    int len(5);
    text = str;
    text = text.prepend(this->text());
    text = text.mid(0,len);
    setText(text);

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
