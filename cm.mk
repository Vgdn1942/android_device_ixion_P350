## Specify phone tech before including full_phone

# Release name
PRODUCT_RELEASE_NAME := P350

# Inherit some common CM stuff.
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

# Inherit device configuration
$(call inherit-product, device/ixion/P350/device_P350.mk)

## Device identifier. This must come after all inclusions
PRODUCT_DEVICE := P350
PRODUCT_NAME := cm_P350
PRODUCT_BRAND := Ixion
PRODUCT_MODEL := DEXP Ixion P350
PRODUCT_MANUFACTURER := DEXP
