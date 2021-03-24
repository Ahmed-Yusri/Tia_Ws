/*
 * Fsm_InternalStateAbstractClass.h
 *
 *  Created on: Oct 27, 2020
 *      Author: ahmed
 */

#ifndef STATEPATTERN_EXAMPLE_FSM_INTERNALSTATE_H_
#define STATEPATTERN_EXAMPLE_FSM_INTERNALSTATE_H_

#include "Std_Types.h"
#include "LedSys_FsmInterface.h"

typedef struct Fsm_Memento Fsm_MementoType;
typedef boolean (*Fsm_enterEventType)(void);
typedef boolean (*Fsm_exitEventType)(void);
typedef void (*Fsm_newEventType)(SysEventType);
typedef const struct internalStateCtx *(*Fsm_cyclicEventType)(void);
typedef void (*Fsm_createMementoType)(Fsm_MementoType *);

struct internalStateCtx
{
   Fsm_enterEventType enterEv;
   Fsm_exitEventType exitEv;
   Fsm_newEventType newEv;
   Fsm_cyclicEventType cyclicEv;
   Fsm_createMementoType createMemento;
};

struct internalFsmState
{
   const struct internalStateCtx *nextCtx;
};

typedef const struct internalStateCtx *internalStateCtxRefType;


extern const struct internalStateCtx Fsm_IdleState;
extern const struct internalStateCtx Fsm_RightState;
extern const struct internalStateCtx Fsm_LeftState;

#endif /* STATEPATTERN_EXAMPLE_FSM_INTERNALSTATE_H_ */
