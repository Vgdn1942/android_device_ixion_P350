typedef struct _tmr_notify {
    unsigned int            timer_id;
    unsigned int            timeout;
    unsigned int            user_data;
} tmr_notify_t;

typedef int (*imsmngr_tmr_timeout_notify_fn)(tmr_notify_t *, void *priv);
typedef struct _mal_imsmngr_notify_function {
    imsmngr_tmr_timeout_notify_fn    tmr_timeout;
} mal_imsmngr_notify_function_t;

extern mal_imsmngr_notify_function_t mal_imsmngr_notify;

int mal_imsmngr_register(mal_imsmngr_notify_function_t *callback_fn);
