ifeq ($(PRODUCT_PREBUILT_WEBVIEWCHROMIUM),yes)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libwebviewchromium
LOCAL_SRC_FILES := lib/libwebviewchromium.so
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE_CLASS := SHARED_LIBRARIES

include $(BUILD_PREBUILT)



include $(CLEAR_VARS)

LOCAL_MODULE := webview
LOCAL_SRC_FILES := app/webview/webview.apk
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_SUFFIX := $(COMMON_ANDROID_PACKAGE_SUFFIX)
LOCAL_MODULE_CLASS := APPS
LOCAL_CERTIFICATE := PRESIGNED

LOCAL_JNI_SHARED_LIBRARIES += libwebviewchromium

LOCAL_REQUIRED_MODULES := \
		libwebviewchromium \
		libwebviewchromium_loader \
		libwebviewchromium_plat_support

include $(BUILD_PREBUILT)

endif
