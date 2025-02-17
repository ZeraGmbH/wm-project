#include "versserial.h"


tVersSerial::tVersSerial()
{
    PCBSerialNr = "Unknown"; // wird ggf. später aus hardware gelesen
    PCBVersion = "Unknown";
    ReleaseVersion = "Unknown";
    PCBServer = "Unknown";
    DSPVersion = "Unknown";
    DSPServer = "Unknown";
    JDataChksum = "Unknown";
    setReleaseNr();
}

QString tVersSerial::GetReleaseVersion()
{
    return ReleaseVersion;
}

void tVersSerial::setReleaseNr()
{ // from     zera-classes/modules-common/zera-basemodule/lib/sysinfo.cpp
    QString path = "/opt/zera/conf/CHANGELOG";
    QString releaseNr = "Unknown";
    QFile file(path);
    if (file.exists()) {
        file.open(QIODevice::ReadOnly);
        QTextStream stream(&file);
        int start, end;
        QString line;
        bool releaseNrFound = false;
        do {
            line = stream.readLine();
            if ((start = line.indexOf("'release-")+1) > 0 ||
                (start = line.indexOf("'snapshot-")+1) > 0 ||
                (start = line.indexOf("'no-release-")+1) > 0) {
                end = line.indexOf("'", start);
                releaseNrFound = end > start;
                if (releaseNrFound)
                    releaseNr = line.mid(start, end-start);
            }
        } while (!line.isNull() && !(releaseNrFound));
        file.close();
    }
    ReleaseVersion = releaseNr;
}
