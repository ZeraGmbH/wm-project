#include "dspsetup.h"

void DspSetup::setDspVarList(cConfData *confData, cDspIFace *DspIFace, int sampleCount)
{
    int nS = sampleCount * confData->m_nMeasPeriod;
    int schanLen = (confData->m_nMeasPeriod < 4)? 4:confData->m_nMeasPeriod;
    schanLen *= sampleCount;

    DspIFace->ClearVarLists();

    // maxima
    m_measData.MaxValData = DspIFace->GetMVHandle(""); // wir holen uns ein handle für den maximumsucher
    m_measData.MaxValData->addVarItem(cDspVar("MAXN",1,vApplication | vDspIntern));
    m_measData.MaxValData->addVarItem(cDspVar("MAXX",1,vApplication | vDspIntern));
    m_measData.MaxValData->addVarItem(cDspVar("MAXRDY",1,vApplication | vDspIntern));

    // schnelle rms messung zur lastpunkt bestimmung 1x rms gesamtsignal 1x ampl 1. grundwelle
    // erweitert jetzt auch für kanal x
    m_measData.RMSValData = DspIFace->GetMVHandle("");
    m_measData.RMSValData->addVarItem(cDspVar("FRMSN",1,vApplication | vDspIntern));
    m_measData.RMSValData->addVarItem(cDspVar("FAMPL1N",1,vApplication | vDspIntern));
    m_measData.RMSValData->addVarItem(cDspVar("FRMSX",1,vApplication | vDspIntern));
    m_measData.RMSValData->addVarItem(cDspVar("FAMPL1X",1,vApplication | vDspIntern));

    m_measData.ActValData = DspIFace->GetMVHandle(""); // wir holen uns ein handle für die istwerte daten
    //	nur dsp intern verwendete messdaten
    m_measData.ActValData->addVarItem(cDspVar("SINDEX",1,vDspIntern)); // index zur speicherung der sampledaten für die fehlermessung (variables messintervall);
    m_measData.ActValData->addVarItem(cDspVar("SINDEX2",1,vDspIntern)); // index zur speicherung der sampledaten für die schnelle lastpunktmessung (festes messintervall = 4 signalperioden);

    m_measData.ActValData->addVarItem(cDspVar("TEMP1",1,vDspIntern)); // werden nur temp. benötigt weil winkel und betrag
    m_measData.ActValData->addVarItem(cDspVar("TEMP2",1,vDspIntern)); // direkt ermittelt werden zwecks filterung

    // diese werte ab hier werden gefiltert
    m_measData.ActValData->addVarItem(cDspVar("KFKORR",1,vDspIntern)); // kreisfrequenz korrektur koeffizient
    m_measData.ActValData->addVarItem(cDspVar("RMSN",1,vDspIntern));
    m_measData.ActValData->addVarItem(cDspVar("AMPL1N",1,vDspIntern));
    m_measData.ActValData->addVarItem(cDspVar("RMSX",1,vDspIntern));
    m_measData.ActValData->addVarItem(cDspVar("AMPL1X",1,vDspIntern));
    m_measData.ActValData->addVarItem(cDspVar("DPHI",1,vDspIntern));
    // dphi=(phix-phin) - tdsync * (10*10^-9) * 2PI / (signalfreq* kfkorr) =(phix-phin) -tdsync * (2PI / (signalfreq*10^8)) * (1/kfkorr) -->

    m_measData.ActValData->addVarItem(cDspVar("FILTER",10,vDspIntern));
    // KFKORR wird separat gefiltert
    m_measData.ActValData->addVarItem(cDspVar("N",1,vDspIntern));

    // gefilterte messergebnisse
    m_measData.ActValData->addVarItem(cDspVar("KFKORRF",1,vApplication | vDspIntern)); // kreisfrequenz korrektur koeffizient
    m_measData.ActValData->addVarItem(cDspVar("RMSNF",1,vApplication | vDspIntern)); // rms wert kanal n
    m_measData.ActValData->addVarItem(cDspVar("AMPL1NF",1,vApplication | vDspIntern)); // amplitude 1. oberwelle kanal n
    m_measData.ActValData->addVarItem(cDspVar("RMSXF",1,vApplication | vDspIntern)); // rms wert kanal x
    m_measData.ActValData->addVarItem(cDspVar("AMPL1XF",1,vApplication | vDspIntern)); // amplitude 1. oberwelle kanal x
    m_measData.ActValData->addVarItem(cDspVar("DPHIF",1,vApplication | vDspIntern)); // winkel kanal x - winkel kanal n

    // nicht gefilterte messergebnisse
    m_measData.ActValData->addVarItem(cDspVar("TDSYNC",1,vApplication | vDspIntern)); // delay time pps -> 1. sample
    m_measData.ActValData->addVarItem(cDspVar("PHIN",1,vApplication | vDspIntern));
    m_measData.ActValData->addVarItem(cDspVar("PHIX",1,vApplication | vDspIntern));

    m_measData.RawValData0 = DspIFace->GetMVHandle("");
    m_measData.RawValData0->addVarItem(cDspVar("MESSSIGNAL0",nS,vApplication | vDspIntern));
    m_measData.RawValData1 = DspIFace->GetMVHandle("");
    m_measData.RawValData1->addVarItem(cDspVar("MESSSIGNAL1",nS,vApplication | vDspIntern));
    m_measData.RawValDataSinConHanning = DspIFace->GetMVHandle("");
    m_measData.RawValDataSinConHanning->addVarItem(cDspVar("SCHAN",schanLen,vApplication | vDspIntern)); // sinus, cosinus, hanning abwechselnd
    m_measData.RawValData2 = DspIFace->GetMVHandle("");
    m_measData.RawValData2->addVarItem(cDspVar("MESSSIGNAL2",4*sampleCount,vApplication | vDspIntern));
    m_measData.RawValData3 = DspIFace->GetMVHandle("");
    m_measData.RawValData3->addVarItem(cDspVar("MESSSIGNAL3",4*sampleCount,vApplication | vDspIntern));
}

