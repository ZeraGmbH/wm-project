#ifndef SCPIQUESTIONSTATES_H
#define SCPIQUESTIONSTATES_H

enum QuestionStates { QuestServerProblems = 0x100,
               QuestOverLoad = 0x200,
               QuestOverLoadMax = 0x400,
               QuestNotJustified = 0x800,
               QuestFrequency = 0x1000
             };

#endif // SCPIQUESTIONSTATES_H
