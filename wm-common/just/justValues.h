#ifndef JUSTVALUES_H
#define JUSTVALUES_H

class tJustValues
{
public:
    tJustValues(){ GainCorrCh0 = 1; GainCorrCh1 = 1; PhaseCorrCh0 = 0; PhaseCorrCh1 = 0; OffsetCorrCh0 = 0; OffsetCorrCh1 = 0; OffsetCorrDevN = 0; OffsetCorrDevX = 0;}
    float GainCorrCh0;
    float GainCorrCh1;
    float PhaseCorrCh0;
    float PhaseCorrCh1;
    float OffsetCorrCh0;
    float OffsetCorrCh1;
    float OffsetCorrDevN;
    float OffsetCorrDevX;
};

#endif // JUSTVALUES_H
