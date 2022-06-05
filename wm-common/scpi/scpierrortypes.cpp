#include "scpierrortypes.h"
#include "scpierrorindicator.h"

scpiErrorType SCPIError[scpiLastError] = {  {0,(char*)"No error"},
                                            {-100,(char*)"Command error"},
                                            {-103,(char*)"Invalid separator"},
                                            {-108,(char*)"Parameter not allowed"},
                                            {-109,(char*)"Missing parameter"},
                                            {-113,(char*)"Undefined header"},
                                            {-120,(char*)"Numeric data error"},

                                            {-200,(char*)"Execution error"},
                                            {-203,(char*)"Command protected"},
                                            {-240,(char*)"Hardware error"},
                                            {-241,(char*)"Hardware missing"},
                                            {-257,(char*)"File name error"},

                                            {-300,(char*)"Device-specific error"},
                                            {-310,(char*)"System error"},
                                            {-350,(char*)"Queue overflow"},

                                            {-400,(char*)"Query error"},

                                            {-500,(char*)"Power on"} };

