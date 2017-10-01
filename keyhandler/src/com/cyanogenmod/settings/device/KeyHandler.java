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

import android.app.ActivityManagerNative;
import android.app.KeyguardManager;
import android.content.ActivityNotFoundException;
import android.content.ContentResolver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.Resources;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.hardware.TorchManager;
import android.media.session.MediaSessionLegacyHelper;
import android.net.Uri;
import android.os.Handler;
import android.os.Message;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.os.SystemClock;
import android.os.UserHandle;
import android.os.Vibrator;
import android.provider.MediaStore;
import android.provider.Settings;
import android.util.Log;
import android.view.KeyEvent;
import android.view.WindowManagerGlobal;

import com.android.internal.os.DeviceKeyHandler;
import com.android.internal.util.ArrayUtils;

public class KeyHandler implements DeviceKeyHandler {

    private static final String TAG = KeyHandler.class.getSimpleName();
    private static final int GESTURE_REQUEST = 1;

    private static final String KEY_GESTURE_DOUBLE_TAP =
            "touchscreen_gesture_double_tap";

    private static final String KEY_GESTURE_CAMERA =
            "touchscreen_gesture_camera";

    private static final String KEY_GESTURE_DIALER =
            "touchscreen_gesture_dialer";

    private static final String KEY_GESTURE_FLASHLIGHT =
            "touchscreen_gesture_flashlight";

    private static final String KEY_GESTURE_MUSIC =
            "touchscreen_gesture_music";

    private static final String KEY_GESTURE_HAPTIC_FEEDBACK =
            "touchscreen_gesture_haptic_feedback";

    private static final String ACTION_DISMISS_KEYGUARD =
            "com.android.keyguard.action.DISMISS_KEYGUARD_SECURELY";

    /*
    Supported scancodes

    K - 0x25 - 37 - double tap
    O - 0x18 - 24 - gesture O
    W - 0x11 - 17 - gesture W
    C - 0x2e - 46 - gesture C
    M - 0x32 - 50 - gesture M
    U - 0x16 - 22 - up swipe
    D - 0x20 - 32 - down swipe
    R - 0x13 - 19 - right swipe
    L - 0x26 - 38 - left swipe
    */

    private static final int GESTURE_DOUBLE_TAP = 37;
    private static final int GESTURE_O_SCANCODE = 24;
    private static final int GESTURE_W_SCANCODE = 17;
    private static final int GESTURE_C_SCANCODE = 46;
    private static final int GESTURE_M_SCANCODE = 50;
    private static final int GESTURE_UP_SCANCODE = 22;
    private static final int GESTURE_DOWN_SCANCODE = 32;
    private static final int GESTURE_RIGHT_SCANCODE = 19;
    private static final int GESTURE_LEFT_SCANCODE = 38;

    private static final int GESTURE_WAKELOCK_DURATION = 3000;

    private static final int[] sSupportedGestures = new int[] {
        GESTURE_DOUBLE_TAP,
        GESTURE_O_SCANCODE,
        GESTURE_W_SCANCODE,
        GESTURE_C_SCANCODE,
        GESTURE_M_SCANCODE,
        GESTURE_UP_SCANCODE,
        GESTURE_DOWN_SCANCODE,
        GESTURE_RIGHT_SCANCODE,
        GESTURE_LEFT_SCANCODE
    };

    private final Context mContext;
    private final PowerManager mPowerManager;
    private KeyguardManager mKeyguardManager;
    private Handler mHandler;
    private SensorManager mSensorManager;
    private TorchManager mTorchManager;
    private Sensor mProximitySensor;
    private Vibrator mVibrator;
    WakeLock mProximityWakeLock;
    WakeLock mGestureWakeLock;
    private int mProximityTimeOut;
    private boolean mProximityWakeSupported;

    public KeyHandler(Context context) {
        mContext = context;
        mPowerManager = (PowerManager) context.getSystemService(Context.POWER_SERVICE);
        mHandler = new Handler();
        mGestureWakeLock = mPowerManager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK,
                "GestureWakeLock");

        final Resources resources = mContext.getResources();
        mProximityTimeOut = resources.getInteger(
                com.android.internal.R.integer.config_proximityCheckTimeout);
        mProximityWakeSupported = resources.getBoolean(
                com.android.internal.R.bool.config_proximityCheckOnWake);

