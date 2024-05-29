#ifndef CWMESSAGEBOX2_H
#define CWMESSAGEBOX2_H

#include <QObject>
#include <QMessageBox>

class cWMessageBox: public QMessageBox
{
    Q_OBJECT

public:
    cWMessageBox();
    cWMessageBox ( const QString & caption, const QString & text, Icon icon, int button0, int button1, int button2, QWidget * parent = 0, const char * name = 0, bool modal = TRUE, Qt::WFlags f = Qt::WStyle_DialogBorder );
    virtual ~cWMessageBox();
    bool analyseOvld(bool bOvld, bool bOldOvld);
    int count();
protected slots:
    virtual void done(int r);

signals:
    void WMBoxClosed();
private:
    int mOVL;
    void resetOVL();
    void countOVL();
    bool OVLD();

};

#endif // CWMESSAGEBOX_H
