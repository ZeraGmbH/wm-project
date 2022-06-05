#ifndef SCPISESRBITS_H
#define SCPISESRBITS_H

enum sesrBits {   SESROperationComplete = 1,
                SESRRequestControl = 2,
                SESRQueryError = 4,
                SESRDevDepError = 8,
                SESRExecError = 16,
                SESRCommandError = 32,
                SESRUserRequest = 64,
                SESRPowerOn = 128};

#endif // SCPISESRBITS_H
