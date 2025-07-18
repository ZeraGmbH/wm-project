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

void wmscreenshoterguibase::startButtonPushed()
{
    ui->progressBar->setValue(1);
    this->hide();
    emit screenShooterStart();
}
