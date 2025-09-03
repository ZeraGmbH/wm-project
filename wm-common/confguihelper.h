#ifndef CONFGUIHELPER_H
#define CONFGUIHELPER_H

#include <QObject>
#include <QComboBox>

enum helperLists {
    ADIVLIST = 0,
    VDIVLIST,
    ALIST,
    VLIST
};

namespace Ui {
class ConfDialogBase;
}
class confGuiHelper
{
public:
    confGuiHelper();
    const QString &baseUnitTextCurrent(const QString & s , QComboBox *unit, QComboBox *divider);
    const QString &baseUnitTextVoltage(const QString & s , QComboBox *unit, QComboBox *divider);
    const QString &genRatioText(const QString & s, QComboBox *unit, QComboBox *divider);
    QStringList GetList(int nr);
private:
    QString m_sText;
    const QStringList m_DividerStrV = {"1",QString::fromUtf8("/\u221A2"),QString::fromUtf8("/\u221A3"),"/3"};
    const QStringList m_DividerStrA = {"1",QString::fromUtf8("/\u221A3"),"/3"};
    QStringList m_unitListA = {"mA","kA","A"};
    QStringList m_unitListV = {"mV","kV","V"};
    bool mVoltage;

};

#endif // CONFGUIHELPER_H
