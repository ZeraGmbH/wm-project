#include "confguihelper.h"

confGuiHelper::confGuiHelper()
{
}

const QString &confGuiHelper::genRatioText(const QString &s, QComboBox *unit, QComboBox *divider)
{
    m_sText = s;
    m_sText += unit->text(unit->currentIndex());
    if (divider->currentIndex()!=0)
        m_sText += divider->text(divider->currentIndex());
    m_sText = m_sText.replace(QString::fromUtf8("\u221A"),"w");
    return m_sText;
}

QStringList confGuiHelper::GetList(int nr)
{
    switch (nr)
    {
    case DIVLIST:
        return m_DividerStr;
    case ALIST:
        return m_unitListA;
    case VLIST:
        return m_unitListV;
    default:
        return {"","",""};
    }
}

const QString &confGuiHelper::baseUnitTextVoltage(const QString &s, QComboBox *unit, QComboBox *divider)
{
    m_sText = s;
    divider->setCurrentText("1");
    unit->setCurrentText("V");
    foreach (QString strDiv, m_DividerStr)
    {
        if(strDiv != "1")
        {
            strDiv = strDiv.replace(QString::fromUtf8("\u221A"),"w");
            if (m_sText.contains(strDiv))
            {
                m_sText = m_sText.replace(strDiv, "");
                strDiv = strDiv.replace("w",QString::fromUtf8("\u221A"));
                divider->setCurrentText(strDiv);
            }
        }
    }

    foreach(QString strUnit, m_unitListV)
    {
        if (m_sText.contains(strUnit))
        {
            m_sText = m_sText.replace(strUnit, "");
            unit->setCurrentText(strUnit);
        }
    }
    return m_sText;
}


const QString& confGuiHelper::baseUnitTextCurrent(const QString &s , QComboBox* unit , QComboBox* divider)
{
    m_sText = s;
    divider->setCurrentText("1");
    unit->setCurrentText("A");
    foreach (QString strDiv, m_DividerStr)
    {
        if(strDiv != "1")
        {
            strDiv = strDiv.replace(QString::fromUtf8("\u221A"),"w");
            if (m_sText.contains(strDiv))
            {
                m_sText = m_sText.replace(strDiv, "");
                strDiv = strDiv.replace("w",QString::fromUtf8("\u221A"));
                divider->setCurrentText(strDiv);
            }
        }
    }
    foreach(QString strUnit, m_unitListA)
    {
        if (m_sText.contains(strUnit))
        {
            m_sText = m_sText.replace(strUnit, "");
            unit->setCurrentText(strUnit);
        }
    }
    return m_sText;
}

