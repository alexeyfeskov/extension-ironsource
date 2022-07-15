#pragma once

namespace dmIronSource {

enum BannerSize
{
    SIZE_BANNER =           0,
    SIZE_LARGE =            1,
    SIZE_RECTANGLE =        2,
    SIZE_SMART =            3,
};

enum BannerPosition
{
    POS_NONE =              0,
    POS_TOP_LEFT =          1,
    POS_TOP_CENTER =        2,
    POS_TOP_RIGHT =         3,
    POS_BOTTOM_LEFT =       4,
    POS_BOTTOM_CENTER =     5,
    POS_BOTTOM_RIGHT =      6,
    POS_CENTER =            7
};

void Initialize_Ext();
void OnActivateApp();
void OnDeactivateApp();

void Initialize(const char* appKey);
void ValidateIntegration();

void LoadInterstitial();
void ShowInterstitial(const char* placement);
bool IsInterstitialLoaded();

void LoadRewarded();
void ShowRewarded(const char* placement);
bool IsRewardedLoaded();

void LoadBanner(BannerSize bannerSize);
void DestroyBanner();
void ShowBanner(BannerPosition bannerPos, const char* placement);
void HideBanner();
bool IsBannerLoaded();
bool IsBannerShown();

} //namespace dmIronSource
