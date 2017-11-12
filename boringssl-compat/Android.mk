# Copyright (C) 2008 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

EVP_FILES := \
    p_dec.c \
    p_enc.c \
    p_open.c \
    p_seal.c

RSA_FILES := rsa_pss.c

B64_FILES := bio_b64.c

LOCAL_SRC_FILES := $(EVP_FILES) $(RSA_FILES)

ifeq ($(TARGET_REQUIRES_B64_COMPAT),true)
LOCAL_SRC_FILES += $(B64_FILES)
endif

LOCAL_SHARED_LIBRARIES := liblog libcrypto
LOCAL_MODULE := libboringssl-compat
LOCAL_MODULE_TAGS := optional
include $(BUILD_SHARED_LIBRARY)

