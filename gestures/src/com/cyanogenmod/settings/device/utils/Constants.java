/*
 * Copyright (C) 2015 The CyanogenMod Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.cyanogenmod.settings.device.utils;

import java.util.HashMap;
import java.util.Map;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;

public class Constants {

    // Preference keys
    public static final String ENABLE_GESTURE_KEY = "touchscreen_enable_gesture";

    // Proc nodes
    public static final String ENABLE_GESTURE_NODE = "/sys/class/tpd_gesture/gesture_mode";

    // Proc nodes default values
    public static final boolean ENABLE_GESTURE_DEFAULT = true;

    // Holds <preference_key> -> <proc_node> mapping
    public static final Map<String, String> sNodePreferenceMap = new HashMap<String, String>();

    // Holds <preference_key> -> <default_values> mapping
    public static final Map<String, Boolean> sNodeDefaultMap = new HashMap<String, Boolean>();

    static {
        sNodePreferenceMap.put(ENABLE_GESTURE_KEY, ENABLE_GESTURE_NODE);

        sNodeDefaultMap.put(ENABLE_GESTURE_KEY, ENABLE_GESTURE_DEFAULT);
    }

    public static boolean isPreferenceEnabled(Context context, String key, boolean defaultValue) {
        SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(context);
        return preferences.getBoolean(key, defaultValue);
    }
}