        if (mProximityWakeSupported) {
            mSensorManager = (SensorManager) context.getSystemService(Context.SENSOR_SERVICE);
            mProximitySensor = mSensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY);
            mProximityWakeLock = mPowerManager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK,
                    "ProximityWakeLock");
        }

        mVibrator = (Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE);
        if (mVibrator == null || !mVibrator.hasVibrator()) {
            mVibrator = null;
        }
    }

    private void ensureKeyguardManager() {
        if (mKeyguardManager == null) {
            mKeyguardManager =
                    (KeyguardManager) mContext.getSystemService(Context.KEYGUARD_SERVICE);
        }
    }

    private void ensureTorchManager() {
        if (mTorchManager == null) {
            mTorchManager = (TorchManager) mContext.getSystemService(Context.TORCH_SERVICE);
        }
    }

    public boolean handleKeyEvent(KeyEvent event) {
        if (event.getAction() != KeyEvent.ACTION_UP) {
            return false;
        }
        boolean isKeySupported = ArrayUtils.contains(sSupportedGestures, event.getScanCode());
        if (isKeySupported) {
            switch (event.getScanCode()) {
            case GESTURE_DOUBLE_TAP:
            if (!mPowerManager.isScreenOn()) {
                boolean enabled_doubletap = Settings.System.getInt(mContext.getContentResolver(),
                    KEY_GESTURE_DOUBLE_TAP, 1) != 0;
                if (enabled_doubletap) {
                    mPowerManager.wakeUpWithProximityCheck(SystemClock.uptimeMillis());
                    doHapticFeedback();
                    return true;
                }
            }
            case GESTURE_O_SCANCODE:
                boolean enabled_camera = Settings.System.getInt(mContext.getContentResolver(),
                    KEY_GESTURE_CAMERA, 1) != 0;
                if (enabled_camera) {
                    ensureKeyguardManager();
                    final String action;
                    mGestureWakeLock.acquire(GESTURE_WAKELOCK_DURATION);
                    if (mKeyguardManager.isKeyguardSecure() && mKeyguardManager.isKeyguardLocked()) {
                        action = MediaStore.INTENT_ACTION_STILL_IMAGE_CAMERA_SECURE;
                    } else {
                        mContext.sendBroadcastAsUser(new Intent(ACTION_DISMISS_KEYGUARD),
                              UserHandle.CURRENT);
                        action = MediaStore.INTENT_ACTION_STILL_IMAGE_CAMERA;
                    }
                    mPowerManager.wakeUpWithProximityCheck(SystemClock.uptimeMillis());
                    Intent intent = new Intent(action, null);
                    startActivitySafely(intent);
                    doHapticFeedback();
                    return true;
                }
                break;
            case GESTURE_W_SCANCODE:
                boolean enabled_dialer = Settings.System.getInt(mContext.getContentResolver(),
                    KEY_GESTURE_DIALER, 1) != 0;
                if (enabled_dialer) {
                    ensureKeyguardManager();
                    final String action;
                    mGestureWakeLock.acquire(GESTURE_WAKELOCK_DURATION);
                    if (mKeyguardManager.isKeyguardSecure() && mKeyguardManager.isKeyguardLocked()) {
                        action = Intent.ACTION_DIAL;
                    } else {
                        mContext.sendBroadcastAsUser(new Intent(ACTION_DISMISS_KEYGUARD),
                              UserHandle.CURRENT);
                        action = Intent.ACTION_DIAL;
                    }
                    mPowerManager.wakeUpWithProximityCheck(SystemClock.uptimeMillis());
                    Intent intent = new Intent(action, null);
                    startActivitySafely(intent);
                    doHapticFeedback();
                    return true;
                }
                break;
            case GESTURE_C_SCANCODE:
                boolean enabled_flashlight = Settings.System.getInt(mContext.getContentResolver(),
                    KEY_GESTURE_FLASHLIGHT, 1) != 0;
                if (enabled_flashlight) {
                    ensureTorchManager();
                    mGestureWakeLock.acquire(GESTURE_WAKELOCK_DURATION);
                    mTorchManager.toggleTorch();
                    doHapticFeedback();
                    return true;
                }
                break;
            case GESTURE_UP_SCANCODE:
                boolean enabled_music_up = Settings.System.getInt(mContext.getContentResolver(),
                    KEY_GESTURE_MUSIC, 1) != 0;
                if (enabled_music_up) {
                    dispatchMediaKeyWithWakeLockToMediaSession(KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE);
                    doHapticFeedback();
                    return true;
                }
                break;
            case GESTURE_DOWN_SCANCODE:
                boolean enabled_music_down = Settings.System.getInt(mContext.getContentResolver(),
                    KEY_GESTURE_MUSIC, 1) != 0;
                if (enabled_music_down) {
                    dispatchMediaKeyWithWakeLockToMediaSession(KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE);
                    doHapticFeedback();
                    return true;
                }
                break;
            case GESTURE_RIGHT_SCANCODE:
                boolean enabled_music_right = Settings.System.getInt(mContext.getContentResolver(),
                    KEY_GESTURE_MUSIC, 1) != 0;
                if (enabled_music_right) {
                    dispatchMediaKeyWithWakeLockToMediaSession(KeyEvent.KEYCODE_MEDIA_NEXT);
                    doHapticFeedback();
                    return true;
                }
                break;
            case GESTURE_LEFT_SCANCODE:
                boolean enabled_music_left = Settings.System.getInt(mContext.getContentResolver(),
                    KEY_GESTURE_MUSIC, 1) != 0;
                if (enabled_music_left) {
                    dispatchMediaKeyWithWakeLockToMediaSession(KeyEvent.KEYCODE_MEDIA_PREVIOUS);
                    doHapticFeedback();
                    return true;
                }
                break;
            }
            Message msg = getMessageForKeyEvent(event);
            boolean defaultProximity = mContext.getResources().getBoolean(
                com.android.internal.R.bool.config_proximityCheckOnWakeEnabledByDefault);
            boolean proximityWakeCheckEnabled = Settings.System.getInt(mContext.getContentResolver(),
                    Settings.System.PROXIMITY_ON_WAKE, defaultProximity ? 1 : 0) == 1;
            if (mProximityWakeSupported && proximityWakeCheckEnabled && mProximitySensor != null) {
                mHandler.sendMessageDelayed(msg, mProximityTimeOut);
                processEvent(event);
            } else {
                mHandler.sendMessage(msg);
            }
        }
        return isKeySupported;
    }

    private Message getMessageForKeyEvent(KeyEvent keyEvent) {
        Message msg = mHandler.obtainMessage(GESTURE_REQUEST);
        msg.obj = keyEvent;
        return msg;
    }

    private void processEvent(final KeyEvent keyEvent) {
        mProximityWakeLock.acquire();
        mSensorManager.registerListener(new SensorEventListener() {
            @Override
            public void onSensorChanged(SensorEvent event) {
                mProximityWakeLock.release();
                mSensorManager.unregisterListener(this);
                if (!mHandler.hasMessages(GESTURE_REQUEST)) {
                    /* The sensor took to long, ignoring. */
                    return;
                }
                mHandler.removeMessages(GESTURE_REQUEST);
                if (event.values[0] == mProximitySensor.getMaximumRange()) {
                    Message msg = getMessageForKeyEvent(keyEvent);
                    mHandler.sendMessage(msg);
                }
            }

            @Override
            public void onAccuracyChanged(Sensor sensor, int accuracy) {}

        }, mProximitySensor, SensorManager.SENSOR_DELAY_FASTEST);
    }

    private void dispatchMediaKeyWithWakeLockToMediaSession(int keycode) {
        MediaSessionLegacyHelper helper = MediaSessionLegacyHelper.getHelper(mContext);
        if (helper != null) {
            KeyEvent event = new KeyEvent(SystemClock.uptimeMillis(),
                    SystemClock.uptimeMillis(), KeyEvent.ACTION_DOWN, keycode, 0);
            helper.sendMediaButtonEvent(event, true);
            event = KeyEvent.changeAction(event, KeyEvent.ACTION_UP);
            helper.sendMediaButtonEvent(event, true);
        } else {
            Log.w(TAG, "Unable to send media key event");
        }
    }

    private void startActivitySafely(Intent intent) {
        intent.addFlags(
                Intent.FLAG_ACTIVITY_NEW_TASK
                | Intent.FLAG_ACTIVITY_SINGLE_TOP
                | Intent.FLAG_ACTIVITY_CLEAR_TOP);
        try {
            UserHandle user = new UserHandle(UserHandle.USER_CURRENT);
            mContext.startActivityAsUser(intent, null, user);
        } catch (ActivityNotFoundException e) {
            /* Ignore */
        }
    }

    private void doHapticFeedback() {
        if (mVibrator == null) {
            return;
        }
        boolean enabled_haptic = Settings.System.getInt(mContext.getContentResolver(),
                KEY_GESTURE_HAPTIC_FEEDBACK, 1) != 0;
        if (enabled_haptic) {
            mVibrator.vibrate(50);
        }
    }
}
