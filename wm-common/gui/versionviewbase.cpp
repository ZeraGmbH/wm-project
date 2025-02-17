/****************************************************************************
** Form implementation generated from reading ui file 'versionviewbase.ui'
**
** Created: Mi Sep 19 14:16:00 2012
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "versionviewbase.h"
#include "ui_versionviewbase.h"

VersionsViewBase::VersionsViewBase( QWidget* parent ):
    QDialog(parent),
    ui(new Ui::VersionViewBase)
{
    ui->setupUi(this);
}

VersionsViewBase::~VersionsViewBase()
{
    delete ui;
}

void VersionsViewBase::setOptionStr(QString str)
{
    ui->Opions->setText(str);
}

void VersionsViewBase::ReceiveVersionData(tVersSerial* p)
{
    pVersion = p;
    ui->DeviceText->setText(p->DeviceName);
    ui->DeviceVersionText->setText(p->DeviceVersion);
    ui->ReleaseNumberText->setText(p->GetReleaseVersion());
    ui->SerialNrText->setText(p->PCBSerialNr);
    ui->AdjDataText->setText(p->JDataChksum);
    ui->PCBVersionText->setText( p->PCBVersion);
    ui->PCBServerText->setText(p->PCBServer);
    ui->DSPVersionText->setText(p->DSPVersion);
    ui->DSPServerText->setText( p->DSPServer);
}




void VersionsViewBase::ShowVersionSlot()
{
    show();
}
