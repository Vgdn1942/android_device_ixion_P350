/* vim: set et sw=4 ts=4: */

#ifndef __MTK_SCENEKNOWLEDGEPROVIDER_H_
#define __MTK_SCENEKNOWLEDGEPROVIDER_H_

__BEGIN_DECLS

/* Provider enumeration */
enum {
    SKP_HWC,
    SKP_DOMAIN_AUDIOTRACK,
    SKP_DOMAIN_WAKELOCK,
    SKP_DOMAIN_GPU_UTILIZATION,
    /*
       SKP_SURFACEVIEW,
       SKP_3D_FENCE,
       SKP_TOUCH,
       SKP_GSENSOR,
    */

    NUM_SKP_PROVIRDER
};

/* To compatible with the def of PerfServiceNative.h */
enum SceneCategory {
    SCENE_OTHERS,
    SCENE_GAME
};

enum SKP_Hints {
    SKP_HINT_HWC_OTHERS_SCENE,
    SKP_HINT_HWC_GAME_SCENE,
    SKP_HINT_AUDIOTRACK_DESTRUCT,
    SKP_HINT_AUDIOTRACK_CONSTRUCT,
    SKP_HINT_AUDIOTRACK_START,
    SKP_HINT_AUDIOTRACK_STOP,
    SKP_HINT_WAKELOCK_RELEASE,
    SKP_HINT_WAKELOCK_ACQUIRE,
    SKP_HINT_GPU_LOW_UTILIZATION,
    SKP_HINT_GPU_HIGH_UTILIZATION,

    SKP_HINT_UNINITIALIZED,
    NUM_SKP_HINT
};

extern void SceneKnowledgeProvider_notifyChange(int provider, int hint, int pid=-1);

__END_DECLS

#endif
