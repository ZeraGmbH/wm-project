#include "cwmjsonexport.h"
#include "qstringlist.h"

cwmJsonExport::cwmJsonExport() {}

QMap<QString, QVariant> cwmJsonExport::addVektor(complex vector)
{
        QMap<QString, QVariant> temp;
        temp["real"] = vector.re();
        temp["imag"] = vector.im();
        return temp;
}

// === Escape special characters in strings ===
QString cwmJsonExport::escapeString(const QString &str) {
    QString escaped = str;
    escaped.replace("\\", "\\\\");   // Backslash first
    escaped.replace("\"", "\\\"");   // Quotes
    escaped.replace("\n", "\\n");    // Newline
    escaped.replace("\r", "\\r");    // Carriage return
    escaped.replace("\t", "\\t");    // Tab
    return escaped;
}

// === Convert QVariant to JSON value string ===
QString cwmJsonExport::variantToJson(const QVariant &value) {
    if (value.isNull() || !value.isValid()) {
        return "null";
    }

    switch (value.type()) {
    case QVariant::String:
        return "\"" + escapeString(value.toString()) + "\"";

    case QVariant::Double:
        // Use QString's default double formatting (sufficient for most cases)
        return QString::number(value.toDouble(), 'g', 6);

    case QVariant::Int:
        return QString::number(value.toInt());

    case QVariant::Bool:
        return value.toBool() ? "true" : "false";

    case QVariant::Map: {
        // Recursively handle nested maps
        QMap<QString, QVariant> map = value.toMap();
        QStringList pairs;
        for (QMap<QString, QVariant>::const_iterator it = map.begin();
             it != map.end(); ++it) {
            pairs.append("\"" + escapeString(it.key()) + "\": " + variantToJson(it.value()));
        }
        return "{ " + pairs.join(", ") + " }";
    }

    case QVariant::List:
    case QVariant::StringList: {
        QStringList list = value.toStringList();
        QStringList items;
        for (int i = 0; i < list.size(); ++i) {
            items.append(variantToJson(list.at(i)));
        }
        return "[ " + items.join(", ") + " ]";
    }

    default:
        // Fallback: convert to string
        return "\"" + escapeString(value.toString()) + "\"";
    }
}
