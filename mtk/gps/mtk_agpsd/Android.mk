LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = mtk_agpsd
LOCAL_MODULE_CLASS = EXECUTABLES
LOCAL_MODULE_OWNER = mtk
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES = libcrypto libssl libicuuc libnetd_client libc++
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/mtk_agpsd
include $(BUILD_PREBUILT)
