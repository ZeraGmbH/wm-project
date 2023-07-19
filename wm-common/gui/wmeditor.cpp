// implementierung des wmEditor

#include <qwidget.h>
#include <qevent.h>
#include <q3popupmenu.h>
//Added by qt3to4:
#include <QCloseEvent>
#include "wmeditor.h"

wmEditor::wmEditor (QWidget* parent, const char *name)
    : QPlainTextEdit (name, parent)	{}

void wmEditor::closeEvent (QCloseEvent *e) {
    emit textMessage ( this->toPlainText() );
    e->accept();
}
