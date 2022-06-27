// LogFile nutzt immer die gleiche datei und subdirectory.
// LogFile hat maximale länge default 8192 (8K). 
// wenn die länge dann später neu gesetzt wird, wird das u.U.
// zuviel gelesene verworfen. Logfile verhält sich wie
// ein Fifo mit der Länge MaxLen. 

#include "logfile.h"
#include <QString>
#include <QFile>
#include <QTime>

CLogFile::CLogFile(const QString FileName, const long flen)
{
    m_sFileName = FileName;
    QFile file(FileName);
    m_nActFileLen = 0;
    if ( file.open( QIODevice::ReadOnly ) ) { // beim öffnen stringliste lesen und aktuelle länge bestimmen
        while(!file.atEnd()) {
            QString line = file.readLine().replace("\n", "");
            m_sLogLinesList.append(line);
            m_nActFileLen+=line.length();
        }
        file.close();
    }
    SetFileSizeSlot(flen); // event. nur kürzen, kann vorkommen wenn wm später mittels xml datei konfiguriert wird
}

CLogFile::~CLogFile() 
{
    QFile file(m_sFileName );
    file.remove();
    if ( file.open( QIODevice::WriteOnly ) ) {
        file.write(m_sLogLinesList.join("\n"));
        file.close();
    }
}

void CLogFile::SetFileSizeSlot(const long fs)
{
    QStringList::Iterator it = m_sLogLinesList.begin();
    m_nMaxFileLen = fs;
    // falls länger als konfiguriert zeilen rauswerfen
    while (m_nMaxFileLen < m_nActFileLen) {
        m_nActFileLen -= QString(*it).length();
        it = m_sLogLinesList.remove(it);
    }
}

void CLogFile::onAddLogText(const QString& logtext) // neuer input für logfile
{
    QString s = logtext.trimmed();
    if(!s.isEmpty()) {
        s = QTime::currentTime().toString("HH:mm:ss:zzz") + ": " + s;
        m_sLogLinesList.append(s); // an die liste hängen
        m_nActFileLen+=s.length(); // länge aktualisieren
        emit SendLogDataSignal(s); // daten weitergeben
        SetFileSizeSlot(m_nMaxFileLen); // tu so als ob wir die grösse neu gesetzt hätten
    }
}

void CLogFile::SendLogSlot() // zu initialisierungs zwecken
{
    emit SendLogDataSignal(m_sLogLinesList.join("\n"));
}