void DspSetup::SetDspWMCmdList(cConfData *confData, cDspIFace *DspIFace, int sampleCount, bool bnConventional)
{
    int nMP = confData->m_nMeasPeriod;
    int nSMeas = sampleCount * nMP; // anzahl samples messperiode
    // int nSAK = nSPer * ((confData.m_nMeasPeriod - 1) >> 1); // messperiode für kreuzkorrelationsintegral
    QString s;
    DspIFace->ClearCmdList();

    DspIFace->addCycListItem( s = "STARTCHAIN(1,1,0x0100)"); // aktiv, prozessnr. (dummy),hauptkette 1 subkette 0 start
    DspIFace->addCycListItem( s = QString("CLEARN(%1,MAXN)").arg(3*nSMeas+8*sampleCount+37) ); // alle variable löschen
    DspIFace->addCycListItem( s = "SETVAL(SINDEX,0)"); // index zum speichern der samples auf anfang der puffer setzen
    DspIFace->addCycListItem( s = "SETVAL(SINDEX2,0)"); // index zum speichern der samples auf anfang der puffer setzen
    DspIFace->addCycListItem( s = "RESETSYNCPPS()"); // pps sync flagge rückstellen
    DspIFace->addCycListItem( s = "SETVAL(KFKORRF,1.0)"); // vorbesetzen filterausgang
    DspIFace->addCycListItem( s = "DEACTIVATECHAIN(1,0x0100)"); // ende prozessnr., hauptkette 1 subkette 0
    DspIFace->addCycListItem( s = "STOPCHAIN(1,0x0100)"); // ende prozessnr., hauptkette 1 subkette 0

    DspIFace->addCycListItem( s = "TESTSYNCPPSSKIPEQ()"); // falls syncimpuls pps war  aktivieren wir hauptkette 2 subkette 0
    DspIFace->addCycListItem( s = "ACTIVATECHAIN(1,0x0200)"); // aktivieren dieser kette

    // hier setzen wir nur das sampling system zurück
    DspIFace->addCycListItem( s = "STARTCHAIN(0,1,0x0200)"); // nicht aktiv, prozessnr. (dummy),hauptkette 2 subkette 0 start
    DspIFace->addCycListItem( s = "SETVAL(SINDEX,0)"); // index zum speichern der samples auf anfang der puffer setzen
    DspIFace->addCycListItem( s = "RESETSYNCPPS()"); // pps sync flagge rückstellen
    DspIFace->addCycListItem( s = "ACTIVATECHAIN(1,0x0300)"); // aktivieren der daten aufnahme
    DspIFace->addCycListItem( s = "DEACTIVATECHAIN(1,0x0200)"); // deaktivieren dieser kette
    DspIFace->addCycListItem( s = "STOPCHAIN(1,0x0200)"); // ende prozessnr., hauptkette 2 subkette 0

    // kopieren der maxima aus dsp workspace
    DspIFace->addCycListItem( s = "COPYDU(2,MAXIMUMSAMPLE,MAXN)");
    DspIFace->addCycListItem( s = "SETVAL(MAXRDY,1.0)"); // sync maxrdy setzen

    // ab hier nehmen wir die daten für die nächste messperiode auf
   //// DspIFace->addCycListItem( s = "ACTIVATECHAIN(1,0x0300)"); // aktivieren der daten aufnahme // wis hinzu wm2023wip
    DspIFace->addCycListItem( s = "STARTCHAIN(0,1,0x0300)"); // nicht aktiv, prozessnr. (dummy),hauptkette 3 subkette 0 start
    // kanal 0 (n)  samples über sindex kopieren
    DspIFace->addCycListItem( s = "COPYINDDATA(CH0,SINDEX,MESSSIGNAL0)");
    // kanal 1 (x)  samples über sindex kopieren
    DspIFace->addCycListItem( s = "COPYINDDATA(CH1,SINDEX,MESSSIGNAL1)");
    // sindex inkrementieren
    DspIFace->addCycListItem( s = "INC(SINDEX)");
    DspIFace->addCycListItem( s = QString("TESTVCSKIPLT(SINDEX,%1)").arg(nMP)); // test ob die messperiode vollständig ist
    DspIFace->addCycListItem( s = "ACTIVATECHAIN(1,0x0400)"); // aktivieren der berechnung
    DspIFace->addCycListItem( s = "STOPCHAIN(1,0x0300)"); // ende prozessnr., hauptkette 3 subkette 0


    DspIFace->addCycListItem( s = "STARTCHAIN(0,1,0x0400)"); // nicht aktiv, prozessnr. (dummy),hauptkette 4 subkette 0 start
    DspIFace->addCycListItem( s = "DEACTIVATECHAIN(1,0x0300)"); // deaktivieren der daten aufnahme
    // kanal 0 (n) bearbeiten
    //DspIFace->addCycListItem( s = "BREAK(1)"); // breakpoint wenn /taster
    //DspIFace->addCycListItem( s = QString("AUTOKORRELATION(%1,MESSSIGNAL0,KFKORR)").arg(nSAK));
    DspIFace->addCycListItem( s = "SETVAL(SINDEX,0)"); // index zum speichern der samples auf anfang der puffer setzen denn wir haben eine volle messperiode
    // alternativ zur autokorrelation verwenden wir die hardware frequenzmessung
    DspIFace->addCycListItem( s = QString("SETVAL(TEMP1,%1)").arg(confData->m_fSFreq,0,'E'));
    DspIFace->addCycListItem( s = "COPYDU(1,TMCH0,TEMP2)"); // perioden dauer messsignal in sec.
    //DspIFace->addCycListItem( s = QString("SETVAL(TEMP2,%1)").arg(0.02,0,'E')); // für test
    //DspIFace->addCycListItem( s = "BREAK(1)"); // breakpoint wenn /taster
    DspIFace->addCycListItem( s = "MULVVV(TEMP1,TEMP2,TEMP2)");
    DspIFace->addCycListItem( s = QString("SETVAL(TEMP1,%1)").arg(1.0,0,'E'));
    DspIFace->addCycListItem( s = "DIVVVV(TEMP1,TEMP2,KFKORR)");
    DspIFace->addCycListItem( s = "DIVVVV(TEMP1,TEMP2,KFKORRF)");  // keine filterung

    /*
    // filterung kfkorr
    float fRueckkopplung = 0.0; //  0 bedeutet filter aus !!
    DspIFace->addCycListItem( s = QString("SETVAL(TEMP1,%1)").arg(fRueckkopplung,0,'E'));
    DspIFace->addCycListItem( s = QString("SETVAL(TEMP2,%1)").arg(fRueckkopplung+1.0,0,'E'));
    DspIFace->addCycListItem( s = "MULVVV(KFKORRF,TEMP1,TEMP1)"); // (rekursives tiefpassfilter für kfkorrf
    DspIFace->addCycListItem( s = "ADDVVV(KFKORR,TEMP1,TEMP1)");
    DspIFace->addCycListItem( s = "DIVVVV(TEMP1,TEMP2,KFKORRF)");
    */

    DspIFace->addCycListItem( s = "COPYUD(1,KFKORRF,KREISFREQKOEFF)");
    // wir nehmen den gefilterten wert

    //DspIFace->addCycListItem( s = QString("FLATTOP(%1,SCHAN)").arg(nSMeas)); // fensterfunktion generieren
    DspIFace->addCycListItem( s = QString("HANNING(%1,SCHAN)").arg(nSMeas)); // fensterfunktion generieren
    //DspIFace->addCycListItem( s = "BREAK(1)"); // breakpoint wenn taster
    DspIFace->addCycListItem( s = QString("MULNCC(%1,SCHAN,MESSSIGNAL0)").arg(nSMeas)); // fenster funktion anwenden
    // korrigierte einheitswurzel berechnen und im bzw. re von kanal 0 bestimmen
    if (confData->m_bDCmeasurement)
        DspIFace->addCycListItem( s = QString("SINUS(0,%1,SCHAN)").arg(nSMeas)); // einheitswurzeln (sinus)
    else
        DspIFace->addCycListItem( s = QString("SINUS(1,%1,SCHAN)").arg(nSMeas)); // einheitswurzeln (sinus)
    DspIFace->addCycListItem( s = QString("MULNCC(%1,MESSSIGNAL0,SCHAN)").arg(nSMeas)); // mit signal multiplizieren
    DspIFace->addCycListItem( s = QString("INTEGRAL(%1,SCHAN,TEMP1)").arg(nSMeas)); // im = integral

    if (confData->m_bDCmeasurement)
        DspIFace->addCycListItem( s = QString("COSINUS(0,%1,SCHAN)").arg(nSMeas)); // einheitswurzeln (cosinus)
    else
        DspIFace->addCycListItem( s = QString("COSINUS(1,%1,SCHAN)").arg(nSMeas)); // einheitswurzeln (cosinus)
    DspIFace->addCycListItem( s = QString("MULNCC(%1,MESSSIGNAL0,SCHAN)").arg(nSMeas)); // mit signal multiplizieren
    DspIFace->addCycListItem( s = QString("INTEGRAL(%1,SCHAN,TEMP2)").arg(nSMeas)); // re = integral

    // amplitude grundwelle = sqr(im^2 + re^2) bzw. geometrische summe und phasenlage
    if (confData->m_bDCmeasurement)
    {   // bei dc interessiert uns eingentlich nur der realteil (temp2);
        DspIFace->addCycListItem( s = "COPYVAL(TEMP2,AMPL1N)");
        DspIFace->addCycListItem( s = "COPYVAL(TEMP1,PHIN)");
    }
    else
    {
        DspIFace->addCycListItem( s = "ADDVVG(TEMP1,TEMP2,AMPL1N)");
        // DspIFace->addCycListItem( s = "ADDVVG(TEMP1,TEMP2,FAMPL1N)");
        DspIFace->addCycListItem( s = "ARCTAN(TEMP1,TEMP2,PHIN)");
    }

    // rms wert berechnung
    DspIFace->addCycListItem( s = QString("RMSN(%1,MESSSIGNAL0,RMSN)").arg(nSMeas));
    //DspIFace->addCycListItem( s = QString("RMSN(%1,MESSSIGNAL0,FRMSN)").arg(nSMeas));
    // kanal 1 (x) bearbeiten

    DspIFace->addCycListItem( s = QString("HANNING(%1,SCHAN)").arg(nSMeas)); // fensterfunktion generieren
    DspIFace->addCycListItem( s = QString("MULNCC(%1,SCHAN,MESSSIGNAL1)").arg(nSMeas)); // fenster funktion anwenden
    // korrigierte einheitswurzel berechnen und im bzw. re von kanal 1 bestimmen
    if (confData->m_bDCmeasurement)
        DspIFace->addCycListItem( s = QString("SINUS(0,%1,SCHAN)").arg(nSMeas)); // einheitswurzeln (sinus)
    else
        DspIFace->addCycListItem( s = QString("SINUS(1,%1,SCHAN)").arg(nSMeas)); // einheitswurzeln (sinus)
    DspIFace->addCycListItem( s = QString("MULNCC(%1,MESSSIGNAL1,SCHAN)").arg(nSMeas)); // mit signal multiplizieren
    DspIFace->addCycListItem( s = QString("INTEGRAL(%1,SCHAN,TEMP1)").arg(nSMeas)); // im = integral

    if (confData->m_bDCmeasurement)
        DspIFace->addCycListItem( s = QString("COSINUS(0,%1,SCHAN)").arg(nSMeas)); // einheitswurzeln (cosinus)
    else
        DspIFace->addCycListItem( s = QString("COSINUS(1,%1,SCHAN)").arg(nSMeas)); // einheitswurzeln (cosinus)
    DspIFace->addCycListItem( s = QString("MULNCC(%1,MESSSIGNAL1,SCHAN)").arg(nSMeas)); // mit signal multiplizieren
    DspIFace->addCycListItem( s = QString("INTEGRAL(%1,SCHAN,TEMP2)").arg(nSMeas)); // re = integral

    // amplitude grundwelle = sqr(im^2 + re^2) bzw. geometrische summe und phasenlage
    if (confData->m_bDCmeasurement)
    {   // bei dc interessiert uns eingentlich nur der realteil (temp2);
        DspIFace->addCycListItem( s = "COPYVAL(TEMP2,AMPL1X)");
        DspIFace->addCycListItem( s = "COPYVAL(TEMP1,PHIX)");
    }
    else
    {
        DspIFace->addCycListItem( s = "ADDVVG(TEMP1,TEMP2,AMPL1X)");
        DspIFace->addCycListItem( s = "ARCTAN(TEMP1,TEMP2,PHIX)");
    }

    // rms wert berechnung
    DspIFace->addCycListItem( s = QString("RMSN(%1,MESSSIGNAL1,RMSX)").arg(nSMeas)); // rmswert berechnen
    // ermitteln der zeit zwischen pps und 1. samplewert
    DspIFace->addCycListItem( s = "COPYDATA(CH2,0,MESSSIGNAL0)"); // auf  kanal 2 PPS2SampleTime ... kopiert nur die ersten 80 oder 256 samples

    // kleiner kunstgriff um das 2. datum aus den sampledaten zu bekommen
    // 2.  datum wegen phasenkorrektur puffer
    DspIFace->addCycListItem( s = "SETVAL(TEMP1,0.0)");
    DspIFace->addCycListItem( s = "ADDVVV(TEMP1,MESSSIGNAL0+2,TDSYNC)");

    // falsch falsch falsch
    // dphi=(phin-phix)+ tdsync * (10*10^-9) * 2PI / (signalfreq* kfkorr)
    // richtig
    // dphi=(phin-phix)+ tdsync * (10*10^-9) * 2PI * (signalfreq* kfkorr)          			            =(phin-phix)+ tdsync * (2PI*10^8 *  signalfreq * kfkorr)

    // die phasenkorrektur ist erforderlich weil von pps bis zum 1. sample eine totzeit liegt

    float fp = 6.283185307e-8 * confData->m_fSFreq;

    DspIFace->addCycListItem( s = QString("SETVAL(TEMP1,%1)").arg(fp));
    DspIFace->addCycListItem( s = "MULVVV(TEMP1,KFKORRF,TEMP1)"); // (2PI * (signalfreq*10^-8) * kfkorr)

    DspIFace->addCycListItem( s = "MULVVV(TEMP1,TDSYNC,TEMP2)"); // tdsync * (2PI * (signalfreq*10^-8)) * kfkorr
    DspIFace->addCycListItem( s = "ADDVVV(PHIN,TEMP2,PHIN)");
    if (bnConventional) // was: if (confData->m_nMeasMode != In_nConvent)   // wenn conventional
        DspIFace->addCycListItem( s = "ADDVVV(PHIX,TEMP2,PHIX)"); // auch kanal x

    DspIFace->addCycListItem( s = "SUBVVV(PHIN,PHIX,DPHI)"); // dphi=(phin-phix)  !!! vorzeichen gedreht
    DspIFace->addCycListItem( s = "NORMVC(DPHI,6.283185307)"); // auf 2PI normieren
    DspIFace->addCycListItem( s = "SYMPHI(DPHI,DPHI)"); // symmetiert den winkel auf  -pi...+pi -> zwecks filterung
    // alle messwerte noch filtern

    DspIFace->addCycListItem( s = "AVERAGE1(5,RMSN,FILTER)");

    // wir haben nach pps nur über die eingestellte anzahl signalperioden gemessen und die werte
    // ins filter übertragen.
    DspIFace->addCycListItem( s = "ACTIVATECHAIN(1,0x0500)"); // kette 0x0500 dient zum synchronisieren mit dem ctrl programm
    DspIFace->addCycListItem( s = "DEACTIVATECHAIN(1,0x0400)"); // deaktivieren der berechnung
    DspIFace->addCycListItem( s = "STOPCHAIN(1,0x0400)"); // ende prozessnr., hauptkette 4 subkette 0

    DspIFace->addCycListItem( s = "STARTCHAIN(0,1,0x0500)"); // aktiv, prozessnr. (dummy), hauptkette 5 subkette 0 start
    DspIFace->addCycListItem( s = "STARTCHAIN(0,1,0x0501)"); // aktiv, prozessnr. (dummy), hauptkette 5 subkette 1 start
    DspIFace->addCycListItem( s = "CMPAVERAGE1(5,FILTER,RMSNF)");
    DspIFace->addCycListItem( s = "CLEARN(11,FILTER)");
    DspIFace->addCycListItem( s = "DSPINTTRIGGER(0x0,0x0001)"); // gibt interrupt an controler
    DspIFace->addCycListItem( s = "DEACTIVATECHAIN(1,0x0501)"); // deaktivieren der berechnung
    DspIFace->addCycListItem( s = "DEACTIVATECHAIN(1,0x0500)"); // deaktivieren der synch
    DspIFace->addCycListItem( s = "STOPCHAIN(1,0x0501)"); // ende prozessnr., hauptkette 5 subkette 1
    DspIFace->addCycListItem( s = "STOPCHAIN(1,0x0500)"); // ende prozessnr., hauptkette 5 subkette 0

    // kanal 0 (n)  samples über sindex2 kopieren
    DspIFace->addCycListItem( s = "COPYINDDATA(CH0,SINDEX2,MESSSIGNAL2)");
    // kanal 1 (x)  samples über sindex2 kopieren
    DspIFace->addCycListItem( s = "COPYINDDATA(CH1,SINDEX2,MESSSIGNAL3)");

    // sindex2 inkrementieren
    DspIFace->addCycListItem( s = "INC(SINDEX2)");
    DspIFace->addCycListItem( s = QString("TESTVCSKIPLT(SINDEX2,%1)").arg(4)); // test ob die messperiode vollständig ist
    DspIFace->addCycListItem( s = "ACTIVATECHAIN(1,0x0600)"); // aktivieren der berechnung

    DspIFace->addCycListItem( s = "STARTCHAIN(0,1,0x0600)"); // nicht aktiv, prozessnr. (dummy),hauptkette 6 subkette 0 start
    DspIFace->addCycListItem( s = "SETVAL(SINDEX2,0)"); // index 0 setzen
    DspIFace->addCycListItem( s = QString("HANNING(%1,SCHAN)").arg(4*sampleCount)); // fensterfunktion über 4 signalperioden generieren
    DspIFace->addCycListItem( s = QString("MULNCC(%1,SCHAN,MESSSIGNAL2)").arg(4*sampleCount)); // fenster funktion anwenden
    DspIFace->addCycListItem( s = QString("RMSN(%1,MESSSIGNAL2,FRMSN)").arg(4*sampleCount)); // die schnelle rms messung für kanal n
    DspIFace->addCycListItem( s = QString("MULNCC(%1,SCHAN,MESSSIGNAL3)").arg(4*sampleCount)); // fenster funktion anwenden
    DspIFace->addCycListItem( s = QString("RMSN(%1,MESSSIGNAL3,FRMSX)").arg(4*sampleCount)); // die schnelle rms messung für kanal x

    if (confData->m_bDCmeasurement)
        DspIFace->addCycListItem( s = QString("SINUS(0,%1,SCHAN)").arg(4*sampleCount)); // einheitswurzeln (sinus)
    else
        DspIFace->addCycListItem( s = QString("SINUS(1,%1,SCHAN)").arg(4*sampleCount)); // einheitswurzeln (sinus)
    DspIFace->addCycListItem( s = QString("MULNCC(%1,MESSSIGNAL2,SCHAN)").arg(4*sampleCount)); // mit signal multiplizieren
    DspIFace->addCycListItem( s = QString("INTEGRAL(%1,SCHAN,TEMP1)").arg(4*sampleCount)); // im = integral
    if (confData->m_bDCmeasurement)
        DspIFace->addCycListItem( s = QString("COSINUS(0,%1,SCHAN)").arg(4*sampleCount)); // einheitswurzeln (cosinus)
    else
        DspIFace->addCycListItem( s = QString("COSINUS(1,%1,SCHAN)").arg(4*sampleCount)); // einheitswurzeln (cosinus)
    DspIFace->addCycListItem( s = QString("MULNCC(%1,MESSSIGNAL2,SCHAN)").arg(4*sampleCount)); // mit signal multiplizieren
    DspIFace->addCycListItem( s = QString("INTEGRAL(%1,SCHAN,TEMP2)").arg(4*sampleCount)); // re = integral
    // amplitude grundwelle = sqr(im^2 + re^2) bzw. geometrische summe und phasenlage
    DspIFace->addCycListItem( s = "ADDVVG(TEMP1,TEMP2,FAMPL1N)"); // für schnelle lp anzeige

    if (confData->m_bDCmeasurement)
        DspIFace->addCycListItem( s = QString("SINUS(0,%1,SCHAN)").arg(4*sampleCount)); // einheitswurzeln (sinus)
    else
        DspIFace->addCycListItem( s = QString("SINUS(1,%1,SCHAN)").arg(4*sampleCount)); // einheitswurzeln (sinus)
    DspIFace->addCycListItem( s = QString("MULNCC(%1,MESSSIGNAL3,SCHAN)").arg(4*sampleCount)); // mit signal multiplizieren
    DspIFace->addCycListItem( s = QString("INTEGRAL(%1,SCHAN,TEMP1)").arg(4*sampleCount)); // im = integral
    if (confData->m_bDCmeasurement)
        DspIFace->addCycListItem( s = QString("COSINUS(0,%1,SCHAN)").arg(4*sampleCount)); // einheitswurzeln (cosinus)
    else
        DspIFace->addCycListItem( s = QString("COSINUS(1,%1,SCHAN)").arg(4*sampleCount)); // einheitswurzeln (cosinus)
    DspIFace->addCycListItem( s = QString("MULNCC(%1,MESSSIGNAL3,SCHAN)").arg(4*sampleCount)); // mit signal multiplizieren
    DspIFace->addCycListItem( s = QString("INTEGRAL(%1,SCHAN,TEMP2)").arg(4*sampleCount)); // re = integral
    // amplitude grundwelle = sqr(im^2 + re^2) bzw. geometrische summe und phasenlage
    DspIFace->addCycListItem( s = "ADDVVG(TEMP1,TEMP2,FAMPL1X)"); // für schnelle lp anzeige

    DspIFace->addCycListItem( s = "DEACTIVATECHAIN(1,0x0600)"); // deaktivieren der berechnung
    DspIFace->addCycListItem( s = "STOPCHAIN(1,0x0600)"); // ende prozessnr., hauptkette 4 subkette 0

    // ab hier interrupt befehlskette
    // ------------------------------

    // die ergebnisse werden aus der appl. heraus mittels kommando (bearbeiten int cmd list) generiert
    DspIFace->addIntListItem( s = "STARTCHAIN(1,1,0x0001)"); // aktiv, prozessnr. (dummy), hauptkette 0 subkette 1 start
    DspIFace->addIntListItem( s = "ACTIVATECHAIN(1,0x0501)"); // aktivieren der berechnung des filters
    DspIFace->addIntListItem( s = "STOPCHAIN(1,0x0001)"); // ende prozessnr., hauptkette 0 subkette 1

    // rücksetzen der maxima wie oben per int cmd list mit angabe der subketten nr.
    DspIFace->addIntListItem( s = "STARTCHAIN(1,1,0x0002)"); // aktiv, prozessnr.(dummy), hauptkette 0 subkette 2 start
    DspIFace->addIntListItem( s = "SETVAL(MAXX,0.0)");
    DspIFace->addIntListItem( s = "SETVAL(MAXN,0.0)");
    DspIFace->addIntListItem( s = "SETVAL(MAXRDY,0.0)");
    //    DspIFace->addIntListItem( s = "DSPINTTRIGGER(0x0,0x0002)"); // gibt interrupt an controler
    DspIFace->addIntListItem( s = "STOPCHAIN(1,0x0002)"); // ende prozessnr., hauptkette 0 subkette 2

    DspIFace->addIntListItem( s = "STARTCHAIN(1,1,0x0003)"); // aktiv, prozessnr. (dummy), hauptkette 0 subkette 3 start
    DspIFace->addIntListItem( s = "CLEARN(11,FILTER)");
    DspIFace->addIntListItem( s = "DSPINTTRIGGER(0x0,0x0003)"); // gibt interrupt an controler
    DspIFace->addIntListItem( s = "STOPCHAIN(1,0x0003)"); // ende prozessnr., hauptkette 0 subkette 3
}

void DspSetup::setDspMemList(cDspIFace* DspIFace)
{
    DspIFace->ClearMemLists();
    m_ethData.ETHStatusHandle = DspIFace->GetMemHandle(""); // wir holen uns ein memory handle
    m_ethData.ETHStatusHandle->addVarItem(cDspVar("ETHDATACOUNT",2,vMemory));
    m_ethData.ETHStatusHandle->addVarItem(cDspVar("ETHERRORS",1,vMemory));
    m_ethData.ETHStatusHandle->addVarItem(cDspVar("ETHSYNCLOSTCOUNT",1,vMemory));

    m_ethData.ETHStatusResetHandle = DspIFace->GetMemHandle(""); // wir holen uns ein memory handle
    m_ethData.ETHStatusResetHandle->addVarItem(cDspVar("ETHERRORS",1,vMemory));
    m_ethData.ETHStatusResetHandle->addVarItem(cDspVar("ETHSYNCLOSTCOUNT",1,vMemory));

    m_ethData.ETHStatusHandle->reset();
    m_ethData.ETHStatusResetHandle->reset();
}

EthDataStruct *DspSetup::getEthData()
{
    return &m_ethData;
}


MeasDataStruct *DspSetup::getMeasData()
{
    return &m_measData;
}
