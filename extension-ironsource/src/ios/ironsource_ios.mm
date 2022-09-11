#if defined(DM_PLATFORM_IOS)
#include "../ironsource_private.h"
#include "../ironsource_callback_private.h"

#include <IronSource/IronSource.h>
#include <UIKit/UIKit.h>

@interface DMIronSourceInitializationDelegate : NSObject<ISInitializationDelegate>
@end

@interface DMIronSourceInterstitialDelegate : NSObject<LevelPlayInterstitialDelegate>
@end

@interface DMIronSourceRewardedDelegate : NSObject<LevelPlayRewardedVideoManualDelegate>
@end

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

    void Initialize(const char* appKey) {
        
        DMIronSourceInitializationDelegate* initializationDelegate = [[DMIronSourceInitializationDelegate alloc] init];
        DMIronSourceInterstitialDelegate* interstitialDelegate = [[DMIronSourceInterstitialDelegate alloc] init];
        DMIronSourceRewardedDelegate* rewardedDelegate = [[DMIronSourceRewardedDelegate alloc] init];

        [IronSource setLevelPlayInterstitialDelegate:interstitialDelegate];
        [IronSource setLevelPlayRewardedVideoManualDelegate:rewardedDelegate];
        [IronSource initWithAppKey:[NSString stringWithUTF8String:appKey] delegate:initializationDelegate];
    }

//--------------------------------------------------
// Interstitial ADS

    void LoadInterstitial() {
        [IronSource loadInterstitial];
    }

    bool IsInterstitialLoaded() {
        BOOL status = [IronSource hasInterstitial];
        return status == YES;
    }

    void ShowInterstitial(const char* placement) {
        if (IsInterstitialLoaded()) {
            [IronSource showInterstitialWithViewController:uiViewController];
        } else {
            SendSimpleMessage(MSG_INTERSTITIAL, EVENT_NOT_LOADED, @"error", @"Can't show interstitial AD that wasn't loaded.");
        }
    }

//--------------------------------------------------
// Rewarded ADS

    void LoadRewarded() {
        [IronSource loadRewardedVideo];
    }

    bool IsRewardedLoaded() {
        BOOL status = [IronSource hasRewardedVideo];
        return status == YES;
    }

    void ShowRewarded(const char* placement) {
        if (IsRewardedLoaded()) {
            [IronSource showRewardedVideoWithViewController:uiViewController];
        } else {
            SendSimpleMessage(MSG_REWARDED, EVENT_NOT_LOADED, @"error", @"Can't show rewarded AD that wasn't loaded.");
        }
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

    void SetHasUserConsent(bool hasConsent) {
        BOOL value = hasConsent ? YES : NO;
        [IronSource setConsent:value];
    }

    void SetIsAgeRestrictedUser(bool ageRestricted) {
        NSString* str = ageRestricted ? @"YES" : @"NO";
        [IronSource setMetaDataWithKey:@"is_child_directed" value:str];

    }

    void SetDoNotSell(bool doNotSell) {
        NSString* str = doNotSell ? @"YES" : @"NO";
        [IronSource setMetaDataWithKey:@"do_not_sell" value:str];
    }

    void ValidateIntegration() {
        [ISIntegrationHelper validateIntegration];
    }

} //namespace

@implementation DMIronSourceInitializationDelegate

-(void)initializationDidComplete {
    dmIronSource::SendSimpleMessage(dmIronSource::MSG_INITIALIZATION, dmIronSource::EVENT_COMPLETE);
}

@end

@implementation DMIronSourceInterstitialDelegate
/**
 Called after an interstitial has been loaded
 @param adInfo The info of the ad.
 */
- (void)didLoadWithAdInfo:(ISAdInfo *)adInfo {
    dmIronSource::SendSimpleMessage(dmIronSource::MSG_INTERSTITIAL, dmIronSource::EVENT_LOADED);
}

/**
 Called after an interstitial has attempted to load but failed.
 @param error The reason for the error
 */
- (void)didFailToLoadWithError:(NSError *)error {
    dmIronSource::SendSimpleMessage(dmIronSource::MSG_INTERSTITIAL, dmIronSource::EVENT_FAILED_TO_LOAD);
}

/**
 Called after an interstitial has been opened. 
 This is the indication for impression. 
 @param adInfo The info of the ad.
 */
