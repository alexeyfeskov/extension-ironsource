#if defined(DM_PLATFORM_IOS)
#include "../ironsource_private.h"
#include "../ironsource_callback_private.h"
#include <UIKit/UIKit.h>

namespace dmIronSource {

    static UIViewController *uiViewController = nil;

    void SendSimpleMessage(MessageId msg, id obj) {
        NSError* error;
        NSData* jsonData = [NSJSONSerialization dataWithJSONObject:obj options:(NSJSONWritingOptions)0 error:&error];
        if (jsonData)
        {
            NSString* nsstring = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
            AddToQueueCallback(msg, (const char*)[nsstring UTF8String]);
            [nsstring release];
        }
        else
        {
            NSMutableDictionary *dict = [NSMutableDictionary dictionary];
            [dict setObject:error.localizedDescription forKey:@"error"];
            [dict setObject:[NSNumber numberWithInt:EVENT_JSON_ERROR] forKey:@"event"];
            NSError* error2;
            NSData* errorJsonData = [NSJSONSerialization dataWithJSONObject:dict options:(NSJSONWritingOptions)0 error:&error2];
            if (errorJsonData)
            {
                NSString* nsstringError = [[NSString alloc] initWithData:errorJsonData encoding:NSUTF8StringEncoding];
                AddToQueueCallback(msg, (const char*)[nsstringError UTF8String]);
                [nsstringError release];
            }
            else
            {
                AddToQueueCallback(msg, [[NSString stringWithFormat:@"{ \"error\": \"Error while converting simple message to JSON.\", \"event\": %d }", EVENT_JSON_ERROR] UTF8String]);
            }
        }
    }

    void SendSimpleMessage(MessageId msg, MessageEvent event) {
        NSMutableDictionary *dict = [NSMutableDictionary dictionary];
        [dict setObject:[NSNumber numberWithInt:event] forKey:@"event"];
        SendSimpleMessage(msg, dict);
    }

    void SendSimpleMessage(MessageId msg, MessageEvent event, NSString *key_2, NSString *value_2) {
        NSMutableDictionary *dict = [NSMutableDictionary dictionary];
        [dict setObject:[NSNumber numberWithInt:event] forKey:@"event"];
        [dict setObject:value_2 forKey:key_2];
        SendSimpleMessage(msg, dict);
    }

    void SendSimpleMessage(MessageId msg, MessageEvent event, NSString *key_2, int value_2, NSString *key_3, NSString *value_3) {
        NSMutableDictionary *dict = [NSMutableDictionary dictionary];
        [dict setObject:[NSNumber numberWithInt:event] forKey:@"event"];
        [dict setObject:[NSNumber numberWithInt:value_2] forKey:key_2];
        [dict setObject:value_3 forKey:key_3];
        SendSimpleMessage(msg, dict);
    }

    void SendSimpleMessage(MessageId msg, MessageEvent event, NSString *key_2, int value_2, NSString *key_3, int value_3) {
        NSMutableDictionary *dict = [NSMutableDictionary dictionary];
        [dict setObject:[NSNumber numberWithInt:event] forKey:@"event"];
        [dict setObject:[NSNumber numberWithInt:value_2] forKey:key_2];
        [dict setObject:[NSNumber numberWithInt:value_3] forKey:key_3];
        SendSimpleMessage(msg, dict);
    }

    void Initialize() {
            SendSimpleMessage(MSG_INITIALIZATION, EVENT_COMPLETE);
    }

//--------------------------------------------------
// Interstitial ADS

    void LoadInterstitial() {
    }

    bool IsInterstitialLoaded() {
        return 0;
    }

    void ShowInterstitial(const char* placement) {
    }

//--------------------------------------------------
// Rewarded ADS


    void LoadRewarded() {
    }

    bool IsRewardedLoaded() {
        return 0;
    }

    void ShowRewarded(const char* placement) {
    }


//--------------------------------------------------
// Banner ADS

    bool IsBannerLoaded() {
        return 0;
    }

    bool IsBannerShown() {
        return 0;
    }

    void LoadBanner(BannerSize bannerSize) {
    }

    void ShowBanner(BannerPosition bannerPos, const char* placement) {
    }

    void HideBanner() {
    }

    void DestroyBanner() {
    }

//--------------------------------------------------

    void Initialize_Ext() {
        UIWindow* window = dmGraphics::GetNativeiOSUIWindow();
        uiViewController = window.rootViewController;
    }

    void OnActivateApp() {
    }

    void OnDeactivateApp() {
    }

    void Initialize(const char* appKey) {
    }

    void SetHasUserConsent(bool hasConsent) {
    }

    void SetIsAgeRestrictedUser(bool ageRestricted) {
    }

    void SetDoNotSell(bool doNotSell) {
    }

    void ValidateIntegration() {
    }

} //namespace

#endif
