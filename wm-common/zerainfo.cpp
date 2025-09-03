// zerainfo definition

#include <qstring.h>
#include <qmessagebox.h>
#include <qpixmap.h>

#include "zerainfo.h"

cZeraInfo::cZeraInfo(int nr)
{
    setInfoStrings(nr);
    m_nr = nr;
}

bool cZeraInfo::isWM3000()
{
    return ((m_nr == wmInfoWm3000I) || (m_nr == wmInfoWm3000U));
}

void cZeraInfo::AboutZeraSlot()
{
    QMessageBox *pAboutZera=new QMessageBox(QString(tr("About ZERA")),
                              tr("<h3>About ZERA</h3>"
                              "<p><br>ZERA was founded in 1920.</p>"
                              "<p>It is a developer and manufactor of high precision<br>measurement systems for the energy market.</p>"
                              "<p>We provide powerful solutions for meter testing<br>as well as transducer testing.</p>"
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
                 m_machineInfo,
                    QMessageBox::NoIcon,
                    QMessageBox::NoButton,
                    QMessageBox::NoButton,
                    QMessageBox::NoButton);

    pAboutWM->show();
}

void cZeraInfo::setInfoStrings(int nr)
{
    switch (nr)
    {
    case 0 :
        m_machineInfoTitle = tr("About WM1000I");
        m_machineInfo = tr ("<h3>About WM1000I</h3> \
                            <p>WM1000I is the new current transducer test <br>device developed by ZERA.</p> \
                            <p>It provides abilities of testing conventional<br>transducer.</p>");
        break;
    case 1 :
        m_machineInfoTitle = tr("About WM3000I");
        m_machineInfo = tr ("<h3>About WM3000I</h3> \
                            <p>WM3000I is the new current transducer test <br>device developed by ZERA.</p> \
                            <p>It provides abilities of testing conventional<br>transducer, ECT as well as nonconventional<br>transducer using IEC 61850-9-2.</p>");
        break;
    case 2 :
        m_machineInfoTitle = tr("About WM1000U");
        m_machineInfo = tr ("<h3>About WM1000U</h3> \
                            <p>WM1000U is the new voltage transducer test <br>device developed by ZERA.</p> \
                            <p>It provides abilities of testing conventional<br>transducer.</p>");
        break;
    case 3 :
        m_machineInfoTitle = tr("About WM3000U");
        m_machineInfo = tr ("<h3>About WM3000U</h3> \
                            <p>WM3000U is the new voltage transducer test <br>device developed by ZERA.</p> \
                            <p>It provides abilities of testing conventional<br>transducer, EVT as well as nonconventional<br>transducer using IEC 61850-9-2.</p>");
        break;
    default :
        m_machineInfoTitle = tr("unknown");
        m_machineInfo = tr("unknown");
        break;

    }
}