- (void)didOpenWithAdInfo:(ISAdInfo *)adInfo {
    dmIronSource::SendSimpleMessage(dmIronSource::MSG_INTERSTITIAL, dmIronSource::EVENT_OPENING);
}

/**
 Called after an interstitial has been dismissed.
 @param adInfo The info of the ad.
 */
- (void)didCloseWithAdInfo:(ISAdInfo *)adInfo {
    dmIronSource::SendSimpleMessage(dmIronSource::MSG_INTERSTITIAL, dmIronSource::EVENT_CLOSED);
}

/**
 Called after an interstitial has attempted to show but failed.
 @param error The reason for the error
 @param adInfo The info of the ad.
 */
- (void)didFailToShowWithError:(NSError *)error andAdInfo:(ISAdInfo *)adInfo {
    dmIronSource::SendSimpleMessage(dmIronSource::MSG_INTERSTITIAL, dmIronSource::EVENT_FAILED_TO_SHOW);
}

/**
 Called after an interstitial has been clicked.
 @param adInfo The info of the ad.
 */
- (void)didClickWithAdInfo:(ISAdInfo *)adInfo {
    dmIronSource::SendSimpleMessage(dmIronSource::MSG_INTERSTITIAL, dmIronSource::EVENT_CLICKED);
}

/**
 Called after an interstitial has been displayed on the screen.
 This callback is not supported by all networks, and we recommend using it 
 only if it's supported by all networks you included in your build. 
 @param adInfo The info of the ad.
 */
- (void)didShowWithAdInfo:(ISAdInfo *)adInfo {
}

@end

@implementation DMIronSourceRewardedDelegate
/**
 Called after an rewarded video has been loaded in manual mode
 @param adInfo The info of the ad.
 */
- (void)didLoadWithAdInfo:(ISAdInfo *)adInfo {
    dmIronSource::SendSimpleMessage(dmIronSource::MSG_REWARDED, dmIronSource::EVENT_LOADED);
}

/**
 Called after a rewarded video has attempted to load but failed in manual mode
 @param error The reason for the error
 */
- (void)didFailToLoadWithError:(NSError *)error {
    dmIronSource::SendSimpleMessage(dmIronSource::MSG_REWARDED, dmIronSource::EVENT_FAILED_TO_LOAD);
}

/**
 Called after a rewarded video has been viewed completely and the user is eligible for a reward.
 @param placementInfo An object that contains the placement's reward name and amount.
 @param adInfo The info of the ad.
 */
- (void)didReceiveRewardForPlacement:(ISPlacementInfo *)placementInfo withAdInfo:(ISAdInfo *)adInfo {
    dmIronSource::SendSimpleMessage(dmIronSource::MSG_REWARDED, dmIronSource::EVENT_EARNED_REWARD);
}

/**
 Called after a rewarded video has attempted to show but failed.
 @param error The reason for the error
 @param adInfo The info of the ad.
 */
- (void)didFailToShowWithError:(NSError *)error andAdInfo:(ISAdInfo *)adInfo {
    dmIronSource::SendSimpleMessage(dmIronSource::MSG_REWARDED, dmIronSource::EVENT_FAILED_TO_SHOW);
}

/**
 Called after a rewarded video has been opened.
 @param adInfo The info of the ad.
 */
- (void)didOpenWithAdInfo:(ISAdInfo *)adInfo {
    dmIronSource::SendSimpleMessage(dmIronSource::MSG_REWARDED, dmIronSource::EVENT_OPENING);
}

/**
 Called after a rewarded video has been dismissed.
 @param adInfo The info of the ad.
 */
- (void)didCloseWithAdInfo:(ISAdInfo *)adInfo {
    dmIronSource::SendSimpleMessage(dmIronSource::MSG_REWARDED, dmIronSource::EVENT_CLOSED);
}

/**
 Called after a rewarded video has been clicked. 
 This callback is not supported by all networks, and we recommend using it 
 only if it's supported by all networks you included in your build
 @param adInfo The info of the ad.
 */
- (void)didClick:(ISPlacementInfo *)placementInfo withAdInfo:(ISAdInfo *)adInfo {
    dmIronSource::SendSimpleMessage(dmIronSource::MSG_REWARDED, dmIronSource::EVENT_CLICKED);
}

@end

#endif