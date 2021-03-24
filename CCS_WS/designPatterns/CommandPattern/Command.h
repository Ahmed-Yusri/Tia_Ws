/*
 * Command_AbstractClass.h
 *
 *  Created on: Oct 30, 2020
 *      Author: Ahmed Yusri Mohammed Ezzat.
 */

#ifndef DESIGNPATTERNS_COMMANDPATTERN_COMMAND_H_
#define DESIGNPATTERNS_COMMANDPATTERN_COMMAND_H_

#include "LedSys_FsmInterface.h"

#define SYSLED_COMMAND_CONSTRUCT(OBJECT_NAME, EVENT)                        SysLedCommandType OBJECT_NAME = {&SysLed_CommandObject, EVENT, NULL_PTR}

typedef void (*Command_ExecuteType)(void *);
typedef void (*Command_UnexecuteType)(void *);

struct Command
{
    Command_ExecuteType   execute;
    Command_UnexecuteType undo;
};

typedef struct SysLedCommand
{
   const struct Command *myCommand;
   const SysEventType ev;
   struct Fsm_Memento *lState;
}SysLedCommandType;

extern const struct Command SysLed_CommandObject;
#endif /* DESIGNPATTERNS_COMMANDPATTERN_COMMAND_H_ */
