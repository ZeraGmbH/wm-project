// headerdatei userinteraction.h
// definiert cUserInterAction: public QDialog
// definiert  cBlinkText: QTextEdit


#ifndef USERINTERACTION_H
#define USERINTERACTION_H

#include <qobject.h>
#include <qstring.h>
#include <qtextedit.h>
#include <qdialog.h>
#include <qevent.h>

/*
class cBlinkText: public QTextEdit
{
    Q_OBJECT

};

class cUserInterAction: public QDialog
{
    Q_OBJECT
    
public:
    cUserInterAction(const QString);
    cBlinkText *m_pText;
 
public slots:    
    void ShowHideLogFileSlot(bool); // sichtbar oder nicht
    void AddLogTextSlot(const QString); // wenn neue daten einlaufen
    
protected:
    virtual void showEvent (QShowEvent *);
    virtual void hideEvent ( QHideEvent * );
    virtual void resizeEvent ( QResizeEvent *);
    
signals:
    void isVisibleSignal(bool);    
    void SendLogFileSignal();
*/
};

#endif
