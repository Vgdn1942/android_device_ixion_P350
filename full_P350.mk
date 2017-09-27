# Release name
PRODUCT_RELEASE_NAME := P350

# Inherit some common AICP stuff.
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)

# Inherit device configuration
$(call inherit-product, device/ixion/P350/device.mk)

# Device identifier. This must come after all inclusions
PRODUCT_NAME := full_P350
PRODUCT_DEVICE := P350
PRODUCT_BRAND := Ixion
PRODUCT_MANUFACTURER := DEXP
PRODUCT_MODEL := DEXP Ixion P350
