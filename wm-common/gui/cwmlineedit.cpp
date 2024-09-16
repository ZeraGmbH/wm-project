#include "cwmlineedit.h"

cWmLineEdit::cWmLineEdit(QGroupBox *parent)
    : mGroup(parent)
{
     mMode = HEXINPUT;
}

cWmLineEdit::cWmLineEdit(QLineEdit *parent)
    : mLine(parent)
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

void cWmLineEdit::inputFromSoftKeyBord(const int key)
{
    QString text;
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
}

void cWmLineEdit::keyPressEvent(QKeyEvent *event)
{
    if (mMode == HEXINPUT || mMode == NUMINPUT)
    {
        mKeyBoard->setAKey(event->key());
    }
    else
    {
    QLineEdit::keyPressEvent(event);
    }
    mKeyBoard->showText(this->text());
}

void cWmLineEdit::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    if (mKeyBoard != nullptr){
        mKeyBoard->setParent(this);
        mKeyBoard->setHex(mMode);
        mKeyBoard->showText(this->text());
    }
}

void cWmLineEdit::mouseReleaseEvent(QMouseEvent *)
{
    this->setCursorPosition(0);

    if ((mMode == FLOATINPUT) || (mMode == FIXEDNUMINPUT))
    {
         this->setSelection(0,this->text().length());
    }
    else
    {
            this->setSelection(0,0);
    }
    mKeyBoard->show(this->text());
}

void cWmLineEdit::keyPressedHex(QString str)
{
    QString text, temp;
    int len;
    if (str != ".")
    {
        text = str;
        temp = inputMask();
        len = inputMask().length()-2;
        text = text.prepend(this->text());
        text = text.mid(1,len);
        setText(text);
    }
}

void cWmLineEdit::keyPressedNum(QString str)
{
    QString text;
    int len;
    if (str != ".")
    {
        text = str;
        len = this->text().length();
        text = text.prepend(this->text());
        text = text.mid(1,len);
        setText(text);
    }
}

void cWmLineEdit::keyPressedNumFixed(QString str)
{
    QString text;
    int len(5);
    int sel;
    sel = selectionStart();
    text = str;
    if(sel == -1)
        text = text.prepend(this->text());
    text = text.mid(0,len);
    setText(text);

}

void cWmLineEdit::keyPressedFloat(QString str)
{
    QString text;
    int len(8);
    int sel;
    sel = selectionStart();
    text = str;
    if(sel == -1)
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
    case Qt::Key_Period : str = ".";
        break;

    default: str = "";
    }
    return str;
}
