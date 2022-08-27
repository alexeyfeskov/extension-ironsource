#if defined(DM_PLATFORM_ANDROID)

#include <dmsdk/dlib/android.h>

#include "../ironsource_private.h"
#include "../ironsource_callback_private.h"
#include "com_defold_ironsource_IronSourceJNI.h"

JNIEXPORT void JNICALL Java_com_defold_ironsource_IronSourceJNI_addToQueue(JNIEnv * env, jclass cls, jint jmsg, jstring jjson)
{
    const char* json = env->GetStringUTFChars(jjson, 0);
    dmIronSource::AddToQueueCallback((dmIronSource::MessageId)jmsg, json);
    env->ReleaseStringUTFChars(jjson, json);
}

namespace dmIronSource {

struct IronSource
{
    jobject        m_IronSourceJNI;

    jmethodID      m_Initialize;
    jmethodID      m_OnActivateApp;
    jmethodID      m_OnDeactivateApp;
    jmethodID      m_SetHasUserConsent;
    jmethodID      m_SetIsAgeRestrictedUser;
    jmethodID      m_SetDoNotSell;
    jmethodID      m_ValidateIntegration;

    jmethodID      m_LoadInterstitial;
    jmethodID      m_ShowInterstitial;
    jmethodID      m_IsInterstitialLoaded;

    jmethodID      m_LoadRewarded;
    jmethodID      m_ShowRewarded;
    jmethodID      m_IsRewardedLoaded;

