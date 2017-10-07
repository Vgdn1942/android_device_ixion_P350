$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)

PRODUCT_CHARACTERISTICS := default

PRODUCT_PROPERTY_OVERRIDES += ro.product.locale=ru-RU
PRODUCT_PROPERTY_OVERRIDES += ro.product.locale.region=RU
PRODUCT_PROPERTY_OVERRIDES += ro.product.locale.language=ru

# The gps config appropriate for this device
$(call inherit-product, device/common/gps/gps_eu_supl.mk)

# Inherit device configuration
$(call inherit-product, device/ixion/P350/device.mk)

# Inherit some common AICP stuff.
$(call inherit-product, vendor/aicp/configs/common.mk)

# Inherit telephony stuff
$(call inherit-product, vendor/aicp/configs/gsm.mk)

## Device identifier. This must come after all inclusions
PRODUCT_RELEASE_NAME := P350
PRODUCT_NAME := aicp_P350
PRODUCT_DEVICE := P350
PRODUCT_BRAND := Ixion
PRODUCT_MANUFACTURER := DEXP
PRODUCT_MODEL := DEXP Ixion P350

# Correct bootanimation size for the screen
TARGET_SCREEN_HEIGHT := 1280
TARGET_SCREEN_WIDTH := 720

# Boot animation
include vendor/aicp/configs/bootanimation.mk
