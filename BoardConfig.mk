LOCAL_PATH := device/ixion/P350

-include $(LOCAL_PATH)/ProjectConfig.mk

# Platform
TARGET_BOARD_PLATFORM := mt6580
TARGET_NO_BOOTLOADER := true

# Architecture
TARGET_ARCH := arm
TARGET_ARCH_VARIANT := armv7-a-neon
TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
TARGET_CPU_SMP := true
TARGET_CPU_VARIANT := cortex-a7
ARCH_ARM_HAVE_NEON := true
ARCH_ARM_HAVE_VFP := true
ARCH_ARM_HAVE_TLS_REGISTER := true

TARGET_LDPRELOAD := libxlog.so

BOARD_HAS_NO_SELECT_BUTTON := true

# Bootloader
TARGET_BOOTLOADER_BOARD_NAME := mt6580

# MTK Hardware
BOARD_HAS_MTK_HARDWARE := true
MTK_HARDWARE := true
BOARD_USES_LEGACY_MTK_AV_BLOB := true
COMMON_GLOBAL_CFLAGS += -DMTK_HARDWARE -DADD_LEGACY_ACQUIRE_BUFFER_SYMBOL
COMMON_GLOBAL_CPPFLAGS += -DMTK_HARDWARE

PRODUCT_PREBUILT_WEBVIEWCHROMIUM := yes

# Partitions
# make_ext4fs requires numbers in dec format
BOARD_BOOTIMAGE_PARTITION_SIZE := 16777216
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 16777216
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 1556925644
BOARD_USERDATAIMAGE_PARTITION_SIZE := 5583457484
BOARD_FLASH_BLOCK_SIZE := 131072

TARGET_USERIMAGES_USE_EXT4 := true
TARGET_USERIMAGES_SPARSE_EXT_DISABLED := false

BOARD_KERNEL_CMDLINE += \
	bootopt=64S3,32S1,32S1 \
	androidboot.selinux=permissive
	
BOARD_KERNEL_BASE := 0x80000000
BOARD_KERNEL_PAGESIZE := 2048

# Flags
TARGET_GLOBAL_CFLAGS   += -mfpu=neon -mfloat-abi=softfp
TARGET_GLOBAL_CPPFLAGS += -mfpu=neon -mfloat-abi=softfp

TARGET_KMODULES := true

COMMON_GLOBAL_CFLAGS += -DDISABLE_HW_ID_MATCH_CHECK
TARGET_RUNNING_WITHOUT_SYNC_FRAMEWORK := true

DEVICE_PACKAGE_OVERLAYS += $(LOCAL_PATH)/overlay

TARGET_PREBUILT_KERNEL := $(LOCAL_PATH)/kernel
BOARD_MKBOOTIMG_ARGS := --kernel_offset 0x00008000 --ramdisk_offset 0x04000000 --tags_offset 0x0e000000 --board 1441186574
BOARD_CUSTOM_BOOTIMG := true

TARGET_OTA_ASSERT_DEVICE := "dexp_ixion_p350","P350","DEXP Ixion P350","Ixion P350"

# Recovery
# Enable twrp variant only for 'make recoveryimage'
# Do not enable for full build
#RECOVERY_VARIANT := twrp

RECOVERY_GRAPHICS_USE_LINELENGTH := true
TARGET_PREBUILT_RECOVERY_KERNEL := $(LOCAL_PATH)/kernel
TARGET_USE_CUSTOM_LUN_FILE_PATH := "/sys/devices/platform/mt_usb/musb-hdrc.0.auto/gadget/lun%d/file"

ifeq ($(RECOVERY_VARIANT),twrp)
TARGET_RECOVERY_FSTAB := $(LOCAL_PATH)/rootdir/twrp.fstab
else
TARGET_RECOVERY_FSTAB := $(LOCAL_PATH)/rootdir/fstab.mt6580
endif

# TWRP
ifeq ($(RECOVERY_VARIANT),twrp)
RECOVERY_SDCARD_ON_DATA := true
TW_THEME := portrait_hdpi
DEVICE_RESOLUTION := 720x1280
TW_USE_MODEL_HARDWARE_ID_FOR_DEVICE_ID := true
TW_DEFAULT_LANGUAGE := ru
TW_NO_REBOOT_BOOTLOADER := false
TW_NO_USB_STORAGE := false
TW_BRIGHTNESS_PATH := "/sys/devices/platform/leds-mt65xx/leds/lcd-backlight/brightness"
TW_MAX_BRIGHTNESS := 255
TW_DEFAULT_EXTERNAL_STORAGE := true
TW_INCLUDE_L_CRYPTO := true
TW_CRYPTO_FS_TYPE := "ext4"
TW_CRYPTO_REAL_BLKDEV := "/dev/block/platform/mtk-msdc.0/by-name/userdata"
TW_CRYPTO_MNT_POINT := "/data"
TW_CRYPTO_FS_OPTIONS := "nosuid,nodev,noatime,discard,noauto_da_alloc,data=ordered"
TW_CUSTOM_CPU_TEMP_PATH := "/sys/devices/virtual/thermal/thermal_zone1/temp"
TW_INCLUDE_FB2PNG := true
endif

# Deodex
WITH_DEXPREOPT := false
DISABLE_DEXPREOPT := true

# Bluetooth
BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_BLUETOOTH_MTK := true
BOARD_BLUETOOTH_DOES_NOT_USE_RFKILL := true
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR := $(LOCAL_PATH)/bluetooth

# RIL
# BOARD_RIL_CLASS := ../../../$(LOCAL_PATH)/ril/

BOARD_CONNECTIVITY_VENDOR := MediaTek
BOARD_CONNECTIVITY_MODULE := conn_soc

