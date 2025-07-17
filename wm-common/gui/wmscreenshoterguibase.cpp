#include "wmscreenshoterguibase.h"
#include "ui_wmscreenshoterguibase.h"

wmscreenshoterguibase::wmscreenshoterguibase(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::wmscreenshoterguibase)
{
    ui->setupUi(this);
    ui->progressBar->hide();
    ui->progressBar->setMaximum(19);
    ui->label->setText("");
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(startButtonPushed()));
}

wmscreenshoterguibase::~wmscreenshoterguibase()
{
    delete ui;
}

void wmscreenshoterguibase::update(const uint nr, const QString text)
{
    uint val;
    if(this->isHidden()) this->show();
    ui->progressBar->show();
    val = ui->progressBar->value();
    ui->progressBar->setValue(++val);
    if (val >18) ui->pushButton->setDisabled(false);
    ui->label->setText(text);
}

void wmscreenshoterguibase::startButtonPushed()
{
    ui->progressBar->setValue(1);
    ui->label->setText("startet");
    ui->pushButton->setDisabled(true);
    this->hide();
    emit screenShooterStart();
}
