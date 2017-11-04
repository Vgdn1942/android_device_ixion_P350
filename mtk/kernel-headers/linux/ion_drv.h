#ifndef __ION_DRV_H__
#define __ION_DRV_H__
#include <linux/ion.h>
#include <linux/ion_debugger.h>
// Structure definitions

typedef enum 
{
    ION_CMD_SYSTEM,
    ION_CMD_MULTIMEDIA,
} ION_CMDS;

typedef enum
{
    ION_MM_CONFIG_BUFFER,
    ION_MM_SET_DEBUG_INFO,
    ION_MM_GET_DEBUG_INFO,
    ION_MM_SET_SF_BUF_INFO,
    ION_MM_GET_SF_BUF_INFO
} ION_MM_CMDS;

typedef enum
{
    ION_SYS_CACHE_SYNC,
    ION_SYS_GET_PHYS,
    ION_SYS_GET_CLIENT,
    ION_SYS_RECORD,
    ION_SYS_SET_HANDLE_BACKTRACE,
    ION_SYS_SET_CLIENT_NAME,
} ION_SYS_CMDS;

typedef enum
{
    ION_CACHE_CLEAN_BY_RANGE,
    ION_CACHE_INVALID_BY_RANGE,
    ION_CACHE_FLUSH_BY_RANGE,
    ION_CACHE_CLEAN_BY_RANGE_USE_VA,
    ION_CACHE_INVALID_BY_RANGE_USE_VA,
    ION_CACHE_FLUSH_BY_RANGE_USE_VA,   
    ION_CACHE_CLEAN_ALL,
    ION_CACHE_INVALID_ALL,
    ION_CACHE_FLUSH_ALL
} ION_CACHE_SYNC_TYPE;

typedef enum
{
    ION_ERROR_CONFIG_LOCKED = 0x10000
} ION_ERROR_E;

typedef struct ion_sys_cache_sync_param
{
	union
	{
		ion_user_handle_t handle;
	    void* kernel_handle;
	};
    void* va;
    unsigned int size;
    ION_CACHE_SYNC_TYPE sync_type;
} ion_sys_cache_sync_param_t;

typedef struct ion_sys_get_phys_param
{
	union
	{
		ion_user_handle_t handle;
		void* kernel_handle;
	};
    unsigned int phy_addr;
    unsigned int len;
} ion_sys_get_phys_param_t;

#define ION_MM_DBG_NAME_LEN 16
#define ION_MM_SF_BUF_INFO_LEN 16

typedef struct __ion_sys_client_name
{
    char name[ION_MM_DBG_NAME_LEN];
}ion_sys_client_name_t;

typedef struct ion_sys_get_client_param
{
    unsigned int client;
} ion_sys_get_client_param_t;

typedef struct ion_sys_data
{
    ION_SYS_CMDS sys_cmd;
    union
    {
        ion_sys_cache_sync_param_t cache_sync_param;
        ion_sys_get_phys_param_t   get_phys_param;
        ion_sys_get_client_param_t get_client_param;
        ion_sys_client_name_t client_name_param;
	    ion_sys_record_t record_param;
    };
} ion_sys_data_t;

typedef struct ion_mm_config_buffer_param
{
	union
	{
		ion_user_handle_t handle;
		void* kernel_handle;
	};
    int eModuleID;
    unsigned int security;
    unsigned int coherent;
} ion_mm_config_buffer_param_t;

typedef struct __ion_mm_buf_debug_info
{
	union
	{
		ion_user_handle_t handle;
		void* kernel_handle;
	};
    char dbg_name[ION_MM_DBG_NAME_LEN];
    unsigned int value1;
    unsigned int value2;
    unsigned int value3;
    unsigned int value4;
}ion_mm_buf_debug_info_t;

typedef struct __ion_mm_sf_buf_info
{
	union
	{
		ion_user_handle_t handle;
		void* kernel_handle;
	};
    unsigned int info[ION_MM_SF_BUF_INFO_LEN];
}ion_mm_sf_buf_info_t;

typedef struct ion_mm_data
{
    ION_MM_CMDS mm_cmd;
    union
    {
        ion_mm_config_buffer_param_t config_buffer_param;
        ion_mm_buf_debug_info_t  buf_debug_info_param;
        ion_mm_sf_buf_info_t sf_buf_info_param;
    };
} ion_mm_data_t;

#endif
