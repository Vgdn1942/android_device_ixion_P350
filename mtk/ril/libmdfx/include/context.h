/*
    Here is an interface of context to hide implementation. 
    The init function initializes the context with a mailbox to send and receive events. 
    The exit function uninitializes the context. 
    The exec function is an abstract interface to conditional pass received events to a user-defined handler. 
*/

#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "sys_info.h"
#include "compiler.h"
#include "event.h"
#include "io/mailbox.h"
#include "thrd.h"
#include "thrd/thrd_lock.h"
#include "ctnr/chain.h"
#include <stddef.h>

// Compiler flags, NEED_TO_BE_NOTICED, set by the compiler
// N/A

// Type definitions
typedef struct context context_t;
typedef struct context* context_ptr_t;
typedef struct context_init_arg context_init_arg_t;
typedef struct context_init_arg* context_init_arg_ptr_t;
// ====
typedef enum task_id context_id_t;
// ====
typedef void* (*context_hdl_t) (context_ptr_t context_ptr, event_ptr_t event_ptr, void *arg);
typedef bool (*context_cond_fp_t) (context_ptr_t context_ptr, event_ptr_t event_ptr, void *cond_arg);
// => Thread context
// N/A

// Macros
#define CONTEXT_INVAL_ID    (-1)
#define CONTEXT_COND_ARG_INIT_SIZE    (sizeof(int))    // NEED_TO_BE_NOTICED, we assume that an integer is the most common key to be used
// => Thread context
// N/A

// Functions
#define context_init(context_ptr, arg_ptr)    (unlikely((context_ptr) == NULL || (context_ptr)->init_fp == NULL) ? NULL : (context_ptr)->init_fp((context_ptr), (arg_ptr)))
#define context_exit(context_ptr)    (unlikely((context_ptr) == NULL || (context_ptr)->exit_fp == NULL) ? NULL : (context_ptr)->exit_fp((context_ptr)))
#define context_exec(context_ptr, hdl, arg)    (unlikely((context_ptr) == NULL || (context_ptr)->exec_fp == NULL) ? SYS_FAIL : (context_ptr)->exec_fp((context_ptr), (hdl), (arg)))
#define context_send(context_ptr, event_ptr, addr_ptr)    (unlikely((context_ptr) == NULL || (context_ptr)->send_fp == NULL) ? SYS_FAIL : (context_ptr)->send_fp((context_ptr), (event_ptr), (addr_ptr)))
#define context_set_cond(context_ptr, cond_fp, cond_arg, cond_arg_len)    (unlikely((context_ptr) == NULL) ? SYS_FAIL : (context_ptr)->set_cond_fp((context_ptr), (cond_fp), (cond_arg), (cond_arg_len)))
#define context_get_id(context_ptr)    (unlikely((context_ptr) == NULL) ? CONTEXT_INVAL_ID : (context_ptr)->id)
#define context_get_mailbox(context_ptr)    (unlikely((context_ptr) == NULL) ? NULL : &((context_ptr)->mailbox))
// => Thread context
// N/A

// Interface-implementation binding, NEED_TO_BE_NOTICED, if the context_ptr were given as a value, a compile-time error would be given
// => Default context
#define default_context_employ(context_ptr)    thrd_context_employ(context_ptr)
#define default_context_dismiss(context_ptr)    thrd_context_dismiss(context_ptr)
// => Thread context
#define thrd_context_employ(context_ptr)    (unlikely((context_ptr) == NULL) ? NULL : \
                                            (((context_ptr)->init_fp = thrd_context_init), \
                                            ((context_ptr)->exit_fp = thrd_context_exit), \
                                            ((context_ptr)->exec_fp = thrd_context_exec), \
                                            ((context_ptr)->send_fp = thrd_context_send), \
                                            ((context_ptr)->set_cond_fp = thrd_context_set_cond), \
                                            (context_ptr)))
#define thrd_context_dismiss(context_ptr)    (unlikely((context_ptr) == NULL) ? NULL : \
                                             (((context_ptr)->init_fp = NULL), \
                                             ((context_ptr)->exit_fp = NULL), \
                                             ((context_ptr)->exec_fp = NULL), \
                                             ((context_ptr)->send_fp = NULL), \
                                             ((context_ptr)->set_cond_fp = NULL), \
                                             (context_ptr)))

// Implementation
// => initialization arguments for contexts
struct context_init_arg
{
    // General variables
    context_id_t id;
    // Individual variables which must be private
    union
    {
        // Thread context
        struct
        {
            // N/A
        }thrd;

        // MFI task
        struct
        {
            // N/A
        }mfi;

        // ATI task
        struct
        {
            // N/A
        }ati;

        // MFIA task
        struct
        {
            char *chnl_name;
        }mfia;
    }idv;
};

// => Abstract data type
struct context
{
    // General variables
    // => Public
    context_ptr_t (*init_fp) (context_ptr_t context_ptr, const context_init_arg_ptr_t arg_ptr);
    context_ptr_t (*exit_fp) (context_ptr_t context_ptr);
    int (*exec_fp) (context_ptr_t context_ptr, context_hdl_t hdl, void *arg);
    int (*send_fp) (context_ptr_t context_ptr, event_ptr_t event_ptr, mailbox_addr_ptr_t addr_ptr);
    int (*set_cond_fp) (context_ptr_t context_ptr, context_cond_fp_t cond_fp, const void *cond_arg, size_t cond_arg_len);
    // => Private
    context_id_t id;
    mailbox_t mailbox;
    event_seq_t seq;
    context_cond_fp_t cond_fp;
    void *cond_arg;
    size_t cond_arg_size;
    // Individual variables which must be private
    union
    {
        // Thread context
        struct
        {
            thrd_t thrd;
            thrd_lock_t cond_mutex;
            bool is_set_cond;
            int next_idx;
            chain_t event_buf;
        }thrd;
    }idv;
};

// => Thread context
extern context_ptr_t thrd_context_init (context_ptr_t context_ptr, const context_init_arg_ptr_t arg_ptr);
extern context_ptr_t thrd_context_exit (context_ptr_t context_ptr);
extern int thrd_context_exec (context_ptr_t context_ptr, context_hdl_t hdl, void *arg);
extern int thrd_context_send (context_ptr_t context_ptr, event_ptr_t event_ptr, mailbox_addr_ptr_t addr_ptr);
extern int thrd_context_set_cond (context_ptr_t context_ptr, context_cond_fp_t cond_fp, const void *cond_arg, size_t cond_arg_len);

#endif
