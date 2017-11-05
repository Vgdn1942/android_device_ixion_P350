/*
	Here is an interface of task to hide implementation. 
	A task inherits a context for the init, exit, send, set_cond, get_id, and get_mailbox functions. 
	The task_exec function passively waits for an event received from the mailbox, and then passes the event to a user-defined handler. 
*/

#ifndef __TASK_H__
#define __TASK_H__

#include "sys_info.h"
#include "compiler.h"
#include "context.h"

// Compiler flags, NEED_TO_BE_NOTICED, set by the compiler
// N/A

// Type definitions
typedef struct task task_t;
typedef struct task* task_ptr_t;
typedef context_init_arg_t task_init_arg_t;
typedef context_init_arg_ptr_t task_init_arg_ptr_t;
typedef void* (*task_hdl_t) (task_ptr_t task_ptr, event_ptr_t event_ptr, void *arg);
typedef bool (*task_cond_fp_t) (task_ptr_t task_ptr, event_ptr_t event_ptr, void *cond_arg);

// Macros
#define TASK_INVAL_ID	CONTEXT_INVAL_ID
// => Thread task
// N/A
// => MFI task
#define MFI_TASK_LISTEN_BACKLOG        8
#define MFI_TASK_PATH   (is_dongle_mode == true ? "mal-mfi-dongle" : "mal-mfi")
// => MFIA task
#define MFIA_TASK_LISTEN_BACKLOG        8
#define MFIA_PATH_LEN   64
// =>ATI task
#define ATI_TASK_LISTEN_BACKLOG 8
#define ATI_TASK_ATC_BUF_SIZE   4096
#define ATI_TASK_UART_PATH_1    "/dev/radio/malcmd1"
#define ATI_TASK_UART_PATH_2    "/dev/radio/malcmd2"
#define ATI_TASK_UART_PATH_3    "/dev/radio/malcmd3"
#define ATI_TASK_UART_PATH_4    "/dev/radio/malcmd4"
#define ATI_TASK_UART_PATH_URC  "/dev/radio/malurc"

// Functions
#define task_init(task_ptr, arg_ptr)	(unlikely((task_ptr) == NULL) ? NULL : context_init(&((task_ptr)->context), (context_init_arg_ptr_t)(arg_ptr)))
#define task_exit(task_ptr)	(unlikely((task_ptr) == NULL) ? NULL : context_exit(&((task_ptr)->context)))
#define task_exec(task_ptr, hdl, arg)	(unlikely((task_ptr) == NULL || (task_ptr)->exec_fp == NULL) ? SYS_FAIL : (task_ptr)->exec_fp((task_ptr), (hdl), (arg)))
#define task_send(task_ptr, event_ptr, addr_ptr)	(unlikely((task_ptr) == NULL) ? SYS_FAIL : context_send(&((task_ptr)->context), (event_ptr), (addr_ptr)))
#define task_set_cond(task_ptr, cond_fp, cond_arg, cond_arg_len)	(unlikely((task_ptr) == NULL) ? SYS_FAIL : context_set_cond(&((task_ptr)->context), (context_cond_fp_t)(cond_fp), (cond_arg), (cond_arg_len)))
#define task_get_id(task_ptr)	(unlikely((task_ptr) == NULL) ? TASK_INVAL_ID : context_get_id(&((task_ptr)->context)))
#define task_get_mailbox(task_ptr)	(unlikely((task_ptr) == NULL) ? NULL : context_get_mailbox(&((task_ptr)->context)))
// => Thread task
// N/A

// Interface-implementation binding, NEED_TO_BE_NOTICED, if the task_ptr were given as a value, a compile-time error would be given
// => Default task
#define default_task_employ(task_ptr)	thrd_task_employ(task_ptr)
#define default_task_dismiss(task_ptr)	thrd_task_dismiss(task_ptr)
// => Thread task
#define thrd_task_employ(task_ptr)	(unlikely((task_ptr) == NULL) ? NULL : \
                                  	(thrd_context_employ(&((task_ptr)->context)), \
                                  	((task_ptr)->exec_fp = thrd_task_exec), \
                                  	(task_ptr)))
#define thrd_task_dismiss(task_ptr)	(unlikely((task_ptr) == NULL) ? NULL : \
                                   	(thrd_context_dismiss(&((task_ptr)->context)), \
                                   	((task_ptr)->exec_fp = NULL), \
                                   	(task_ptr)))
