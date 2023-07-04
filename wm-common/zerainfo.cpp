// zerainfo definition

#include <qstring.h>
#include <qmessagebox.h>
#include <qpixmap.h>

#include "zerainfo.h"

cZeraInfo::cZeraInfo(QString machineInfoTitle, QString machineInfo) :
    m_machineInfoTitle(machineInfoTitle), m_machineInfo(machineInfo)
{
}

void cZeraInfo::AboutZeraSlot()
{
    QMessageBox *pAboutZera=new QMessageBox(QString("About ZERA"),
					       tr("<h3>About ZERA</h3>"
					          "<p><br>ZERA was founded in 1920.</p>"
					          "<p>It is a developer and manufactor of high precision<br>measurement systems for the energy market.</p>"
					          "<p>We provide powerful solutions for meter testing<br>as well as transformer testing.</p>"
					          "<p>For further information please visit <tt>http://www.zera.de</tt></p>"),
					      QMessageBox::NoIcon,
					      QMessageBox::NoButton,
					      QMessageBox::NoButton,
					      QMessageBox::NoButton);
    
    pAboutZera->setIconPixmap(QPixmap("./images/zeralogo.png"));
    pAboutZera->show();
}					    


void cZeraInfo::AboutWM3000Slot()
{
    QMessageBox *pAboutWM;

    pAboutWM=new QMessageBox(m_machineInfoTitle,
                 tr(m_machineInfo),
                    QMessageBox::NoIcon,
                    QMessageBox::NoButton,
                    QMessageBox::NoButton,
                    QMessageBox::NoButton);

    pAboutWM->show();
}

