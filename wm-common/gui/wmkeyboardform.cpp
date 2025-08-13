#include "wmkeyboardform.h"
#include "cwmlineedit.h"
#include "qdebug.h"
#include "ui_wmkeyboardform.h"
#include <QKeyEvent>

wmKeyboardForm::wmKeyboardForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::wmKeyboardForm)
{
    ui->setupUi(this);
    setHex(HEXINPUT);
    ui->lineEdit->setDisabled(true);
    ui->lineEdit->setStyleSheet("background-color: white;  color: black");
    setWindowFlags(Qt::WindowStaysOnTopHint);
}

wmKeyboardForm::~wmKeyboardForm()
{
    delete ui;
}

void wmKeyboardForm::setHex(int mode)
{
    QString titel;
    switch (mode)
    {
    case FLOATINPUT:
        ui->pushButtonA->hide();
        ui->pushButtonB->hide();
        ui->pushButtonC->hide();
        ui->pushButtonD->hide();
        ui->pushButtonE->hide();
        ui->pushButtonF->hide();
        ui->pushButtonKomma->show();
        titel = "float keyboard";
        break;

    case NUMINPUT:
        ui->pushButtonA->hide();
        ui->pushButtonB->hide();
        ui->pushButtonC->hide();
        ui->pushButtonD->hide();
        ui->pushButtonE->hide();
        ui->pushButtonF->hide();
        ui->pushButtonKomma->hide();
        titel = "number keyboard";
        break;

    case FIXEDNUMINPUT:
        ui->pushButtonA->hide();
        ui->pushButtonB->hide();
        ui->pushButtonC->hide();
        ui->pushButtonD->hide();
        ui->pushButtonE->hide();
        ui->pushButtonF->hide();
        ui->pushButtonKomma->hide();
        titel = "keyboard";
        break;

    case HEXINPUT:
        ui->pushButtonA->show();
        ui->pushButtonB->show();
        ui->pushButtonC->show();
        ui->pushButtonD->show();
        ui->pushButtonE->show();
        ui->pushButtonF->show();
        ui->pushButtonKomma->hide();
        titel = "hex keyboard";
        break;
    }
    setWindowTitle(titel);
}

void wmKeyboardForm::setParent(cWmLineEdit *parent)
{
    mPoi = parent;
}

void wmKeyboardForm::show(const QString text)
{
    ui->lineEdit->setText(text);
    moveWindow();
    QWidget::show();
}

void wmKeyboardForm::showText(const QString text)
{
    ui->lineEdit->setText(text);
}

void wmKeyboardForm::hide()
{
    QWidget::hide();
}

void wmKeyboardForm::setAvailGeometry(const QRect desktop)
{
    mDesktop = desktop;
}

void wmKeyboardForm::setAKey(const int key)
{
    if(((key <= Qt::Key_F) && (key >= Qt::Key_0)) || (key == Qt::Key_Comma))
        postEvent(key,"");
}

void wmKeyboardForm::postEvent(const int iKey, const QString strKey)
{
    Q_UNUSED(strKey);
    if(mPoi != nullptr)
    {
        mPoi->inputFromSoftKeyBord(iKey);
        mPoi->activateWindow();
        /*
        QKeyEvent event(QEvent::KeyPress, iKey, Qt::NoModifier);
        QApplication::sendEvent(mPoi,&event);
        QKeyEvent event2nd(QEvent::KeyRelease, iKey, Qt::NoModifier);
        QApplication::sendEvent(mPoi,&event2nd);
*/
    }
}

void wmKeyboardForm::moveWindow()
{
    int width = mDesktop.width()-this->width()-10;
    int height = mDesktop.height()-this->height()-8;
    qDebug() << "keyboard" << width << height ;
    this->move(width,height);
}

void wmKeyboardForm::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if(((key <= Qt::Key_F) && (key >= Qt::Key_0)) || (key == Qt::Key_Comma))
    {
        postEvent(key,"");
    }
}

void wmKeyboardForm::on_pushButton1_clicked()
{
    postEvent(Qt::Key_1,"1");
}

void wmKeyboardForm::on_pushButton2_clicked()
{
    postEvent(Qt::Key_2,"2");
}

void wmKeyboardForm::on_pushButton3_clicked()
{
    postEvent(Qt::Key_3,"3");
}


void wmKeyboardForm::on_pushButton4_clicked()
{
    postEvent(Qt::Key_4,"4");
}


void wmKeyboardForm::on_pushButton5_clicked()
{
    postEvent(Qt::Key_5,"5");
}


void wmKeyboardForm::on_pushButton6_clicked()
{
    postEvent(Qt::Key_6,"6");
}


void wmKeyboardForm::on_pushButton7_clicked()
{
    postEvent(Qt::Key_7,"7");
}


void wmKeyboardForm::on_pushButton8_clicked()
{
    postEvent(Qt::Key_8,"8");
}


void wmKeyboardForm::on_pushButton9_clicked()
{
    postEvent(Qt::Key_9,"9");
}


void wmKeyboardForm::on_pushButton0_clicked()
{
    postEvent(Qt::Key_0,"0");
}


void wmKeyboardForm::on_pushButtonKomma_clicked()
{
    // ASCII defines
    // Comma (,) to be 44 Qt::Key_Comma 0x2c
    // Period (.) to be 46 Qt::Key_Period 0x2e
    postEvent(Qt::Key_Comma,".");
}


void wmKeyboardForm::on_pushButtonA_clicked()
{
    postEvent(Qt::Key_A,"A");
}


void wmKeyboardForm::on_pushButtonB_clicked()
{
    postEvent(Qt::Key_B,"B");
}


void wmKeyboardForm::on_pushButtonC_clicked()
{
    postEvent(Qt::Key_C,"C");
}


void wmKeyboardForm::on_pushButtonD_clicked()
{
    postEvent(Qt::Key_D,"D");
}


void wmKeyboardForm::on_pushButtonE_clicked()
{
    postEvent(Qt::Key_E,"E");
}


void wmKeyboardForm::on_pushButtonF_clicked()
{
    postEvent(Qt::Key_F,"F");
}