// ==== TO RE_WRITE ====
// => MFI task
#define mfi_task_employ(task_ptr)	(unlikely((task_ptr) == NULL) ? NULL : \
                                 	(((task_ptr)->context.init_fp = mfi_task_init), \
                                 	((task_ptr)->context.exit_fp = mfi_task_exit), \
                                 	((task_ptr)->exec_fp = mfi_task_exec), \
                                 	(task_ptr)))
#define mfi_task_dismiss(task_ptr)	(unlikely((task_ptr) == NULL) ? NULL : \
                                  	(((task_ptr)->context.init_fp = NULL), \
                                  	((task_ptr)->context.exit_fp = NULL), \
                                  	((task_ptr)->exec_fp = NULL), \
                                  	(task_ptr)))

// => ATI task
#define ati_task_employ(task_ptr)	(unlikely((task_ptr) == NULL) ? NULL : \
                                 	(((task_ptr)->context.init_fp = ati_task_init), \
                                 	((task_ptr)->context.exit_fp = ati_task_exit), \
                                 	((task_ptr)->exec_fp = ati_task_exec), \
                                 	(task_ptr)))
#define ati_task_dismiss(task_ptr)	(unlikely((task_ptr) == NULL) ? NULL : \
                                  	(((task_ptr)->context.init_fp = NULL), \
                                  	((task_ptr)->context.exit_fp = NULL), \
                                  	((task_ptr)->exec_fp = NULL), \
                                  	(task_ptr)))

// => MFIA task
#define mfia_task_employ(task_ptr)	(unlikely((task_ptr) == NULL) ? NULL : \
                                  	(((task_ptr)->context.init_fp = mfia_task_init), \
                                  	((task_ptr)->context.exit_fp = mfia_task_exit), \
                                  	((task_ptr)->exec_fp = mfia_task_exec), \
                                  	(task_ptr)))
#define mfia_task_dismiss(task_ptr)	(unlikely((task_ptr) == NULL) ? NULL : \
                                  	(((task_ptr)->context.init_fp = NULL), \
                                  	((task_ptr)->context.exit_fp = NULL), \
                                  	((task_ptr)->exec_fp = NULL), \
                                  	(task_ptr)))
// ==== TO RE_WRITE ====

// Implementation
// => Abstract data type
struct task
{
	// General variables
	// => Public
	context_t context;	// inheritance
	int (*exec_fp) (task_ptr_t task_ptr, task_hdl_t hdl, void *arg);
	// => Private
	// N/A
	// Individual variables which must be private
	union
	{
		// Thread task
		struct
		{
			// N/A
		}thrd;

// ==== TO RE_WRITE ====
		// MFI task
		struct
		{
			thrd_t thrd;
		}mfi;

		// ATI task
		struct
		{
			thrd_t thrd;
		}ati;

		// MFIA task
		struct
		{
			thrd_t thrd;
            char path[MFIA_PATH_LEN];
		}mfia;
// ==== TO RE_WRITE ====
	}idv;
};

// => Thread task
extern int thrd_task_exec (task_ptr_t task_ptr, task_hdl_t hdl, void *arg);
// ==== TO RE_WRITE ====
// => MFI task
extern task_ptr_t mfi_task_init (task_ptr_t task_ptr, const task_init_arg_ptr_t arg_ptr);
extern task_ptr_t mfi_task_exit (task_ptr_t task_ptr);
extern int mfi_task_exec (task_ptr_t task_ptr, task_hdl_t hdl, void *arg);
// => ATI task
extern task_ptr_t ati_task_init (task_ptr_t task_ptr, const task_init_arg_ptr_t arg_ptr);
extern task_ptr_t ati_task_exit (task_ptr_t task_ptr);
extern int ati_task_exec (task_ptr_t task_ptr, task_hdl_t hdl, void *arg);
// => MFIA task
extern task_ptr_t mfia_task_init (task_ptr_t task_ptr, const task_init_arg_ptr_t arg_ptr);
extern task_ptr_t mfia_task_exit (task_ptr_t task_ptr);
extern int mfia_task_exec (task_ptr_t task_ptr, task_hdl_t hdl, void *arg);
// ==== TO RE_WRITE ====

#endif
