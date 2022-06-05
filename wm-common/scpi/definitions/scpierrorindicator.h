#ifndef SCPIERRORINDICATOR_H
#define SCPIERRORINDICATOR_H

enum scpiErrorInd {  NoError,
                              CommandError,
          InvalidSeparator,
          ParameterNotAllowed,
          MissingParameter,
          UndefinedHeader,
          NumericDataError,

          ExecutionError,
          CommandProtected,
          HardwareError,
          HardwareMissing,
          FileNameError,

          DeviceSpecificError,
          systemError,
          QueueOverflow,
          QueryError,
          PowerOn,
          scpiLastError};


#endif // SCPIERRORINDICATOR_H
