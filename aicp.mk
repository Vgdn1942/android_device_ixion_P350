## Specify phone tech before including full_phone

# Release name
PRODUCT_RELEASE_NAME := P350

# Inherit some common AICP stuff.
$(call inherit-product, vendor/aicp/configs/common.mk)

# Inherit phone stuff
$(call inherit-product, vendor/aicp/configs/gsm.mk)

# Inherit telephony stuff
$(call inherit-product, vendor/aicp/configs/telephony.mk)

# Inherit device configuration
$(call inherit-product, device/ixion/P350/device_P350.mk)

## Device identifier. This must come after all inclusions
PRODUCT_DEVICE := P350
PRODUCT_NAME := aicp_P350
PRODUCT_BRAND := Ixion
PRODUCT_MODEL := DEXP Ixion P350
PRODUCT_MANUFACTURER := DEXP

# SuperSU
PRODUCT_COPY_FILES += \
    vendor/aicp/prebuilt/common/UPDATE-SuperSU.zip:system/addon.d/UPDATE-SuperSU.zip \
    vendor/aicp/prebuilt/common/etc/init.d/99SuperSUDaemon:system/etc/init.d/99SuperSUDaemon

