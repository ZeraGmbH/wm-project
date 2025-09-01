#include "wmmanualview.h"
#include "ui_wmmanualview.h"

wmManualView::wmManualView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::wmManualView)
{
    ui->setupUi(this);
}

void wmManualView::load(const QString adr)
{
    ui->wvIntroduction->load(QUrl(adr));
    this->show();
}

void wmManualView::myExecute()
{
    load("/home/operator/html-output/dewm3000I.html");

}

wmManualView::~wmManualView()
{
    delete ui;
}

