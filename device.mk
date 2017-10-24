# call the proprietary setup
$(call inherit-product-if-exists, vendor/ixion/P350/P350-vendor.mk)

LOCAL_PATH := device/ixion/P350

ifeq ($(TARGET_PREBUILT_KERNEL),)
	LOCAL_KERNEL := $(LOCAL_PATH)/kernel
else
	LOCAL_KERNEL := $(TARGET_PREBUILT_KERNEL)
endif

# PERMISSIONS
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/config/etc/permissions/platform.xml:system/etc/permissions/platform.xml \
	frameworks/native/data/etc/android.hardware.bluetooth_le.xml:system/etc/permissions/android.hardware.bluetooth_le.xml \
	frameworks/native/data/etc/android.hardware.bluetooth.xml:system/etc/permissions/android.hardware.bluetooth.xml \
	$(LOCAL_PATH)/config/etc/permissions/android.hardware.camera.xml:system/etc/permissions/android.hardware.camera.xml \
	frameworks/native/data/etc/android.hardware.faketouch.xml:system/etc/permissions/android.hardware.faketouch.xml \
	frameworks/native/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
	frameworks/native/data/etc/android.hardware.sensor.accelerometer.xml:system/etc/permissions/android.hardware.sensor.accelerometer.xml \
	frameworks/native/data/etc/android.hardware.sensor.compass.xml:system/etc/permissions/android.hardware.sensor.compass.xml \
	frameworks/native/data/etc/android.hardware.sensor.gyroscope.xml:system/etc/permissions/android.hardware.sensor.gyroscope.xml \
	frameworks/native/data/etc/android.hardware.sensor.light.xml:system/etc/permissions/android.hardware.sensor.light.xml \
	frameworks/native/data/etc/android.hardware.sensor.proximity.xml:system/etc/permissions/android.hardware.sensor.proximity.xml \
	frameworks/native/data/etc/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml \
	frameworks/native/data/etc/android.hardware.touchscreen.multitouch.distinct.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.distinct.xml \
	frameworks/native/data/etc/android.hardware.touchscreen.multitouch.jazzhand.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.jazzhand.xml \
	frameworks/native/data/etc/android.hardware.touchscreen.multitouch.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.xml \
	frameworks/native/data/etc/android.hardware.touchscreen.xml:system/etc/permissions/android.hardware.touchscreen.xml \
	frameworks/native/data/etc/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml \
	frameworks/native/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml \
	frameworks/native/data/etc/android.hardware.wifi.direct.xml:system/etc/permissions/android.hardware.wifi.direct.xml \
	frameworks/native/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
	packages/wallpapers/LivePicker/android.software.live_wallpaper.xml:system/etc/permissions/android.software.live_wallpaper.xml \
	frameworks/native/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml

# AGPS
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/config/etc/agps_profiles_conf2.xml:system/etc/agps_profiles_conf2.xml

# Telecom
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/config/etc/apns-conf.xml:system/etc/apns-conf.xml \
	$(LOCAL_PATH)/config/etc/ecc_list.xml:system/etc/ecc_list.xml \
	$(LOCAL_PATH)/config/etc/spn-conf.xml:system/etc/spn-conf.xml
	
# Audio	
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/config/etc/media_profiles.xml:system/etc/media_profiles.xml \
	$(LOCAL_PATH)/config/etc/media_codecs.xml:system/etc/media_codecs.xml \
	$(LOCAL_PATH)/config/etc/audio_device.xml:system/etc/audio_device.xml \
	$(LOCAL_PATH)/config/etc/audio_policy.conf:system/etc/audio_policy.conf \
	$(LOCAL_PATH)/config/etc/permissions/android.hardware.microphone.xml:system/etc/permissions/android.hardware.microphone.xml

# Thermal
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/config/etc/tp/.ht120.mtc:system/etc/.tp/.ht120.mtc \
	$(LOCAL_PATH)/config/etc/tp/thermal.conf:system/etc/.tp/thermal.conf \
	$(LOCAL_PATH)/config/etc/tp/thermal.off.conf:system/etc/.tp/thermal.off.conf
	
# Keylayout
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/config/usr/keylayout/mtk-kpd.kl:system/usr/keylayout/mtk-kpd.kl


