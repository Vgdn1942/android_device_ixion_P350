LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_IS_HOST_MODULE =
LOCAL_MODULE = libdrmmtkutil
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_PATH =
LOCAL_MODULE_RELATIVE_PATH =
LOCAL_MODULE_SUFFIX = .so
LOCAL_SHARED_LIBRARIES = libicui18n libicuuc libcrypto libssl libdrmframework libdrmmtkwhitelist libbinder libnvramagentclient libmtk_drvb libc++
LOCAL_EXPORT_C_INCLUDE_DIRS = $(LOCAL_PATH)/../include
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = libdrmmtkutil.so
include $(BUILD_PREBUILT)
