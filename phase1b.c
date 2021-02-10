/*
Phase 1b
*/

#include "phase1Int.h"
#include "phase1a.c"
#include "usloss.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

typedef struct PCB {
    int             cid;                // context's ID
    int             cpuTime;            // process's running time
    char            name[P1_MAXNAME+1]; // process's name
    int             priority;           // process's priority
    P1_State        state;              // state of the PCB
    int             children[P1_MAXPROC];
    int             numOfChildren;
    int             parentID;
    int             lid;
    int             vid;

    // more fields here
    // who your parent is (processid)

} PCB;

static int priorityQueue1[P1_MAXPROC] = {1};
static int indexForQueue1 = 0;

static int priorityQueue2[P1_MAXPROC] = {1};
static int indexForQueue2 = 0;

static int priorityQueue3[P1_MAXPROC] = {1};
static int indexForQueue3 = 0;

static int priorityQueue4[P1_MAXPROC] = {1};
static int indexForQueue4 = 0;

static int priorityQueue5[P1_MAXPROC] = {1};
static int indexForQueue5 = 0;

static int priorityQueue6[1] = {1};
static int indexForQueue6 = 0;

static PCB processTable[P1_MAXPROC];   // the process table
static int currProcessID = 0;

void P1Handler(int dev, void *arg) {
    USLOSS_Console("Error! Illegal Instruction.\n");
    USLOSS_Halt(0);
}

void launch(void) {
    assert(contexts[currentCid].startFunc != NULL);
    P1_Quit(processTable[currentPid].startFunc(args));
}

void P1ProcInit(void) {
    P1ContextInit();
    for (int i = 0; i < P1_MAXPROC; i++) {
        processTable[i].cid = i;
        processTable[i].cpuTime = 0;
        processTable[i].priority = 0;
        processTable[i].state = P1_STATE_FREE;
        processTable[i].numOfChildren = 0;
        processTable[i].parentID = -1;
        processTable[i].lid = 0;
        processTable[i].vid = 0;

        // initialize the rest of the PCB
        // when allocating a new control block you need to see whats free
    }
    // initialize everything else

}

int P1_GetPid(void) {
    return currProcessID;
}

int P1_Fork(char *name, int (*func)(void*), void *arg, int stacksize, int priority, int *pid ) {
    int result = P1_SUCCESS;

    // check for kernel mode
    // disable interrupts
    // check all parameters
    // create a context using P1ContextCreate
    // allocate and initialize PCB
    // if this is the first process or this process's priority is higher than the 
    //    currently running process call P1Dispatch(FALSE)
    // re-enable interrupts if they were previously enabled
    return result;
}

void P1_Quit(int status) {
    // check for kernel mode
    // disable interrupts
    // remove from ready queue, set status to P1_STATE_QUIT
    // if first process verify it doesn't have children, otherwise give children to first process
    // add ourself to list of our parent's children that have quit
    // if parent is in state P1_STATE_JOINING set its state to P1_STATE_READY
    P1Dispatch(FALSE);
    // should never get here
    assert(0);
}


int P1GetChildStatus(int *cpid, int *status) {
    int result = P1_SUCCESS;
    // do stuff here
    return result;
}

// LOOK AT PHASE1.H LINE 51
// PROCESS CONTROL BLOCK CAN BE IN 1 OF 6 STATES AND IT CAN TRANSITION.
// WHAT SHOULD WE DO IF A SPECIFIC TRANSITION OCCURS.
// IF ITS RUNNING STATE AND IT CALLS QUIT, THEN IT WILL GO QUIT
int P1SetState(int pid, P1_State state, int lid, int vid) {

    int result = P1_SUCCESS;
    int status;
    int cpid;
    // do stuff here

    if ((USLOSS_PSR_CURRENT_MODE & USLOSS_PsrGet()) == 0) {
        USLOSS_IllegalInstruction();
    }

    if ((state != P1_STATE_READY) || (state != P1_STATE_BLOCKED) || (state != P1_STATE_JOINING) || (state != P1_STATE_QUIT)) {
        result = P1_INVALID_STATE;
        return result;
    }
    else {
        if (pid < P1_MAXPROC && pid >= 0) {
            for (int i = 0; i < P1_MAXPROC; i++) {
                if (processTable[i].cid == pid) {
                    processTable[i].state = state;
                }
            }
        }
        else {
            result = P1_INVALID_STATE;
            return result;
        }
        return result;
    }

}

void P1Dispatch(int rotate) {
    // select the highest-priority runnable process
    // call P1ContextSwitch to switch to that process

    status = USLOSS_DeviceInput(USLOSS_CLOCK_DEV, 0, &start);
    if (status != USLOSS_DEV_OK) {
        USLOSS_Halt(status);
    }

    P1ContextSwitch(cid);
}

int P1_GetProcInfo(int pid, P1_ProcInfo *info) {
    int         result = P1_SUCCESS;

    if ((USLOSS_PSR_CURRENT_MODE & USLOSS_PsrGet()) == 0) {
        USLOSS_IllegalInstruction();
    }

    if (pid >= 0 && pid < P1_MAXPROC) {
        if (processTable[pid].state != P1_STATE_FREE) {
            info->cpu = processTable[pid].cpuTime;
            info->priority = processTable[pid].priority;
            info->state = processTable[pid].state;
            info->numChildren = processTable[pid].numOfChildren;
            info->lid = processTable[pid].lid;
            info->vid = processTable[pid].vid;
            info->name = processTable[pid].name;
            info->children = processTable[pid].children;

            return result;
        }
    }
    result = P1_INVALID_PID;
    return result;
}