# RAMDISK
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/rootdir/sbin/busybox:root/sbin/busybox \
	$(LOCAL_PATH)/rootdir/enableswap.sh:root/enableswap.sh \
	$(LOCAL_PATH)/rootdir/fstab.mt6580:root/fstab.mt6580 \
	$(LOCAL_PATH)/rootdir/init.recovery.mt6580.rc:root/init.recovery.mt6580.rc \
	$(LOCAL_PATH)/rootdir/init.mt6580.rc:root/init.mt6580.rc \
	$(LOCAL_PATH)/rootdir/init.project.rc:root/init.project.rc \
	$(LOCAL_PATH)/rootdir/init.ssd.rc:root/init.ssd.rc \
	$(LOCAL_PATH)/rootdir/init.modem.rc:root/init.modem.rc \
	$(LOCAL_PATH)/rootdir/ueventd.rc:root/ueventd.rc \
	$(LOCAL_PATH)/rootdir/init.mt6580.usb.rc:root/init.mt6580.usb.rc \
	$(LOCAL_PATH)/rootdir/init.trace.rc:root/init.trace.rc \
	$(LOCAL_PATH)/rootdir/init.nvdata.rc:root/init.nvdata.rc \
	$(LOCAL_PATH)/rootdir/init.mt6580.power.rc:root/init.mt6580.power.rc \
	$(LOCAL_PATH)/rootdir/init.xlog.rc:root/init.xlog.rc \
	$(LOCAL_PATH)/rootdir/init.aee.rc:root/init.aee.rc \
	$(LOCAL_KERNEL):kernel

PRODUCT_PACKAGES += \
	libxlog

# FM Radio
PRODUCT_PACKAGES += \
	FMRadio \
	libfmjni \
	libfmmt6580 \
	libfmmt6620 \
	libfmmt6628 \
	libfmmt6627 \
	libfmmt6630 \
	libfmcust

PRODUCT_PACKAGES += \
	audio_policy.default \
	audio.a2dp.default \
	audio_policy.stub \
	audio.usb.default \
	audio.r_submix.default \
	libdashplayer

PRODUCT_PACKAGES += \
	libgralloc_extra

# RIL bin/lib
PRODUCT_PACKAGES += \
	ccci_fsd \
	ccci_mdinit \
	gsm0710muxd \
	gsm0710muxdmd2 \
	mtkrild \
	mtkrildmd2 \
	libaed \
	librilmtk \
	librilmtkmd2 \
	mtk-ril \
	mtk-rilmd2

# Prebuilt WebView
ifeq ($(PRODUCT_PREBUILT_WEBVIEWCHROMIUM),yes)
PRODUCT_PACKAGES += \
	webview \
	libwebviewchromium
endif

# hack to fix asec on emulated sdcard
PRODUCT_PACKAGES += \
	asec_helper

# Bluetooth
PRODUCT_PACKAGES += \
	libbt-vendor

PRODUCT_PACKAGES += \
	libwpa_client \
	hostapd \
	dhcpcd.conf \
	hostapd_cli \
	wpa_supplicant \
	wpa_supplicant.conf

PRODUCT_PACKAGES += \
	libion

# Keyhandler
PRODUCT_PACKAGES += \
	com.cyanogenmod.keyhandler \
	Gestures

PRODUCT_SYSTEM_SERVER_JARS += \
	com.cyanogenmod.keyhandler

PRODUCT_DEFAULT_PROPERTY_OVERRIDES := \
	ro.secure=0 \
	ro.adb.secure=0 \
	ro.allow.mock.location=1 \
	ro.debuggable=1 \
	ro.zygote=zygote32 \
	persist.sys.usb.config=mtp

ADDITIONAL_DEFAULT_PROPERTIES += \
	ro.secure=0 \
	ro.adb.secure=0 \
	ro.allow.mock.location=1 \
	ro.debuggable=1 \
	persist.service.acm.enable=0 \
	ro.oem_unlock_supported=1

PRODUCT_BUILD_PROP_OVERRIDES += BUILD_UTC_DATE=0

$(call inherit-product, build/target/product/full.mk)
$(call inherit-product, frameworks/native/build/phone-xhdpi-1024-dalvik-heap.mk)
# never dexopt the keyhandler
$(call add-product-dex-preopt-module-config,com.cyanogenmod.keyhandler,disable)
