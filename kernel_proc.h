#ifndef __KERNEL_PROC_H
#define __KERNEL_PROC_H

/**
  @file kernel_proc.h
  @brief The process table and process management.
  @defgroup proc Processes
  @ingroup kernel
  @brief The process table and process management.
  This file defines the PCB structure and basic helpers for
  process access.
  @{
*/ 

#include "tinyos.h"
#include "kernel_sched.h"
#include "kernel_streams.h"

/**
  @brief PID state
  A PID can be either free (no process is using it), ALIVE (some running process is
  using it), or ZOMBIE (a zombie process is using it).
  */
typedef enum pid_state_e {
  FREE,   /**< @brief The PID is free and available */
  ALIVE,  /**< @brief The PID is given to a process */
  ZOMBIE  /**< @brief The PID is held by a zombie */
} pid_state;

/**
  @brief Process Control Block.
  This structure holds all information pertaining to a process.
 */
typedef struct process_control_block {
  pid_state  pstate;      /**< @brief The pid state for this PCB */

  PCB* parent;            /**< @brief Parent's pcb. */
  int exitval;            /**< @brief The exit value of the process */

  /*EDITS.........*/

  rlnode thread_list;     //ptcb's lists
  uint thread_count;     //how many threads running in this process
  
  TCB* main_thread;       /**< @brief The main thread */
  Task main_task;         /**< @brief The main thread's function */
  int argl;               /**< @brief The main thread's argument length */
  void* args;             /**< @brief The main thread's argument string */

  rlnode children_list;   /**< @brief List of children */
  rlnode exited_list;     /**< @brief List of exited children */

  rlnode children_node;   /**< @brief Intrusive node for @c children_list */
  rlnode exited_node;     /**< @brief Intrusive node for @c exited_list */

  CondVar child_exit;     /**< @brief Condition variable for @c WaitChild. 
                             This condition variable is  broadcast each time a child
                             process terminates. It is used in the implementation of
                             @c WaitChild() */

  FCB* FIDT[MAX_FILEID];  /**< @brief The fileid table of the process */

} PCB;


/**
  @brief Initialize the process table.
  This function is called during kernel initialization, to initialize
  any data structures related to process creation.
*/
void initialize_processes();

/**
  @brief Get the PCB for a PID.
  This function will return a pointer to the PCB of 
  the process with a given PID. If the PID does not
  correspond to a process, the function returns @c NULL.
  @param pid the pid of the process 
  @returns A pointer to the PCB of the process, or NULL.
*/
PCB* get_pcb(Pid_t pid);

/**
  @brief Get the PID of a PCB.
  This function will return the PID of the process 
  whose PCB is pointed at by @c pcb. If the pcb does not
  correspond to a process, the function returns @c NOPROC.
  @param pcb the pcb of the process 
  @returns the PID of the process, or NOPROC.
*/
Pid_t get_pid(PCB* pcb);

/** @} */

/*EDITS.........*/
//From this point until the end the code is ours
void cleanup_pcb();
// enum for detach state
typedef enum Detach_state{
  DETACH,
  UNDETACH
}detach_state;

// enum for exit state
typedef enum Exit_state{
  EXITED_STATE,
  NOTEXITED
}Exit_state;


typedef struct p_thread_control_block{

  
  uint refcount;                   // a variable to show how much joins there are
  TCB* tcb ;                      // the  thread of PTCB
  rlnode thread_list_node ;      // node for list

  Task main_task;               // the  thread func
  int argl;                    // argument lenght
  void *args ;                // argument string

  int exit_val;              // exit value of task
  
  
 
 
  detach_state is_detached;         // detach state
  Exit_state is_exited ;           // exit state 
  
  CondVar cond_var;               // condition variable for WaitChild
 
 


} PTCB;

int procinfo_read(void* read,char*buffer , unsigned int size);
int procinfo_write(void* write,const char*buffer , unsigned int size);
int procinfo_close(void* fid);



typedef struct procinfo_control_block{
  procinfo* curinfo;
  int cursor;
  
}procinfo_cb;


#endif