# WIFI
WPA_SUPPLICANT_VERSION := VER_0_8_X
BOARD_HOSTAPD_DRIVER := NL80211
BOARD_HOSTAPD_PRIVATE_LIB := lib_driver_cmd_mt66xx
BOARD_WPA_SUPPLICANT_DRIVER := NL80211
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_mt66xx
WIFI_DRIVER_FW_PATH_PARAM := "/dev/wmtWifi"
WIFI_DRIVER_FW_PATH_STA := STA
WIFI_DRIVER_FW_PATH_AP := AP
WIFI_DRIVER_FW_PATH_P2P := P2P

# GPS
TARGET_SPECIFIC_HEADER_PATH := $(LOCAL_PATH)/include

# FM Radio
#MTK_FM_SUPPORT := yes
MTK_FM_RX_SUPPORT := yes
MTK_FM_CHIP := MT6580_FM

# EGL settings
BOARD_EGL_CFG := $(LOCAL_PATH)/config/lib/egl/egl.cfg
USE_OPENGL_RENDERER := true
BOARD_EGL_WORKAROUND_BUG_10194508 := true

# Disable memcpy opt (for audio libraries)
TARGET_CPU_MEMCPY_OPT_DISABLE := true

# SELINUX
BOARD_SEPOLICY_DIRS := \
	$(LOCAL_PATH)/sepolicy

BOARD_SEPOLICY_UNION := \
	app.te \
	device.te \
	domain.te \
	file.te \
	file_contexts \
	fs_use \
	installd.te \
	net.te \
	netd.te \
	te_macros \
	vold.te \
	untrusted_app.te \
	platform_app.te \
	system_app.te \
	zygote.te \
	aal.te \
	aee_core_forwarder.te \
	akmd09911.te \
	akmd8963.te \
	akmd8975.te \
	ami304d.te \
	auditd.te \
	ist8303.te \
	mc6470d.te \
	qmcX983d.te \
	st480.te \
	af7133e.te \
	mmc3524xd.te \
	atcid.te \
	atci_service.te \
	audiocmdservice_atci.te \
	batterywarning.te \
	bmm050d.te \
	bmm056d.te \
	bmx056d.te \
	boot_logo_updater.te \
	br_app_data_service.te \
	BGW.te \
	ccci_fsd.te \
	ccci_mdinit.te \
	statusd.te \
	flashlessd.te \
	ccci_rpcd.te \
	eemcs_fsd.te \
	eemcs_mdinit.te \
	dhcp6c.te \
	dm_agent_binder.te \
	dualmdlogger.te \
	dumpstate.te \
	em_svr.te \
	enableswap.te \
	disableswap.te \
	factory.te \
	fota1.te \
	fuelgauged.te \
	geomagneticd.te \
	GoogleOtaBinder.te \
	gsm0710muxdmd2.te \
	gsm0710muxd.te \
	guiext-server.te \
	ipod.te \
	matv.te \
	mc6420d.te \
	mdlogger.te \
	mdnsd.te \
	memsicd3416x.te \
	bmc156d.te \
	memsicd.te \
	memsicp.te \
	meta_tst.te \
	mmc_ffu.te \
	mmp.te \
	mnld.te \
	mobile_log_d.te \
	mpud6050.te \
	msensord.te \
	mtk_6620_launcher.te \
	mtk_agpsd.te \
	mtkbt.te \
	muxreport.te \
	netdiag.te \
	nvram_agent_binder.te \
	nvram_backup_binder.te \
	nvram_daemon.te \
	orientationd.te \
	permission_check.te \
	poad.te \
	pppd_dt.te \
	pppd_via.te \
	pq.te \
	recovery.te \
	resmon.te \
	mtkrild.te \
	mtkrildmd2.te \
	viarild.te \
	s62xd.te \
	sn.te \
	epdg_wod.te \
	ipsec.te \
	terservice.te \
	thermald.te \
	thermal_manager.te \
	thermal.te \
	tiny_mkswap.te \
	tiny_swapon.te \
	vdc.te \
	volte_imcb.te \
	volte_ua.te \
	volte_stack.te \
	wmt_loader.te \
	icusbd.te \
	xlog.te \
	mobicore.te \
	install_recovery.te \
	program_binary.te \
	genfs_contexts

BOARD_SEPOLICY_UNION += \
	adbd.te \
	bluetooth.te \
	bootanim.te \
	clatd.te \
	debuggerd.te \
	drmserver.te \
	dhcp.te \
	dnsmasq.te \
	gpsd.te \
	hci_attach.te \
	healthd.te \
	hostapd.te \
	inputflinger.te \
	init.te \
	init_shell.te \
	isolated_app.te \
	keystore.te \
	kernel.te \
	lmkd.te \
	logd.te \
	mediaserver.te \
	mtp.te \
	nfc.te \
	racoon.te \
	radio.te \
	rild.te \
	runas.te \
	sdcardd.te \
	servicemanager.te \
	shared_relro.te \
	shell.te \
	system_app.te \
	system_server.te \
	surfaceflinger.te \
	tee.te \
	ueventd.te \
	uncrypt.te \
	watchdogd.te \
	wpa_supplicant.te \
	wpa.te \
	property.te \
	property_contexts \
	service.te \
	dmlog.te \
	MtkCodecService.te \
	ppl_agent.te \
	pvrsrvctl.te \
	wifi2agps.te \
	dex2oat.te \
	emdlogger.te \
	autokd.te \
	ppp.te \
	launchpppoe.te \
	sbchk.te \
	service_contexts \
	ril-3gddaemon.te \
	usbdongled.te \
	zpppd_gprs.te \
	md_ctrl.te \
	cmddumper.te \
	tunman.te

