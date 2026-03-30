#ifndef CWMJSONEXPORT_H
#define CWMJSONEXPORT_H

#include "dspvar.h"
#include "complex.h"
#include <qmap.h>
#include "qvariant.h"

class cwmJsonExport
{
public:
    cwmJsonExport();
    QMap<QString, QVariant> addVektor(complex vector);
    QString escapeString(const QString &str);
    QString variantToJson(const QVariant &value);
};

#endif // CWMJSONEXPORT_H
