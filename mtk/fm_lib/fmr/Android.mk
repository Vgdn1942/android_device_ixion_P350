
FM_LIB_BUILD_MT6580 := yes

LOCAL_PATH := $(call my-dir)
###############################################################################
# Define MTK FM Radio Chip solution
###############################################################################

include $(CLEAR_VARS)

LOCAL_CFLAGS+= \
    -DMT6580_FM

LOCAL_SRC_FILES := \
	fmr_core.cpp \
	fmr_err.cpp \
	libfm_jni.cpp
	
LOCAL_C_INCLUDES := $(JNI_H_INCLUDE) \
	frameworks/av/include/media

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libdl \
	libmedia

LOCAL_PRELINK_MODULE := false

LOCAL_MODULE := libfmjni

include $(BUILD_SHARED_LIBRARY)

########################

ifeq ($(FM_LIB_BUILD_MT6580), yes)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	mt6580.cpp \
	common.cpp 

LOCAL_C_INCLUDES := $(JNI_H_INCLUDE) \
	frameworks/av/include/media \

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libdl \
	libmedia

LOCAL_PRELINK_MODULE := false

LOCAL_MODULE := libfmmt6580
include $(BUILD_SHARED_LIBRARY)
endif

########################
