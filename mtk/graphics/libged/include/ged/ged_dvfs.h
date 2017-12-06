#ifndef __GED_DVFS_H__
#define __GED_DVFS_H__

#include "ged_type.h"

#define GED_NO_UM_SERVICE -1

#define GED_DVFS_VSYNC_OFFSET_SIGNAL_EVENT 44
#define GED_FPS_CHANGE_SIGNAL_EVENT        45
#define GED_SRV_SUICIDE_EVENT 46

#define DYNAMIC_VSYNC_OFFSET_PROTOCOL 10002

#if defined (__cplusplus)
extern "C" {
#endif

GED_ERROR ged_dvfs_policy(GED_HANDLE hGed, unsigned long ul3DFenceDoneTime, unsigned long ulvsync_period,unsigned long t, long phase);
void ged_dvfs_set_vsync_offset(int voffset);
unsigned long proc_overhead;

#define GED_EVENT_TOUCH (1 << 0)
#define GED_EVENT_THERMAL (1 << 1)
#define GED_EVENT_WFD (1 << 2)
#define GED_EVENT_MHL  (1 << 3)
#define GED_EVENT_GAS  (1 << 4)

#define GED_EVENT_FORCE_ON  (1 << 0)
#define GED_EVENT_FORCE_OFF  (1 << 1)
#define GED_EVENT_NOT_SYNC  (1 << 2)

#define GED_VSYNC_OFFSET_NOT_SYNC -2
#define GED_VSYNC_OFFSET_SYNC -3

#if defined (__cplusplus)
}
#endif

#endif
