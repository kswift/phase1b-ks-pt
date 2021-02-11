/*
* Author:   Pranav Talwar, Kevin Swift
* NetID:    pranavtalwar@email.arizona.edu, kevinswift@email.arizona.edu
* Phase:    1b
*/

#include "phase1Int.h"
#include "phase1a.c"
#include "usloss.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

typedef struct PCB {
    int             cid;                    // context's ID
    int             cpuTime;                // process's running time
    char            name[P1_MAXNAME+1];     // process's name
    int             priority;               // process's priority
    P1_State        state;                  // state of the PCB
    int             children[P1_MAXPROC];
    int             numOfChildren;
    int             parentID;
    int             lid;
    int             vid;

    // more fields here
    // who your parent is (processid)

} PCB;

// Initialize a queue/pointer for each process
static int priorityQueue1[P1_MAXPROC] = {-1};
static int indexForQueue1 = 0;

static int priorityQueue2[P1_MAXPROC] = {-1};
static int indexForQueue2 = 0;

static int priorityQueue3[P1_MAXPROC] = {-1};
static int indexForQueue3 = 0;

static int priorityQueue4[P1_MAXPROC] = {-1};
static int indexForQueue4 = 0;

static int priorityQueue5[P1_MAXPROC] = {-1};
static int indexForQueue5 = 0;

static int priorityQueue6[1] = {1};
static int indexForQueue6 = 0;

static PCB processTable[P1_MAXPROC];   // the process table
static int currProcessID = 0;

// interrupt handler (passing in code 1024 to P1_Quit())
void P1Handler(int dev, void *arg) {
    USLOSS_Console("Error! Illegal Instruction.\n");
    P1_Quit(1024);
    USLOSS_Halt(0);
}

// launch func
void launch(void) {
    assert(contexts[currentCid].startFunc != NULL);
    P1_Quit(processTable[currentPid].startFunc(args));
}

/*
* Initializes all of our fields in the struct, and initialize the contexts
* and interrupt handler.
*/
void P1ProcInit(void) {
    // check to see kernel mode
    if ((USLOSS_PSR_CURRENT_MODE & USLOSS_PsrGet()) == 0) {
        USLOSS_IllegalInstruction();
    }

    // call handler 
    P1ContextInit();

    // fill in struct fields
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

// gets the id of the current process
int P1_GetPid(void) {
    return currProcessID;
}

/*
* Creates a child process
*
* @return: P1_INVALID_PRIORITY, invalid parameter
* @return: P1_INVALID_STACK, stacksize is less than the minimum
* @return: P1_DUPLICATE_NAME, name in use
* @return: P1_NAME_IS_NULL, NULL
* @return: P1_NAME_TOO_LONG, name too long
* @return: P1_TOO_MANY_PROCESS, no more processes
* @return: P1_SUCCESS, success
*/
int P1_Fork(char *name, int (*func)(void*), void *arg, int stacksize, int priority, int *pid ) {
    int result = P1_SUCCESS;

    // check for kernel mode
    if ((USLOSS_PSR_CURRENT_MODE & USLOSS_PsrGet()) == 0) {
        USLOSS_IllegalInstruction();
    }

    // disable interrupts
    if ((USLOSS_PSR_CURRENT_INT & USLOSS_PsrGet()) == 2) {
        P1DisableInterrupts();
    }

    // TODO: check all parameters
    // create a context using P1ContextCreate
    // allocate and initialize PCB
    // if this is the first process or this process's priority is higher than the 
    // currently running process call P1Dispatch(FALSE)
    // re-enable interrupts if they were previously enabled
    return result;
}

/*
* Terminates the current process.
*
* @ param status int, status gets saved in P1GetChildStatus
*/
void P1_Quit(int status) {
    // check for kernel mode
    if ((USLOSS_PSR_CURRENT_MODE & USLOSS_PsrGet()) != 1) {
        USLOSS_IllegalInstruction();
    }

    // disable interrupts
    if ((USLOSS_PSR_CURRENT_MODE & USLOSS_PsrGet()) == 2) {
        P1DisableInterrupts();
    }

    // remove from ready queue, set status to P1_STATE_QUIT
    // if first process verify it doesn't have children, otherwise give children to first process
    // add ourself to list of our parent's children that have quit
    // if parent is in state P1_STATE_JOINING set its state to P1_STATE_READY
    P1Dispatch(FALSE);
    // should never get here
    assert(0);
}

/*
* Gets information about the child process that has quit. 
*
* @param *pid, int*, points to process id
* @param *status, int*, points to status
*
* @return result int*, the process id of the child 
*/
int P1GetChildStatus(int *pid, int *status) {
    int result = P1_SUCCESS;

    // check if in kernel mode
    if ((USLOSS_PSR_CURRENT_MODE & USLOSS_PsrGet()) == 0) {
        USLOSS_IllegalInstruction();
    }

    // check if process has any children, any that have or haven't quit
    if (processTable[*pid].numOfChildren == 0) {
        result = P1_NO_CHILDREN;
        return result;
    }

    // check if process has any children that have quit


    // return PID of oldest child that has quit
    // return status of child that has quit
    // remove the child from the parent's list of children that have quit
    // free context of PCB
    // do stuff here
    return result;
}

/*
* Gets the process using pid and changes the state inside the struct to the passed
* parameter state. 
*
* @param pid int, process id
* @param state P1_State, process state
* @param lid int, lock id
* @param vid int, lock value
*
* @return P1_STATE_READY, process pid is now ready 
* @return  P1_STATE_JOINING, process pid is waiting for a child to quit
* @return P1_STATE_BLOCKED, process pid is is not runnable
* @return P1_STATE_QUIT, process pid has called P1_Quit
*/
int P1SetState(int pid, P1_State state, int lid, int vid) {
    int result = P1_SUCCESS;
    int status;
    int cpid;

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

/*
* Finds the next highest priority process to run
*
* @param rotate int 1 run next process, 0 run current process
*/
void P1Dispatch(int rotate) {

    // no processes exists
    if (priorityQueue6[0] == -1 || priorityQueue5[0] == -1 || priorityQueue4[0] == -1 || priorityQueue3[0] == -1 || priorityQueue2[0] == -1 || priorityQueue1[0] == -1 ) {
        USLOSS_Console("Halting, since there is no runnable processes.");
        USLOSS_Halt(0);
    }

    // keep track of how long each process runs
    status = USLOSS_DeviceInput(USLOSS_CLOCK_DEV, 0, &start);
    if (status != USLOSS_DEV_OK) {
        USLOSS_Halt(status);
    }

    // switch to higher priority process
    P1ContextSwitch(cid);
}

/*
* Copies information about process pid into the USLOSS P1_ProcInfo struct
*
* @param pid int process id range of (0,6]
* @param info P1_ProcInfo struct
* @return P1_INVALID_PID; if the process id is invalid
* @return P1_SUCCESS; succesfully copied the process' fields
*/
int P1_GetProcInfo(int pid, P1_ProcInfo *info) {
    int result = P1_SUCCESS;

    // check for kernel mode
    if ((USLOSS_PSR_CURRENT_MODE & USLOSS_PsrGet()) == 0) {
        USLOSS_IllegalInstruction();
    }

    // copying our process table's fields
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

            // success
            return result;
        }
    }

    // invalid pid
    result = P1_INVALID_PID;
    return result;
}

