/****************************************************************************
** Form interface generated from reading ui file 'userinteractiondialogbase.ui'
**
** Created: Mo Aug 13 18:03:14 2007
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef USERINTERACTIONDIALOG_H
#define USERINTERACTIONDIALOG_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class cBlinkTextEdit;
class QPushButton;

class UserInterActionDialog : public QDialog
{
    Q_OBJECT

public:
    UserInterActionDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~UserInterActionDialog();

    cBlinkTextEdit* BlinkTextEdit1;
    QPushButton* Button1;
    QPushButton* Button2;
    QPushButton* Button3;

protected:
    QVBoxLayout* UserInterActionDialogLayout;
    QHBoxLayout* layout32;
    QSpacerItem* spacer7;
    QSpacerItem* spacer8;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

};

#endif // USERINTERACTIONDIALOG_H
