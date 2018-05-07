/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

//#define LOG_NDEBUG 0
#define LOG_TAG "FMPlayer"
#include "utils/Log.h"

#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <system/audio.h>

#include <binder/IServiceManager.h>
//#include <hardware/audio.h>
#include <AudioSystem.h>
#include "FMAudioPlayer.h"

#ifndef CHANGE_AUDIO_PRIORITY
#include <sys/time.h>
#include <sys/resource.h>
#endif

static pid_t myTid()
{
    return gettid();
}

static long long getTimeMs()
{
    struct timeval t1;
    long long ms;

    gettimeofday(&t1, NULL);
    ms = t1.tv_sec * 1000LL + t1.tv_usec / 1000;

    return ms;
}

// ----------------------------------------------------------------------------

extern const int fm_use_analog_input;//from libaudiosetting.so
extern const int fm_chip_519x;//from libaudiosetting.so

namespace android
{

// workaround to avoid linking against libaudiosetting
const int matv_use_analog_input = 0;
const int fm_use_analog_input = 0;
const int fm_chip_519x = 0;
const int using_class_ab_amp = 0;

typedef struct _AUDIO_DEVICE_CHANGE_CALLBACK_STRUCT {
		void (*callback)(void *data);
} AUDIO_DEVICE_CHANGE_CALLBACK_STRUCT;

enum AudioCommand {
    HOOK_FM_DEVICE_CALLBACK    = 0x71,
    UNHOOK_FM_DEVICE_CALLBACK  = 0x72
};

// ----------------------------------------------------------------------------

#define sineTable48KSIZE 480
#define sineTable32KSIZE 320

static bool getValue(String8 str)
{
	bool ret = false;
	char string[str.size() + 1];
	memcpy(string, str.string(), str.size());
	string[str.size()] = 0;
	ALOGD("string is %s", string);

	if(strlen(string) != 0)
	{
		size_t eqIdx = strcspn(string, "=");
		String8 key = String8(string, eqIdx);
		ALOGD("Key is %s", key.string());
		String8 value;

		if(eqIdx == strlen(string))
		{
			ALOGE("Invailed value");
		}
		else
		{
			value = String8(string + eqIdx + 1);
			ALOGD("Value is %s", value.string());
			if(!strcasecmp(value.string(), "true"))
			{
				ret = true;
			}
			else if(!strcasecmp(value.string(), "false"))
			{
				ret = false;
			}
			else
				ALOGE("Invailed value");
		}
	}
	return ret;
}
static int GetReadBufferSize()
{
    return 384 * sizeof(uint16_t) * 2;        // 2 x sine table size
}

// TODO: Determine appropriate return codes
static status_t ERROR_NOT_OPEN = -1;
static status_t ERROR_OPEN_FAILED = -2;
static status_t ERROR_ALLOCATE_FAILED = -4;
static status_t ERROR_NOT_SUPPORTED = -8;
static status_t ERROR_NOT_READY = -16;
static status_t ERROR_START_FAILED = -32;
static status_t ERROR_STOP_FAILED = -64;
static status_t STATE_INIT = 0;
static status_t STATE_ERROR = 1;
static status_t STATE_OPEN = 2;
static status_t STATE_PLAY = 3;
static status_t STATE_STOP = 4;


String8  ANALOG_FM_ENABLE = (String8)("AudioSetFmEnable=1");
String8  ANALOG_FM_DISABLE = (String8)("AudioSetFmEnable=0");

String8  DIGITAL_FM_ENABLE = (String8)("AudioSetFmDigitalEnable=1");
String8  DIGITAL_FM_DISABLE = (String8)("AudioSetFmDigitalEnable=0");

String8 IS_WIRED_HEADSET_ON = (String8)("AudioFmIsWiredHeadsetOn");

FMAudioPlayer *FmAudioPlayerInstance = NULL;

void FMAudioPlayerCallback(void *data)
{
    ALOGD("FMAudioPlayer callback 0x%x\n", FmAudioPlayerInstance);

    if (FmAudioPlayerInstance != NULL)
    {
        FmAudioPlayerInstance->setRender((bool)data);
    }
}

FMAudioPlayer::FMAudioPlayer() :
    mAudioBuffer(NULL), mPlayTime(-1), mDuration(-1), mState(STATE_ERROR),
    mStreamType(AUDIO_STREAM_MUSIC),mAudioRecord(NULL),flagRecordError(false),
    mExit(false), mPaused(false), mRender(false), mRenderTid(-1)
{
    ALOGD("[%d]FMAudioPlayer constructor\n");

    if (fm_use_analog_input == 1)
    {
        ALOGD("FM use analog input");
    }

    else if (fm_use_analog_input == 0)
    {
        ALOGD("I2S driver doesn't exists\n");
    }

    FmAudioPlayerInstance = this;
    mMutePause = 0;
    mSetRender = false;
}

void FMAudioPlayer::onFirstRef()
{
    ALOGD("onFirstRef");

    // create playback thread
    Mutex::Autolock l(mMutex);

    if (fm_chip_519x == 0)
    {
        mFmAudioSamplingRate = 44100;
    }
    else
    {
        mFmAudioSamplingRate = 32000;
    }

    setHwCallback(true);

    if (fm_use_analog_input == 1)
    {
        ALOGD("FMAudioPlayer use analog input - onFirstRef");
    }
    else if (fm_use_analog_input == 0)
    {
        ALOGD("FMAudioPlayer use digital input - onFirstRef");
    }

    createThreadEtc(renderThread, this, "FM audio player", ANDROID_PRIORITY_AUDIO);
    mCondition.waitRelative(mMutex, seconds(3));

    if (mRenderTid > 0)
    {
        ALOGD("render thread(%d) started", mRenderTid);
        mState = STATE_INIT;
    }
}

status_t FMAudioPlayer::initCheck()
{
    if (mState != STATE_ERROR)
    {
        return NO_ERROR;
    }

    return ERROR_NOT_READY;
}

FMAudioPlayer::~FMAudioPlayer()
{
    ALOGD("FMAudioPlayer destructor");
    FmAudioPlayerInstance = NULL;
    release();

    if (fm_use_analog_input == 1)
    {
        ALOGD("FMAudioPlayer use analog input - destructor end\n");
    }

    else if (fm_use_analog_input == 0)
    {
        ALOGD("FMAudioPlayer destructor end\n");
    }
}

status_t FMAudioPlayer::setDataSource(const sp<IMediaHTTPService> &httpService, const char *url, const KeyedVector<String8, String8> *headers)
{
    //ALOGD("FMAudioPlayer setDataSource path=%s \n", path);
    return setdatasource(url, -1, 0, 0x7ffffffffffffffLL); // intentionally less than LONG_MAX
}

status_t FMAudioPlayer::setDataSource(int fd, int64_t offset, int64_t length)
{
    ALOGD("FMAudioPlayer setDataSource offset=%d, length=%d \n", ((int)offset), ((int)length));
    return setdatasource(NULL, fd, offset, length);
}


status_t FMAudioPlayer::setdatasource(const char *path, int fd, int64_t offset, int64_t length)
{
    ALOGD("setdatasource");

    // file still open?
    Mutex::Autolock l(mMutex);

    if (mState == STATE_OPEN)
    {
        reset_nosync();
    }

    mState = STATE_OPEN;
    return NO_ERROR;
}

status_t FMAudioPlayer::prepare()
{
    ALOGD("prepare\n");

    if (mState != STATE_OPEN)
    {
        ALOGE("prepare ERROR_NOT_OPEN \n");
        return ERROR_NOT_OPEN;
    }

    return NO_ERROR;
}

status_t FMAudioPlayer::prepareAsync()
{
    ALOGD("prepareAsync\n");

    // can't hold the lock here because of the callback
    // it's safe because we don't change state
    if (mState != STATE_OPEN && mState != STATE_PLAY && mState != STATE_STOP)
    {
        sendEvent(MEDIA_ERROR);
        ALOGD("prepareAsync sendEvent(MEDIA_ERROR) \n");
        return NO_ERROR;
    }

    sendEvent(MEDIA_PREPARED);
    return NO_ERROR;
}

status_t FMAudioPlayer::start()
{
    ALOGD("start\n");
    Mutex::Autolock l(mMutex);

    if (mState != STATE_OPEN && mState != STATE_PLAY && mState != STATE_STOP)
    {
        ALOGE("start ERROR_NOT_OPEN \n");
        return ERROR_NOT_OPEN;
    }

    if (fm_use_analog_input == 1)
    {
        status_t result;    
        AudioSystem::setParameters(0, ANALOG_FM_ENABLE);
        mPaused = false;
    }
    else if (fm_use_analog_input == 0)
    {
        status_t result;

        if (mMutePause == true)
        {
            mMutePause = false;
            mPaused = false;
			//if(mAudioSink != NULL)
            //mAudioSink->setVolume(1.0, 1.0);
        }

        AudioSystem::setParameters(0, DIGITAL_FM_ENABLE);
    }

    mPaused = false;
#if !defined(FM_DIRECT_HW_CONNECT)
	String8 str = AudioSystem::getParameters(0, IS_WIRED_HEADSET_ON);
	ALOGD("mSetRender is %d", mSetRender);
//	if(!getValue(str))
	if(mSetRender)
    {
        // wake up render thread
        ALOGD("start wakeup render thread---\n");
        mCondition.signal();
    }
#else
	mRender = true;
    // wake up render thread
    ALOGD("start wakeup render thread\n");
    mCondition.signal();

#endif
    mState = STATE_PLAY;
    return NO_ERROR;
}

status_t FMAudioPlayer::stop()
{
    ALOGD("stop\n");
    Mutex::Autolock l(mMutex);

    if (mState != STATE_OPEN && mState != STATE_PLAY && mState != STATE_STOP)
    {
        ALOGE("stop ERROR_NOT_OPEN \n");
        return ERROR_NOT_OPEN;
    }
	
	if(mAudioRecord.get())
	{
		mAudioRecord->stop();
		deleteAudioRecord();  //delete for other APP related AudioRecord
	}
	if(mAudioSink != NULL)
//	mAudioSink->stop();

    if (fm_use_analog_input == 1)
    {
        AudioSystem::setParameters(0, ANALOG_FM_DISABLE);
    }
    else
    {
        AudioSystem::setParameters(0, DIGITAL_FM_DISABLE);
    }

    mPaused = true;
    mRender = false;
    mState = STATE_STOP;
    return NO_ERROR;
}

status_t FMAudioPlayer::seekTo(int position)
{
    ALOGD("seekTo %d\n", position);
    return NO_ERROR;
}

status_t FMAudioPlayer::pause()
{
    ALOGD("pause\n");
    Mutex::Autolock l(mMutex);

    if (mState != STATE_OPEN && mState != STATE_PLAY && mState != STATE_STOP)
    {
        ALOGD("pause ERROR_NOT_OPEN \n");
        return ERROR_NOT_OPEN;
    }

    ALOGD("pause got lock\n");

    if (fm_use_analog_input == 1)
    {
        AudioSystem::setParameters(0, ANALOG_FM_DISABLE);
    }
    else if (fm_use_analog_input == 0)
    {
        AudioSystem::setParameters(0, DIGITAL_FM_DISABLE);
    }

    mState = STATE_STOP;
    mPaused = true;
    return NO_ERROR;
}

bool FMAudioPlayer::isPlaying()
{
    ALOGD("isPlaying\n");

    if (mState == STATE_PLAY)
    {
        return true;
    }

    return false;
}

status_t FMAudioPlayer::getCurrentPosition(int *position)
{
    ALOGD("getCurrentPosition always return 0\n");
    *position = 0;
    return NO_ERROR;
}

status_t FMAudioPlayer::getDuration(int *duration)
{
    *duration = 1000;
    ALOGD("getDuration duration, always return 1000 \n");
    return NO_ERROR;
}

status_t FMAudioPlayer::release()
{
    ALOGD("release\n");

    int ret = 0;
    int count = 100;
    ALOGD("release mMutex.tryLock ()");
	
#ifndef CHANGE_AUDIO_PRIORITY
	int priority = getpriority(PRIO_PROCESS, 0);
	ALOGD("FM Render Thread priority is %d", priority);
#endif

    setHwCallback(false);

    do
    {
        ret = mMutex.tryLock();

        if (ret)
        {
            ALOGW("FMAudioPlayer::release() mMutex return ret = %d", ret);
            usleep(20 * 1000);
            count --;
        }
    }
    while (ret && count);  // only cannot lock

    reset_nosync();

    // TODO: timeout when thread won't exit, wait for render thread to exit
    if (mRenderTid > 0)
    {
        mExit = true;
        ALOGD("release signal \n");
        mCondition.signal();
        ALOGD("release wait \n");
        mCondition.waitRelative(mMutex, seconds(3));
    }

    mMutex.unlock();
    return NO_ERROR;
}

status_t FMAudioPlayer::reset()
{
    ALOGD("reset\n");
    Mutex::Autolock l(mMutex);
    return reset_nosync();
}

// always call with lock held
status_t FMAudioPlayer::reset_nosync()
{
    ALOGD("reset_nosync start\n");

    if (fm_use_analog_input == 1)
    {
        AudioSystem::setParameters(0, ANALOG_FM_DISABLE);//Add by Changqing
    }
    else if (fm_use_analog_input == 0)
    {
        AudioSystem::setParameters(0, DIGITAL_FM_DISABLE);//Add by Changqing
    }

    mState = STATE_ERROR;
    mPlayTime = -1;
    mDuration = -1;
    mPaused = false;
    mRender = false;
    ALOGD("reset_nosync end\n");
    return NO_ERROR;
}

status_t FMAudioPlayer::setLooping(int loop)
{
    ALOGD("setLooping, do nothing \n");
    return NO_ERROR;
}

void FMAudioPlayer::setHwCallback(bool enable)
{
    AUDIO_DEVICE_CHANGE_CALLBACK_STRUCT callback_data;
    callback_data.callback = FMAudioPlayerCallback;
}

status_t FMAudioPlayer::setRender(bool enable)
{
    ALOGD("setRender %d when mRender %d\n", enable, mRender);
    if (enable)
    {
        mSetRender = true;
        if(mState != STATE_PLAY)
            return NO_ERROR;
        if(mRender == false){
//            Mutex::Autolock l(mMutex);
            mCondition.signal();
        }
    }
    else
    {
        mSetRender = false;
    }

    return NO_ERROR;
}

#define FM_AUDIO_CHANNEL_NUM      2

status_t FMAudioPlayer::createOutputTrack()
{
    // base on configuration define samplerate .
    int FM_AUDIO_SAMPLING_RATE;

    if (fm_chip_519x == 0)
    {
        FM_AUDIO_SAMPLING_RATE = 44100;
    }
    else
    {
        FM_AUDIO_SAMPLING_RATE = 32000;
    }

    ALOGD("Create AudioTrack object: rate=%d, channels=%d\n", FM_AUDIO_SAMPLING_RATE, FM_AUDIO_CHANNEL_NUM);

    // open audio track
    if (mAudioSink->open(FM_AUDIO_SAMPLING_RATE, FM_AUDIO_CHANNEL_NUM, AUDIO_CHANNEL_OUT_STEREO, AUDIO_FORMAT_PCM_16_BIT, 6) != NO_ERROR)
    {
        ALOGE("mAudioSink open failed");
        return ERROR_OPEN_FAILED;
    }

    return NO_ERROR;
}

int FMAudioPlayer::renderThread(void *p)
{
    return ((FMAudioPlayer *)p)->render();
}

//#define AUDIOBUFFER_SIZE 4096
int FMAudioPlayer::render()
{
    int result = -1;
    int temp;
    int current_section = 0;
    bool audioStarted = false;
    bool firstOutput = false;
    int t_result = -1;
    int bufSize = 0;
    int lastTime = 0;
    int thisTime = 0;
    int dataCount = 0;
    int frameCount = 0;

    bufSize = GetReadBufferSize();
    ALOGD("got buffer size = %d", bufSize);
    mAudioBuffer = new char[bufSize * 2];
    mDummyBuffer = new char[bufSize * 2];
    memset(mDummyBuffer, 0, bufSize);

    ALOGD("mAudioBuffer: %p \n", mAudioBuffer);

    if (!mAudioBuffer)
    {
        ALOGD("mAudioBuffer allocate failed\n");
        goto threadExit;
    }

    // let main thread know we're ready
    {
        int ret = 0;
        int count = 100;
        ALOGD("render mMutex.tryLock ()");

        do
        {
            ret = mMutex.tryLock();

            if (ret)
            {
                ALOGW("FMAudioPlayer::render() mMutex return ret = %d", ret);
                usleep(20 * 1000);
                count --;
            }
        }while (ret && count);  // only cannot lock

        mRenderTid = myTid();
        ALOGD("render start mRenderTid=%d\n",mRenderTid);
        mCondition.signal();
        mMutex.unlock();
    }

    while (1)
    {
        long numread = 0;
		bool flagOfRecord = false;
        {
            Mutex::Autolock l(mMutex);

            // pausing?
            if (mPaused || (!mSetRender && audioStarted) || flagRecordError)
            {
                ALOGD("render - pause\n");

                if (mAudioSink->ready())
                {
                    mAudioSink->pause();
					usleep(300*1000);		//For ALPS00821792
                    mAudioSink->flush();
                }

				if(mAudioRecord.get())
				{
					mAudioRecord->stop();
					deleteAudioRecord();
				}
                mRender = false;
                audioStarted = false;
            }

            // nothing to render, wait for client thread to wake us up
            if( mSetRender && !mPaused && !flagRecordError)
                mRender = true;
            if (!mExit && !mRender)
            {
                ALOGD("render - signal wait\n");
                mCondition.wait(mMutex);
                frameCount = 0;
                flagRecordError = false;
                ALOGD("render - signal rx'd\n");
            }

			if(!mPaused && !mExit)
			{
				//flagOfRecord = createAudioRecord();
				if(false == flagOfRecord)
				{
					ALOGE("Create AudioRecord Failed !!!");
					break;
				}
			}

            if (mExit)
            {
                break;
            }

            // We could end up here if start() is called, and before we get a
            // chance to run, the app calls stop() or reset(). Re-check render
            // flag so we don't try to render in stop or reset state.
            if (!mRender)
            {
                continue;
            }

            if (!mAudioSink->ready())
            {
                ALOGD("render - create output track\n");

                if (createOutputTrack() != NO_ERROR)
                {
                    break;
                }
            }
		}

		{
			Mutex::Autolock l(mMutex);
        	// codec returns negative number on error
        	if (numread < 0)
        	{
          	  ALOGE("Error in FMPlayer  numread=%ld", numread);
          	  sendEvent(MEDIA_ERROR);
          	  break;
       		}

        	// start audio output if necessary
        	if (!audioStarted && !mPaused && !mExit)
        	{
          	    ALOGD("render - starting audio\n");
           	    mAudioSink->start();
			    if(!mAudioRecord.get())
				{
					ALOGD("stop mAudioRecord Before mAudioRecord Start");
					continue;
				}
				else
				{
					mAudioRecord->start();
				}
            	// setparameter to hardware after staring, for cr ALPS00073272
            	//AudioSystem::setParameters (0,DIGITAL_FM_ENABLE);
            	audioStarted = true;
            	firstOutput = true;

				
           	    //firstly push some amount of buffer to make the mixer alive
            	if ((temp = mAudioSink->write(mDummyBuffer, bufSize)) < 0)
            	{
               		 ALOGE("Error in writing:%d", temp);
               		 result = temp;
                	break;
            	}

            	if ((temp = mAudioSink->write(mDummyBuffer, bufSize)) < 0)
            	{
                	ALOGE("Error in writing:%d", temp);
                	result = temp;
                	break;
            	}

            	if ((temp = mAudioSink->write(mDummyBuffer, bufSize)) < 0)
            	{
                	ALOGE("Error in writing:%d", temp);
                	result = temp;
                	break;
            	}
            }
		}

        {
            Mutex::Autolock l(mMutex);
            int brt = 0, art = 0;

            //ALOGD("[%lld] before read %d",brt=getTimeMs());
            if (firstOutput)
            {
                firstOutput = false;
            }
			
			if(!mAudioRecord.get())
			{
				ALOGE("mAudioRecord is deleted by FMAudioPlayer stop !!!");
				continue;
			}
			else
			{
				numread = mAudioRecord->read(mAudioBuffer, bufSize);
			}

            //ALOGD("[%lld] after read %d",art=getTimeMs());
            if (art - brt > 90)
            {
                ALOGW("read time abnormal");
            }

            frameCount++;
        }

        lastTime = thisTime;
        thisTime = getTimeMs();

        if (thisTime - lastTime > 160)
        {
            ALOGW(" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!time diff = %d", thisTime - lastTime);
        }

        // Write data to the audio hardware
        dataCount += numread;
        if ( mPaused || !mSetRender ) //When try to pause, skip write 
            continue;
        if(numread < 0)
        {
            flagRecordError = true;
            ALOGE("Error in Record:%d", numread);
            continue;
        }
        else if ((temp = mAudioSink->write(mAudioBuffer, numread)) < 0)
        {
        	Mutex::Autolock l(mMutex);
			
            ALOGE("Error in writing:%d", temp);
            result = temp;
			if(mAudioRecord.get())
            	mAudioRecord->stop();
            break;
        }

        //ALOGD("[%lld] after write writecount = %d" ,getTimeMs(),temp);
        //sleep to allow command to get mutex
        usleep(1000);
    }

threadExit:
    if (mAudioBuffer)
    {
        delete [] mAudioBuffer;
        mAudioBuffer = NULL;
    }

    if (mDummyBuffer)
    {
        delete [] mDummyBuffer;
        mDummyBuffer = NULL;
    }

	Mutex::Autolock l(mMutex);
	deleteAudioRecord();
	if(mAudioSink != NULL)
		mAudioSink.clear();
	
    ALOGD("render end mRenderTid=%d\n",mRenderTid);

    // tell main thread goodbye  
    mRenderTid = -1;
    mCondition.signal();

#ifdef FM_AUDIO_FILELOG
    fclose(fp);
#endif
    return result;
}


bool FMAudioPlayer::deleteAudioRecord()
{
	ALOGD("deleteAudioRecord !!!");

	if(mAudioRecord.get())
	{
		mAudioRecord.clear();
	}
	return true;
}

} // end namespace android


