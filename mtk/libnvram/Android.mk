LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := libnvram
LOCAL_SRC_FILES := libnvram.so
LOCAL_SHARED_LIBRARIES := libcustom_nvram libnvram_platform libnvram_sec libstdc++
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_SUFFIX := .so
include $(BUILD_PREBUILT)
