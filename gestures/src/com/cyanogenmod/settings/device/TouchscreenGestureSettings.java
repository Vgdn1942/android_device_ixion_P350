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

package com.cyanogenmod.settings.device;

import com.cyanogenmod.settings.device.utils.NodePreferenceActivity;

import android.os.Bundle;
import android.provider.Settings;
import android.preference.Preference;
import android.preference.SwitchPreference;

public class TouchscreenGestureSettings extends NodePreferenceActivity {
    private static final String KEY_DOUBLE_TAP_GESTURE = "touchscreen_gesture_double_tap";
    private static final String KEY_CAMERA_GESTURE = "touchscreen_gesture_camera";
    private static final String KEY_DIALER_GESTURE = "touchscreen_gesture_dialer";
    private static final String KEY_FLASHLIGHT_GESTURE = "touchscreen_gesture_flashlight";
    private static final String KEY_MUSIC_GESTURE = "touchscreen_gesture_music";
    private static final String KEY_HAPTIC_FEEDBACK = "touchscreen_gesture_haptic_feedback";

    private SwitchPreference mDoubleTapGesture;
    private SwitchPreference mCameraGesture;
    private SwitchPreference mDialerGesture;
    private SwitchPreference mFlashlightGesture;
    private SwitchPreference mMusicGesture;
    private SwitchPreference mHapticFeedback;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        addPreferencesFromResource(R.xml.touchscreen_panel);

        mDoubleTapGesture = (SwitchPreference) findPreference(KEY_DOUBLE_TAP_GESTURE);
        mDoubleTapGesture.setOnPreferenceChangeListener(this);

        mCameraGesture = (SwitchPreference) findPreference(KEY_CAMERA_GESTURE);
        mCameraGesture.setOnPreferenceChangeListener(this);

        mDialerGesture = (SwitchPreference) findPreference(KEY_DIALER_GESTURE);
        mDialerGesture.setOnPreferenceChangeListener(this);

        mFlashlightGesture = (SwitchPreference) findPreference(KEY_FLASHLIGHT_GESTURE);
        mFlashlightGesture.setOnPreferenceChangeListener(this);

        mMusicGesture = (SwitchPreference) findPreference(KEY_MUSIC_GESTURE);
        mMusicGesture.setOnPreferenceChangeListener(this);

        mHapticFeedback = (SwitchPreference) findPreference(KEY_HAPTIC_FEEDBACK);
        mHapticFeedback.setOnPreferenceChangeListener(this);
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        final String key = preference.getKey();
        if (KEY_DOUBLE_TAP_GESTURE.equals(key)) {
            final boolean value = (Boolean) newValue;
            Settings.System.putInt(getContentResolver(), KEY_DOUBLE_TAP_GESTURE, value ? 1 : 0);
            return true;
        }
        if (KEY_CAMERA_GESTURE.equals(key)) {
            final boolean value = (Boolean) newValue;
            Settings.System.putInt(getContentResolver(), KEY_CAMERA_GESTURE, value ? 1 : 0);
            return true;
        }
        if (KEY_DIALER_GESTURE.equals(key)) {
            final boolean value = (Boolean) newValue;
            Settings.System.putInt(getContentResolver(), KEY_DIALER_GESTURE, value ? 1 : 0);
            return true;
        }
        if (KEY_FLASHLIGHT_GESTURE.equals(key)) {
            final boolean value = (Boolean) newValue;
            Settings.System.putInt(getContentResolver(), KEY_FLASHLIGHT_GESTURE, value ? 1 : 0);
            return true;
        }
        if (KEY_MUSIC_GESTURE.equals(key)) {
            final boolean value = (Boolean) newValue;
            Settings.System.putInt(getContentResolver(), KEY_MUSIC_GESTURE, value ? 1 : 0);
            return true;
        }
        if (KEY_HAPTIC_FEEDBACK.equals(key)) {
            final boolean value = (Boolean) newValue;
            Settings.System.putInt(getContentResolver(), KEY_HAPTIC_FEEDBACK, value ? 1 : 0);
            return true;
        }

        return super.onPreferenceChange(preference, newValue);
    }

    @Override
    protected void onResume() {
        super.onResume();

        mDoubleTapGesture.setChecked(
                Settings.System.getInt(getContentResolver(), KEY_DOUBLE_TAP_GESTURE, 1) != 0);

        mCameraGesture.setChecked(
                Settings.System.getInt(getContentResolver(), KEY_CAMERA_GESTURE, 1) != 0);

        mDialerGesture.setChecked(
                Settings.System.getInt(getContentResolver(), KEY_DIALER_GESTURE, 1) != 0);

        mFlashlightGesture.setChecked(
                Settings.System.getInt(getContentResolver(), KEY_FLASHLIGHT_GESTURE, 1) != 0);

        mMusicGesture.setChecked(
                Settings.System.getInt(getContentResolver(), KEY_MUSIC_GESTURE, 1) != 0);

        mHapticFeedback.setChecked(
                Settings.System.getInt(getContentResolver(), KEY_HAPTIC_FEEDBACK, 1) != 0);
    }
}
