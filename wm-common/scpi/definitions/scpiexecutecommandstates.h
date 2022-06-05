#ifndef SCPIEXECUTECOMMANDSTATES_H
#define SCPIEXECUTECOMMANDSTATES_H

enum ExecuteCommandStates   { ExecCmdIdle,

                     ExecCmdStart,
                     ExecCmdPart,
                     ExecCmdPartFinished, // teikommando fehlerfrei bearbeitet
                     ExecCmdFinished, // komplettes kommando bearbeitet

                     ExecCmdContinue, // wir machen da weiter wo wir sind

                     ExecCmdLast
                 };

#endif // SCPIEXECUTECOMMANDSTATES_H
