/****************************************************************************
** Form interface generated from reading ui file 'versionviewbase.ui'
**
** Created: Mi Sep 19 14:11:07 2012
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef VERSIONSVIEWBASE_H
#define VERSIONSVIEWBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include "versserial.h"

namespace Ui {
    class VersionViewBase;
}

class VersionsViewBase : public QDialog
{
    Q_OBJECT

public:
    explicit VersionsViewBase( QWidget* parent = 0);
    ~VersionsViewBase();
    void setOptionStr(QString str);

public slots:
    virtual void ReceiveVersionData( tVersSerial * p );
    virtual void ShowVersionSlot();

private:
    Ui::VersionViewBase *ui;
    tVersSerial* pVersion;

};

#endif // VERSIONSVIEWBASE_H
