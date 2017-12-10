#
# Copyright (C) 2017 The LineageOS Project
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
#

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    mtk_gui.cpp \
    mtk_ui.cpp \
    mtk_omx.cpp \
    mtk_parcel.cpp \
    mtk_egl.cpp \
    mtk_string.cpp \
    mtk_wvm32.cpp \
    icu55.c \
    icu53.c \
    ssl.c

LOCAL_SHARED_LIBRARIES := libbinder libutils liblog libgui libui \
                          libicuuc libicui18n libcrypto libstagefright_foundation

LOCAL_MODULE := libmtkabi
LOCAL_MODULE_TAGS := optional

LOCAL_CLANG := true
LOCAL_CPPFLAGS := -std=c++1y 
LOCAL_CPPFLAGS += -Wno-exit-time-destructors
LOCAL_CPPFLAGS += -Wno-global-constructors
LOCAL_CPPFLAGS += -Wno-c++98-compat-pedantic
LOCAL_CPPFLAGS += -Wno-four-char-constants
LOCAL_CPPFLAGS += -Wno-padded

include $(BUILD_SHARED_LIBRARY)
