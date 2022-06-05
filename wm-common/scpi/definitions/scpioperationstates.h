#ifndef SCPIOPERATIONSTATES_H
#define SCPIOPERATIONSTATES_H

enum OperationStates { OperRanging = 4,
                OperMeasuring = 0x10,
                                    OperConfiguring = 0x100
              };

#endif // SCPIOPERATIONSTATES_H
