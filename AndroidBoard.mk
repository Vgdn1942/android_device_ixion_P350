LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

ALL_PREBUILT += $(INSTALLED_KERNEL_TARGET)

ifneq ($(TARGET_PREBUILT_KERNEL),)
$(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr:
	mkdir -p $@
endif

# include the non-open-source counterpart to this file
-include vendor/ixion/P350/AndroidBoardVendor.mk

