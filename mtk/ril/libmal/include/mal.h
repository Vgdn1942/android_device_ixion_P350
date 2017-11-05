#ifndef __MAL_INTERFACE_H__
#define __MAL_INTERFACE_H__

#include "mdfx.h"
#include "simmngr_interface.h"
#include "datamngr_interface.h"
#include "imsmngr_interface.h"

// Compiler flags, NEED_TO_BE NOTICED, set by the compiler
// N/A

// Type definitions
typedef void (*mal_report_hdl_t) (void *arg);

// Macros, NEED_TO_BE_NOTICED, the maximal size should be constrained
// N/A

// API
// => Logging
#define mal_print(tag, format, args ...)	(lv_print((tag), format, ## args))

void* mal_init ();

void* mal_init2 ();

int mal_exit (void *o);

extern int mal_report_restart (mal_report_hdl_t hdl, void * arg);

int mal_execute_atcmd (void *o, char *cmd, char *response, int response_len);

int mal_ril_connect (void *o);

int mal_ril_disconnect (void *o);

int mal_get_ps_reg_state (char *buf, unsigned int len);

int mal_get_ps_reg_state_ex (int type, char *buf, unsigned int buf_len);

int mal_set_ps_format(int long_format);

// API
// => Event
#define mal_event_alloc(rel_event_ptr)  (mdfx_event_alloc((rel_event_ptr)))
#define mal_event_free(event_ptr)  (mdfx_event_free((event_ptr)))
// => => Event ID
#define mal_event_set_id(event_ptr, id)	(mdfx_event_set_id((event_ptr), (id)))
#define mal_event_get_id(event_ptr)	(mdfx_event_get_id((event_ptr)))
// => => Event Data
#define mal_event_set_data(event_ptr, data, data_len)	(mdfx_event_set_data((event_ptr), (data), (data_len)))
#define mal_event_get_data(event_ptr, data_len_ptr)	(mdfx_event_get_data((event_ptr), (data_len_ptr)))
#define mal_event_get_data_len(event_ptr)	(mdfx_event_get_data_len((event_ptr)))

// => Timer
#define mal_tmr_init(tmr_ptr, arg_ptr)  (mdfx_tmr_init((tmr_ptr), (arg_ptr)))
#define mal_tmr_exit(tmr_ptr)   (mdfx_tmr_exit((tmr_ptr)))
#define mal_tmr_arm(tmr_ptr, time_ptr, event_ptr, context_id)   (mdfx_tmr_arm((tmr_ptr), (time_ptr), (event_ptr), (context_id)))
#define mal_conn_get_id(conn_ptr)   (unlikely((conn_ptr) == NULL) ? CONTEXT_INVAL_ID : (conn_ptr)->chnl.idv.uds.sock + SRV_MAX_ID)

#endif /* __MAL_INTERFACE_H__ */