    jmethodID      m_LoadBanner;
    jmethodID      m_DestroyBanner;
    jmethodID      m_ShowBanner;
    jmethodID      m_HideBanner;
    jmethodID      m_IsBannerLoaded;
    jmethodID      m_IsBannerShown;
};

static IronSource g_ironsource;

static void CallVoidMethod(jobject instance, jmethodID method)
{
    
    dmAndroid::ThreadAttacher threadAttacher;
    JNIEnv* env = threadAttacher.GetEnv();

    env->CallVoidMethod(instance, method);
}

static bool CallBoolMethod(jobject instance, jmethodID method)
{
    dmAndroid::ThreadAttacher threadAttacher;
    JNIEnv* env = threadAttacher.GetEnv();

    jboolean return_value = (jboolean)env->CallBooleanMethod(instance, method);
    return JNI_TRUE == return_value;
}

static void CallVoidMethodBool(jobject instance, jmethodID method, bool cbool)
{
    dmAndroid::ThreadAttacher threadAttacher;
    JNIEnv* env = threadAttacher.GetEnv();

    env->CallVoidMethod(instance, method, cbool);
}

static void CallVoidMethodChar(jobject instance, jmethodID method, const char* cstr)
{
    dmAndroid::ThreadAttacher threadAttacher;
    JNIEnv* env = threadAttacher.GetEnv();

    jstring jstr = NULL;
    if (cstr)
    {
        jstr = env->NewStringUTF(cstr);
    }

    env->CallVoidMethod(instance, method, jstr);

    if (cstr)
    {
        env->DeleteLocalRef(jstr);
    }
}

static void CallVoidMethodCharInt(jobject instance, jmethodID method, const char* cstr, int cint)
{
    dmAndroid::ThreadAttacher threadAttacher;
    JNIEnv* env = threadAttacher.GetEnv();

    jstring jstr = env->NewStringUTF(cstr);
    env->CallVoidMethod(instance, method, jstr, cint);
    env->DeleteLocalRef(jstr);
}

static void CallVoidMethodIntChar(jobject instance, jmethodID method, int cint, const char* cstr)
{
    dmAndroid::ThreadAttacher threadAttacher;
    JNIEnv* env = threadAttacher.GetEnv();

    jstring jstr = NULL;
    if (cstr)
    {
        jstr = env->NewStringUTF(cstr);
    }

    env->CallVoidMethod(instance, method, cint, jstr);

    if (cstr)
    {
        env->DeleteLocalRef(jstr);
    }
}

static void CallVoidMethodInt(jobject instance, jmethodID method, int cint)
{
    dmAndroid::ThreadAttacher threadAttacher;
    JNIEnv* env = threadAttacher.GetEnv();

    env->CallVoidMethod(instance, method, cint);
}

static void InitJNIMethods(JNIEnv* env, jclass cls)
{
    g_ironsource.m_Initialize             = env->GetMethodID(cls, "initialize", "(Ljava/lang/String;)V");
    g_ironsource.m_OnActivateApp          = env->GetMethodID(cls, "onActivateApp", "()V");
    g_ironsource.m_OnDeactivateApp        = env->GetMethodID(cls, "onDeactivateApp", "()V");
    g_ironsource.m_SetHasUserConsent      = env->GetMethodID(cls, "setHasUserConsent", "(Z)V");
    g_ironsource.m_SetIsAgeRestrictedUser = env->GetMethodID(cls, "setIsAgeRestrictedUser", "(Z)V");
    g_ironsource.m_SetDoNotSell           = env->GetMethodID(cls, "setDoNotSell", "(Z)V");
    g_ironsource.m_ValidateIntegration    = env->GetMethodID(cls, "validateIntegration", "()V");

    g_ironsource.m_LoadInterstitial       = env->GetMethodID(cls, "loadInterstitial", "()V");
    g_ironsource.m_ShowInterstitial       = env->GetMethodID(cls, "showInterstitial", "(Ljava/lang/String;)V");
    g_ironsource.m_IsInterstitialLoaded   = env->GetMethodID(cls, "isInterstitialLoaded", "()Z");

    g_ironsource.m_LoadRewarded     = env->GetMethodID(cls, "loadRewarded", "()V");
    g_ironsource.m_ShowRewarded     = env->GetMethodID(cls, "showRewarded", "(Ljava/lang/String;)V");
    g_ironsource.m_IsRewardedLoaded = env->GetMethodID(cls, "isRewardedLoaded", "()Z");

    g_ironsource.m_LoadBanner     = env->GetMethodID(cls, "loadBanner", "(I)V");
    g_ironsource.m_DestroyBanner  = env->GetMethodID(cls, "destroyBanner", "()V");
    g_ironsource.m_ShowBanner     = env->GetMethodID(cls, "showBanner", "(ILjava/lang/String;)V");
    g_ironsource.m_HideBanner     = env->GetMethodID(cls, "hideBanner", "()V");
    g_ironsource.m_IsBannerLoaded = env->GetMethodID(cls, "isBannerLoaded", "()Z");
    g_ironsource.m_IsBannerShown  = env->GetMethodID(cls, "isBannerShown", "()Z");
}

void Initialize_Ext()
{
    dmAndroid::ThreadAttacher threadAttacher;
    JNIEnv* env = threadAttacher.GetEnv();
    jclass cls = dmAndroid::LoadClass(env, "com.defold.ironsource.IronSourceJNI");

    InitJNIMethods(env, cls);

    jmethodID jni_constructor = env->GetMethodID(cls, "<init>", "(Landroid/app/Activity;)V");

    g_ironsource.m_IronSourceJNI = env->NewGlobalRef(env->NewObject(cls, jni_constructor, threadAttacher.GetActivity()->clazz));
}

void OnActivateApp()
{
    CallVoidMethod(g_ironsource.m_IronSourceJNI, g_ironsource.m_OnActivateApp);
}

void OnDeactivateApp()
{
    CallVoidMethod(g_ironsource.m_IronSourceJNI, g_ironsource.m_OnDeactivateApp);
}

void Initialize(const char* appKey)
{
    CallVoidMethodChar(g_ironsource.m_IronSourceJNI, g_ironsource.m_Initialize, appKey);
}

void SetHasUserConsent(bool hasConsent)
{
    CallVoidMethodBool(g_ironsource.m_IronSourceJNI, g_ironsource.m_SetHasUserConsent, hasConsent);
}

void SetIsAgeRestrictedUser(bool ageRestricted)
{
    CallVoidMethodBool(g_ironsource.m_IronSourceJNI, g_ironsource.m_SetIsAgeRestrictedUser, ageRestricted);
}

void SetDoNotSell(bool doNotSell)
{
    CallVoidMethodBool(g_ironsource.m_IronSourceJNI, g_ironsource.m_SetDoNotSell, doNotSell);
}

void ValidateIntegration()
{
    CallVoidMethod(g_ironsource.m_IronSourceJNI, g_ironsource.m_ValidateIntegration);
}

void LoadInterstitial()
{
    CallVoidMethod(g_ironsource.m_IronSourceJNI, g_ironsource.m_LoadInterstitial);
}

void ShowInterstitial(const char* placement)
{
    CallVoidMethodChar(g_ironsource.m_IronSourceJNI, g_ironsource.m_ShowInterstitial, placement);
}

bool IsInterstitialLoaded()
{
    return CallBoolMethod(g_ironsource.m_IronSourceJNI, g_ironsource.m_IsInterstitialLoaded);
}

void LoadRewarded()
{
    CallVoidMethod(g_ironsource.m_IronSourceJNI, g_ironsource.m_LoadRewarded);
}

void ShowRewarded(const char* placement)
{
    CallVoidMethodChar(g_ironsource.m_IronSourceJNI, g_ironsource.m_ShowRewarded, placement);
}

bool IsRewardedLoaded()
{
    return CallBoolMethod(g_ironsource.m_IronSourceJNI, g_ironsource.m_IsRewardedLoaded);
}

void LoadBanner(BannerSize bannerSize)
{
    CallVoidMethodInt(g_ironsource.m_IronSourceJNI, g_ironsource.m_LoadBanner, (int)bannerSize);
}

void DestroyBanner()
{
    CallVoidMethod(g_ironsource.m_IronSourceJNI, g_ironsource.m_DestroyBanner);
}

void ShowBanner(BannerPosition bannerPos, const char* placement)
{
    CallVoidMethodIntChar(g_ironsource.m_IronSourceJNI, g_ironsource.m_ShowBanner, (int)bannerPos, placement);
}

void HideBanner()
{
    CallVoidMethod(g_ironsource.m_IronSourceJNI, g_ironsource.m_HideBanner);
}

bool IsBannerLoaded()
{
    return CallBoolMethod(g_ironsource.m_IronSourceJNI, g_ironsource.m_IsBannerLoaded);
}

bool IsBannerShown()
{
    return CallBoolMethod(g_ironsource.m_IronSourceJNI, g_ironsource.m_IsBannerShown);
}

}//namespace dmIronSource

#endif
