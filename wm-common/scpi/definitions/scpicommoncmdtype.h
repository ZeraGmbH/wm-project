#ifndef SCPICOMMONCMDTYPE_H
#define SCPICOMMONCMDTYPE_H

enum CommonCmdType  {	nixCmd, // 0 is nix

            // alle common commands

                                          IdentQuery, // *IDN?
            ResetCmd, // *RST
            SelfTestQuery, // *TST?
            OperationCompleteCmd, // *OPC
            OperationCompleteQuery, // *OPC?
            ClearStatusCmd, // *CLS
            StdEventStatEnableCmd, // *ESE
            StdEventStatEnableQuery, // *ESE?
            StdEventStatRegQuery, // *ESR?
            ServiceRequestEnableCmd, // *SRE
            ServiceRequestEnableQuery, // *SRE?
            StatusByteQuery, // *STB?
            LastCommonCommand // 13
              };

#endif // SCPICOMMONCMDTYPE_H
