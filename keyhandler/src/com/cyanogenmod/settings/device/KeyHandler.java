/*
 * Copyright (C) 2015-2016 The CyanogenMod Project
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

import android.Manifest;
import android.app.NotificationManager;
import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.hardware.camera2.CameraAccessException;
import android.hardware.camera2.CameraCharacteristics;
import android.hardware.camera2.CameraManager;
import android.media.session.MediaSessionLegacyHelper;
import android.os.Handler;
import android.os.Message;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.os.SystemClock;
import android.os.Vibrator;
import android.provider.Settings;
import android.util.Log;
import android.util.SparseIntArray;
import android.view.KeyEvent;

import com.android.internal.os.DeviceKeyHandler;
import com.android.internal.util.ArrayUtils;

import cyanogenmod.providers.CMSettings;

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

    private static final int MODE_TOTAL_SILENCE = 600;
    private static final int MODE_ALARMS_ONLY = 601;
    private static final int MODE_PRIORITY_ONLY = 602;
    private static final int MODE_NONE = 603;

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

    private static final SparseIntArray sSupportedSliderModes = new SparseIntArray();
    static {
        sSupportedSliderModes.put(MODE_TOTAL_SILENCE, Settings.Global.ZEN_MODE_NO_INTERRUPTIONS);
        sSupportedSliderModes.put(MODE_ALARMS_ONLY, Settings.Global.ZEN_MODE_ALARMS);
        sSupportedSliderModes.put(MODE_PRIORITY_ONLY,
                Settings.Global.ZEN_MODE_IMPORTANT_INTERRUPTIONS);
        sSupportedSliderModes.put(MODE_NONE, Settings.Global.ZEN_MODE_OFF);
    }

    private final Context mContext;
    private final PowerManager mPowerManager;
    private final NotificationManager mNotificationManager;
    private EventHandler mEventHandler;
    private SensorManager mSensorManager;
    private CameraManager mCameraManager;
    private String mRearCameraId;
    private boolean mTorchEnabled;
    private Sensor mProximitySensor;
    private Vibrator mVibrator;
    WakeLock mProximityWakeLock;
    WakeLock mGestureWakeLock;
    private int mProximityTimeOut;
    private boolean mProximityWakeSupported;

    public KeyHandler(Context context) {
        mContext = context;
        mPowerManager = (PowerManager) context.getSystemService(Context.POWER_SERVICE);
        mNotificationManager
                = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
        mEventHandler = new EventHandler();
        mGestureWakeLock = mPowerManager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK,
                "GestureWakeLock");

        final Resources resources = mContext.getResources();
        mProximityTimeOut = resources.getInteger(
                org.cyanogenmod.platform.internal.R.integer.config_proximityCheckTimeout);
        mProximityWakeSupported = resources.getBoolean(
                org.cyanogenmod.platform.internal.R.bool.config_proximityCheckOnWake);

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

        mCameraManager = (CameraManager) mContext.getSystemService(Context.CAMERA_SERVICE);
        mCameraManager.registerTorchCallback(new MyTorchCallback(), mEventHandler);
    }

    private class MyTorchCallback extends CameraManager.TorchCallback {
        @Override
        public void onTorchModeChanged(String cameraId, boolean enabled) {
            if (!cameraId.equals(mRearCameraId))
                return;
            mTorchEnabled = enabled;
        }

        @Override
        public void onTorchModeUnavailable(String cameraId) {
            if (!cameraId.equals(mRearCameraId))
                return;
            mTorchEnabled = false;
        }
    }

    private String getRearCameraId() {
        if (mRearCameraId == null) {
            try {
                for (final String cameraId : mCameraManager.getCameraIdList()) {
                    CameraCharacteristics characteristics =
                            mCameraManager.getCameraCharacteristics(cameraId);
                    int cOrientation = characteristics.get(CameraCharacteristics.LENS_FACING);
                    if (cOrientation == CameraCharacteristics.LENS_FACING_BACK) {
                        mRearCameraId = cameraId;
                        break;
                    }
                }
            } catch (CameraAccessException e) {
                // Ignore
            }
        }
        return mRearCameraId;
    }

    private class EventHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.arg1) {
            case GESTURE_DOUBLE_TAP:
                if (!mPowerManager.isScreenOn()) {
                    boolean enabled_doubletap = Settings.System.getInt(mContext.getContentResolver(),
                        KEY_GESTURE_DOUBLE_TAP, 1) != 0;
                    if (enabled_doubletap) {
                        mPowerManager.wakeUpWithProximityCheck(SystemClock.uptimeMillis(), "wakeup-gesture-proximity");
                        doHapticFeedback();
                    }
                }
                break;
            case GESTURE_O_SCANCODE:
                boolean enabled_camera = Settings.System.getInt(mContext.getContentResolver(),
                    KEY_GESTURE_CAMERA, 1) != 0;
                if (enabled_camera) {
                mGestureWakeLock.acquire(GESTURE_WAKELOCK_DURATION);

                Intent intent = new Intent(cyanogenmod.content.Intent.ACTION_SCREEN_CAMERA_GESTURE);
                mContext.sendBroadcast(intent, Manifest.permission.STATUS_BAR_SERVICE);
                doHapticFeedback();
                }
                break;
            case GESTURE_C_SCANCODE:
                boolean enabled_flashlight = Settings.System.getInt(mContext.getContentResolver(),
                    KEY_GESTURE_FLASHLIGHT, 1) != 0;
                if (enabled_flashlight) {
                    String rearCameraId = getRearCameraId();
                    if (rearCameraId != null) {
                        mGestureWakeLock.acquire(GESTURE_WAKELOCK_DURATION);
                    try {
                        mCameraManager.setTorchMode(rearCameraId, !mTorchEnabled);
                        mTorchEnabled = !mTorchEnabled;
                    } catch (CameraAccessException e) {
                    // Ignore
                    }
                    doHapticFeedback();
                    }
                }
                break;
            case GESTURE_UP_SCANCODE:
                boolean enabled_music_up = Settings.System.getInt(mContext.getContentResolver(),
                    KEY_GESTURE_MUSIC, 1) != 0;
                if (enabled_music_up) {
                    dispatchMediaKeyWithWakeLockToMediaSession(KeyEvent.KEYCODE_MEDIA_STOP);
                    doHapticFeedback();
                }
                break;
            case GESTURE_DOWN_SCANCODE:
                boolean enabled_music_down = Settings.System.getInt(mContext.getContentResolver(),
                    KEY_GESTURE_MUSIC, 1) != 0;
                if (enabled_music_down) {
                    dispatchMediaKeyWithWakeLockToMediaSession(KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE);
                    doHapticFeedback();
                }
                break;
            case GESTURE_RIGHT_SCANCODE:
                boolean enabled_music_right = Settings.System.getInt(mContext.getContentResolver(),
                    KEY_GESTURE_MUSIC, 1) != 0;
                if (enabled_music_right) {
                    dispatchMediaKeyWithWakeLockToMediaSession(KeyEvent.KEYCODE_MEDIA_NEXT);
                    doHapticFeedback();
                }
                break;
            case GESTURE_LEFT_SCANCODE:
                boolean enabled_music_left = Settings.System.getInt(mContext.getContentResolver(),
                    KEY_GESTURE_MUSIC, 1) != 0;
                if (enabled_music_left) {
                    dispatchMediaKeyWithWakeLockToMediaSession(KeyEvent.KEYCODE_MEDIA_PREVIOUS);
                    doHapticFeedback();
                }
                break;
            }
        }
    }

    private boolean hasSetupCompleted() {
        return CMSettings.Secure.getInt(mContext.getContentResolver(),
            CMSettings.Secure.CM_SETUP_WIZARD_COMPLETED, 0) != 0;
    }

    public boolean handleKeyEvent(KeyEvent event) {
        int scanCode = event.getScanCode();
        boolean isKeySupported = ArrayUtils.contains(sSupportedGestures, scanCode);
        boolean isSliderModeSupported = sSupportedSliderModes.indexOfKey(scanCode) >= 0;
        if (!isKeySupported && !isSliderModeSupported) {
            return false;
        }

        if (!hasSetupCompleted()) {
            return false;
        }

        if (isSliderModeSupported) {
            mNotificationManager.setZenMode(sSupportedSliderModes.get(scanCode), null, TAG);
            doHapticFeedback();
        } else if (!mEventHandler.hasMessages(GESTURE_REQUEST)) {
            Message msg = getMessageForKeyEvent(scanCode);
            boolean defaultProximity = mContext.getResources().getBoolean(
                org.cyanogenmod.platform.internal.R.bool.config_proximityCheckOnWakeEnabledByDefault);
            boolean proximityWakeCheckEnabled = CMSettings.System.getInt(mContext.getContentResolver(),
                    CMSettings.System.PROXIMITY_ON_WAKE, defaultProximity ? 1 : 0) == 1;
            if (mProximityWakeSupported && proximityWakeCheckEnabled && mProximitySensor != null) {
                mEventHandler.sendMessageDelayed(msg, mProximityTimeOut);
                processEvent(scanCode);
            } else {
                mEventHandler.sendMessage(msg);
            }
        }
        return true;
    }

    private Message getMessageForKeyEvent(int scancode) {
        Message msg = mEventHandler.obtainMessage(GESTURE_REQUEST);
        msg.arg1 = scancode;
        return msg;
    }

    private void processEvent(final int scancode) {
        mProximityWakeLock.acquire();
        mSensorManager.registerListener(new SensorEventListener() {
            @Override
            public void onSensorChanged(SensorEvent event) {
                mProximityWakeLock.release();
                mSensorManager.unregisterListener(this);
                if (!mEventHandler.hasMessages(GESTURE_REQUEST)) {
                    // The sensor took to long, ignoring.
                    return;
                }
                mEventHandler.removeMessages(GESTURE_REQUEST);
                if (event.values[0] == mProximitySensor.getMaximumRange()) {
                    Message msg = getMessageForKeyEvent(scancode);
                    mEventHandler.sendMessage(msg);
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

    private void doHapticFeedback() {
        if (mVibrator == null) {
            return;
        }
        boolean enabled_haptic = Settings.System.getInt(mContext.getContentResolver(),
                KEY_GESTURE_HAPTIC_FEEDBACK, 1) != 0;
        if (enabled_haptic) {
            mVibrator.vibrate(100);
        }
    }
}
