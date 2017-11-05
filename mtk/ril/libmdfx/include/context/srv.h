/*
    Here is an interface of service to hide implementation. 
    A service inherits a context for the init, exit, send, set_cond, get_id, and get_mailbox functions. 
    The srv_exec function invokes a user-defined handler. 
    In the user-defined handler, the srv_recv function waits for an event received from the mailbox. 
*/

#ifndef __CONTEXT_SRV_H__
#define __CONTEXT_SRV_H__

#include "sys_info.h"
#include "compiler.h"
#include "context.h"

// Compiler flags, NEED_TO_BE_NOTICED, set by the compiler
// N/A

// Type definitions
typedef struct srv srv_t;
typedef struct srv* srv_ptr_t;
typedef context_init_arg_t srv_init_arg_t;
typedef context_init_arg_ptr_t srv_init_arg_ptr_t;
typedef void* (*srv_hdl_t) (srv_ptr_t srv_ptr, void *arg);
typedef bool (*srv_cond_fp_t) (srv_ptr_t srv_ptr, event_ptr_t event_ptr, void *cond_arg);

// Macros
#define SRV_INVAL_ID    CONTEXT_INVAL_ID
// => Thread service
// N/A

// Functions
#define srv_init(srv_ptr, arg_ptr)    (unlikely((srv_ptr) == NULL) ? NULL : context_init(&((srv_ptr)->context), (context_init_arg_ptr_t)(arg_ptr)))
#define srv_exit(srv_ptr)    (unlikely((srv_ptr) == NULL) ? NULL : context_exit(&((srv_ptr)->context)))
#define srv_exec(srv_ptr, hdl, arg)    (unlikely((srv_ptr) == NULL || (srv_ptr)->exec_fp == NULL) ? SYS_FAIL : (srv_ptr)->exec_fp((srv_ptr), (hdl), (arg)))
#define srv_send(srv_ptr, event_ptr, addr_ptr)    (unlikely((srv_ptr) == NULL) ? SYS_FAIL : context_send(&((srv_ptr)->context), (event_ptr), (addr_ptr)))
#define srv_recv(srv_ptr)    (unlikely((srv_ptr) == NULL || (srv_ptr)->recv_fp == NULL) ? NULL : (srv_ptr)->recv_fp((srv_ptr)))
#define srv_set_cond(srv_ptr, cond_fp, cond_arg, cond_arg_len)    (unlikely((srv_ptr) == NULL) ? SYS_FAIL : context_set_cond(&((srv_ptr)->context), (context_cond_fp_t)(cond_fp), (cond_arg), (cond_arg_len)))
#define srv_get_id(srv_ptr)    (unlikely((srv_ptr) == NULL) ? SRV_INVAL_ID : context_get_id(&((srv_ptr)->context)))
#define srv_get_mailbox(srv_ptr)    (unlikely((srv_ptr) == NULL) ? NULL : context_get_mailbox(&((srv_ptr)->context)))
// => Thread service
// N/A

// Interface-implementation binding, NEED_TO_BE_NOTICED, if the srv_ptr were given as a value, a compile-time error would be given
// => Default service
#define default_srv_employ(srv_ptr)    thrd_srv_employ(srv_ptr)
#define default_srv_dismiss(srv_ptr)    thrd_srv_dismiss(srv_ptr)
// => Thread service
#define thrd_srv_employ(srv_ptr)    (unlikely((srv_ptr) == NULL) ? NULL : \
                                    (thrd_context_employ(&((srv_ptr)->context)), \
                                    ((srv_ptr)->exec_fp = thrd_srv_exec), \
                                    ((srv_ptr)->recv_fp = thrd_srv_recv), \
                                    (srv_ptr)))
#define thrd_srv_dismiss(srv_ptr)    (unlikely((srv_ptr) == NULL) ? NULL : \
                                     (thrd_context_dismiss(&((srv_ptr)->context)), \
                                     ((srv_ptr)->exec_fp = NULL), \
                                     ((srv_ptr)->recv_fp = NULL), \
                                     (srv_ptr)))

// Implementation
// => Abstract data type
struct srv
{
    // General variables
    // => Public
    context_t context;    // inheritance
    int (*exec_fp) (srv_ptr_t srv_ptr, srv_hdl_t hdl, void *arg);
    event_ptr_t (*recv_fp) (srv_ptr_t srv_ptr);
    // => Private
    // N/A
    // Individual variables which must be private
    union
    {
        // Thread service
        struct
        {
            // N/A
        }thrd;
    }idv;
};

// => Thread service
extern int thrd_srv_exec (srv_ptr_t srv_ptr, srv_hdl_t hdl, void *arg);
extern event_ptr_t thrd_srv_recv (srv_ptr_t srv_ptr);

#endif
