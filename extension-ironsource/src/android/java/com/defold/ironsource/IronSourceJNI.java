package com.defold.ironsource;

import android.app.Activity;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.WindowManager;
import android.widget.RelativeLayout;

import com.ironsource.mediationsdk.ISBannerSize;
import com.ironsource.mediationsdk.IronSource;
import com.ironsource.mediationsdk.IronSourceBannerLayout;
import com.ironsource.mediationsdk.adunit.adapter.utility.AdInfo;
import com.ironsource.mediationsdk.integration.IntegrationHelper;
import com.ironsource.mediationsdk.logger.IronSourceError;
import com.ironsource.mediationsdk.model.Placement;
import com.ironsource.mediationsdk.sdk.InitializationListener;
import com.ironsource.mediationsdk.sdk.LevelPlayBannerListener;
import com.ironsource.mediationsdk.sdk.LevelPlayInterstitialListener;
import com.ironsource.mediationsdk.sdk.LevelPlayRewardedVideoManualListener;

import org.json.JSONException;
import org.json.JSONObject;

public class IronSourceJNI {

    private static final String TAG = "IronSourceJNI";

    public static native void addToQueue(int msg, String json);

    // CONSTANTS:
    // duplicate of enums from ironsource_callback_private.h:
    private static final int MSG_INTERSTITIAL =         1;
    private static final int MSG_REWARDED =             2;
    private static final int MSG_BANNER =               3;
    private static final int MSG_INITIALIZATION =       4;

    private static final int EVENT_CLOSED =             1;
    private static final int EVENT_FAILED_TO_SHOW =     2;
    private static final int EVENT_OPENING =            3;
    private static final int EVENT_FAILED_TO_LOAD =     4;
    private static final int EVENT_LOADED =             5;
    private static final int EVENT_NOT_LOADED =         6;
    private static final int EVENT_EARNED_REWARD =      7;
    private static final int EVENT_COMPLETE =           8;
    private static final int EVENT_CLICKED =            9;
    private static final int EVENT_DESTROYED =          10;
    private static final int EVENT_EXPANDED =           11;
    private static final int EVENT_COLLAPSED =          12;

    // duplicate of enums from ironsource_private.h:
    private static final int SIZE_BANNER =              0;
    private static final int SIZE_LARGE =               1;
    private static final int SIZE_RECTANGLE =           2;
    private static final int SIZE_SMART =               3;

    private static final int POS_NONE =                 0;
    private static final int POS_TOP_LEFT =             1;
    private static final int POS_TOP_CENTER =           2;
    private static final int POS_TOP_RIGHT =            3;
    private static final int POS_BOTTOM_LEFT =          4;
    private static final int POS_BOTTOM_CENTER =        5;
    private static final int POS_BOTTOM_RIGHT =         6;
    private static final int POS_CENTER =               7;
    // END CONSTANTS

    private Activity mActivity;

    public IronSourceJNI(final Activity activity) {
        mActivity = activity;
        IronSource.setLevelPlayInterstitialListener(new InterstitialListener());
        IronSource.setLevelPlayRewardedVideoManualListener(new RewardedVideoListener());
    }

    public void initialize(String appKey) {
        IronSource.init(mActivity, appKey, new InitializationListener() {
            @Override
            public void onInitializationComplete() {
                sendSimpleMessage(MSG_INITIALIZATION, EVENT_COMPLETE);
            }
        });
    }

    public void onActivateApp()
    {
        IronSource.onResume(mActivity);
    }

    public void onDeactivateApp()
    {
        IronSource.onPause(mActivity);
    }

    public void validateIntegration() {
        IntegrationHelper.validateIntegration(mActivity);
    }

    // https://www.baeldung.com/java-json-escaping
    private String getJsonConversionErrorMessage(String messageText) {
        String message = null;
        try {
            JSONObject obj = new JSONObject();
            obj.put("error", messageText);
            message = obj.toString();
        } catch (JSONException e) {
            message = "{ \"error\": \"Error while converting simple message to JSON.\" }";
        }
        return message;
    }

    private void sendSimpleMessage(int msg, int eventId) {
        String message = null;
        try {
            JSONObject obj = new JSONObject();
            obj.put("event", eventId);
            message = obj.toString();
        } catch (JSONException e) {
            message = getJsonConversionErrorMessage(e.getMessage());
        }
        addToQueue(msg, message);
    }

    private void sendSimpleMessage(int msg, int eventId, String key_2, String value_2) {
        String message = null;
        try {
            JSONObject obj = new JSONObject();
            obj.put("event", eventId);
            obj.put(key_2, value_2);
            message = obj.toString();
        } catch (JSONException e) {
            message = getJsonConversionErrorMessage(e.getMessage());
        }
        addToQueue(msg, message);
    }

    private void sendSimpleMessage(int msg, int eventId, String key_2, int value_2, String key_3, String value_3) {
        String message = null;
        try {
            JSONObject obj = new JSONObject();
            obj.put("event", eventId);
            obj.put(key_2, value_2);
            obj.put(key_3, value_3);
            message = obj.toString();
        } catch (JSONException e) {
            message = getJsonConversionErrorMessage(e.getMessage());
        }
        addToQueue(msg, message);
    }

//--------------------------------------------------
// Interstitial ADS

    public void loadInterstitial() {
        IronSource.loadInterstitial();
    }

    public void showInterstitial(final String placement) {
        if (!isInterstitialLoaded()) {
            sendSimpleMessage(MSG_INTERSTITIAL, EVENT_NOT_LOADED,
                    "error", "Can't show Interstitial AD that wasn't loaded.");
            return;
        }

        if (placement != null) {
            IronSource.showInterstitial(placement);
        } else {
            IronSource.showInterstitial();
        }
    }

    public boolean isInterstitialLoaded() {
        return IronSource.isInterstitialReady();
    }

    private class InterstitialListener implements LevelPlayInterstitialListener {

        // Invoked when the interstitial ad was loaded successfully.
        // AdInfo parameter includes information about the loaded ad
        @Override
        public void onAdReady (AdInfo adInfo) {
            sendSimpleMessage(MSG_INTERSTITIAL, EVENT_LOADED, "network", adInfo.getAdNetwork());
        }

        // Indicates that the ad failed to be loaded
        @Override
        public void onAdLoadFailed (IronSourceError error) {
            int code = error.getErrorCode();
            String msg = error.getErrorMessage();
            sendSimpleMessage(MSG_INTERSTITIAL, EVENT_FAILED_TO_LOAD, "code", code, "error", msg);
        }

        // Invoked when the Interstitial Ad Unit has opened, and user left the application screen.
        // This is the impression indication.
        @Override
        public void onAdOpened (AdInfo adInfo) {
            sendSimpleMessage(MSG_INTERSTITIAL, EVENT_OPENING, "network", adInfo.getAdNetwork());
        }

        // Invoked when the interstitial ad closed and the user went back to the application screen.
        @Override
        public void onAdClosed (AdInfo adInfo) {
            sendSimpleMessage(MSG_INTERSTITIAL, EVENT_CLOSED, "network", adInfo.getAdNetwork());
        }

        // Invoked when the ad failed to show
        @Override
        public void onAdShowFailed (IronSourceError error, AdInfo adInfo) {
            int code = error.getErrorCode();
            String msg = error.getErrorMessage();
            sendSimpleMessage(MSG_INTERSTITIAL, EVENT_FAILED_TO_SHOW, "code", code, "error", msg);
        }

        // Invoked when end user clicked on the interstitial ad
        @Override
        public void onAdClicked (AdInfo adInfo) {
            sendSimpleMessage(MSG_INTERSTITIAL, EVENT_CLICKED, "network", adInfo.getAdNetwork());
        }

        // Invoked before the interstitial ad was opened, and before the InterstitialOnAdOpenedEvent is reported.
        // This callback is not supported by all networks, and we recommend using it only if
        // it's supported by all networks you included in your build.
        @Override
        public void onAdShowSucceeded (AdInfo adInfo) {
        }
    }

//--------------------------------------------------
// Rewarded ADS

    public void loadRewarded() {
        IronSource.loadRewardedVideo();
    }

    public void showRewarded(final String placement) {
        if (!isRewardedLoaded()) {
            sendSimpleMessage(MSG_REWARDED, EVENT_NOT_LOADED,
                    "error", "Can't show Rewarded AD that wasn't loaded.");
            return;
        }

        if (placement != null) {
            IronSource.showRewardedVideo(placement);
        } else {
            IronSource.showRewardedVideo();
        }
    }

    public boolean isRewardedLoaded() {
        return IronSource.isRewardedVideoAvailable();
    }

    private class RewardedVideoListener implements LevelPlayRewardedVideoManualListener {

        // Indicates that the Rewarded video ad was loaded successfully.
        // AdInfo parameter includes information about the loaded ad
        @Override
        public void onAdReady(AdInfo adInfo) {
            sendSimpleMessage(MSG_REWARDED, EVENT_LOADED, "network", adInfo.getAdNetwork());
        }

        // Invoked when the rewarded video failed to load
        @Override
        public void onAdLoadFailed(IronSourceError error) {
            int code = error.getErrorCode();
            String msg = error.getErrorMessage();
            sendSimpleMessage(MSG_REWARDED, EVENT_FAILED_TO_LOAD, "code", code, "error", msg);
        }

        // The Rewarded Video ad view has opened. Your activity will loose focus
        @Override
        public void onAdOpened(AdInfo adInfo) {
            sendSimpleMessage(MSG_REWARDED, EVENT_OPENING, "network", adInfo.getAdNetwork());
        }

        // The Rewarded Video ad view is about to be closed. Your activity will regain its focus
        @Override
        public void onAdClosed(AdInfo adInfo) {
            sendSimpleMessage(MSG_REWARDED, EVENT_CLOSED, "network", adInfo.getAdNetwork());
        }

        // The user completed to watch the video, and should be rewarded.
        // The placement parameter will include the reward data.
        // When using server-to-server callbacks, you may ignore this event and wait for the ironSource server callback
        @Override
        public void onAdRewarded(Placement placement, AdInfo adInfo) {
            int rewardAmount = placement.getRewardAmount();
            String rewardType = placement.getRewardName();
            sendSimpleMessage(MSG_REWARDED, EVENT_EARNED_REWARD, "amount", rewardAmount, "type", rewardType);
        }

        // The rewarded video ad was failed to show
        @Override
        public void onAdShowFailed(IronSourceError error, AdInfo adInfo) {
            int code = error.getErrorCode();
            String msg = error.getErrorMessage();
            sendSimpleMessage(MSG_REWARDED, EVENT_FAILED_TO_SHOW, "code", code, "error", msg);
        }

        // Invoked when the video ad was clicked.
        // This callback is not supported by all networks, and we recommend using it
        // only if it's supported by all networks you included in your build
        @Override
        public void onAdClicked(Placement placement, AdInfo adInfo) {
            sendSimpleMessage(MSG_REWARDED, EVENT_CLICKED, "network", adInfo.getAdNetwork());
        }
    }

//--------------------------------------------------
// Banner ADS

    private RelativeLayout mBannerContainer = null;
    private IronSourceBannerLayout mBanner = null;
    private boolean mBannerLoaded = false;
    private int mBannerGravity = Gravity.NO_GRAVITY;

    public void loadBanner(final int bannerSize) {
        mActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                destroyBannerUiThread();
                ISBannerSize size = getISBannerSize(bannerSize);
                final IronSourceBannerLayout createdBanner = IronSource.createBanner(mActivity, size);

                mBanner = createdBanner;
                mBanner.setLevelPlayBannerListener(new LevelPlayBannerListener() {
                    // Invoked each time a banner was loaded. Either on refresh, or manual load.
                    // AdInfo parameter includes information about the loaded ad
                    @Override
                    public void onAdLoaded(final AdInfo adInfo) {
                        mActivity.runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                if (createdBanner != mBanner) {
                                    Log.w(TAG, "Prevent reporting onAdLoaded for obsolete BannerAd (loadBanner was called multiple times)");
                                    IronSource.destroyBanner(createdBanner);
                                    return;
                                }

                                mBannerLoaded = true;
                                recreateBannerLayout(createdBanner);
                                sendSimpleMessage(MSG_BANNER, EVENT_LOADED, "network", adInfo.getAdNetwork());
                            }
                        });
                    }

                    // Invoked when the banner loading process has failed.
                    // This callback will be sent both for manual load and refreshed banner failures.
                    @Override
                    public void onAdLoadFailed(IronSourceError error) {
                        int code = error.getErrorCode();
                        String msg = error.getErrorMessage();
                        sendSimpleMessage(MSG_BANNER, EVENT_FAILED_TO_LOAD, "code", code, "error", msg);
                    }

                    // Invoked when end user clicks on the banner ad
                    @Override
                    public void onAdClicked(AdInfo adInfo) {
                        sendSimpleMessage(MSG_BANNER, EVENT_CLICKED, "network", adInfo.getAdNetwork());
                    }

                    // Notifies the presentation of a full screen content following user click
                    @Override
                    public void onAdScreenPresented(AdInfo adInfo) {
                        sendSimpleMessage(MSG_BANNER, EVENT_EXPANDED, "network", adInfo.getAdNetwork());
                    }

                    // Notifies the presented screen has been dismissed
                    @Override
                    public void onAdScreenDismissed(AdInfo adInfo) {
                        sendSimpleMessage(MSG_BANNER, EVENT_COLLAPSED, "network", adInfo.getAdNetwork());
                    }

                    //Invoked when the user left the app
                    @Override
                    public void onAdLeftApplication(AdInfo adInfo){}
                });

                IronSource.loadBanner(createdBanner);
            }
        });
    }

    public void destroyBanner() {
        mActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                destroyBannerUiThread();
            }
        });
    }

    public void showBanner(final int pos, final String placement) {
        mActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (isBannerLoaded()) {
                    mBannerGravity = getGravity(pos);
                    mBanner.setPlacementName(placement);
                    recreateBannerLayout(mBanner);
                }
                else
                {
                    sendSimpleMessage(MSG_BANNER, EVENT_NOT_LOADED,
                            "error", "Can't show Banner AD that wasn't loaded.");
                }
            }
        });
    }

    public void hideBanner() {
        mActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (mBannerContainer != null) {
                    mBannerContainer.setVisibility(View.INVISIBLE);
                }
            }
        });
    }

    public boolean isBannerLoaded() {
        return mBannerLoaded;
    }

    public boolean isBannerShown() {
        return isBannerLoaded() && mBannerContainer != null && mBannerContainer.getVisibility() == View.VISIBLE;
    }

    private void destroyBannerUiThread() {
        if (mBanner != null && !mBanner.isDestroyed()) {
            IronSource.destroyBanner(mBanner);
            sendSimpleMessage(MSG_BANNER, EVENT_DESTROYED);
        }

        removeBannerLayout();
        mBanner = null;
        mBannerLoaded = false;
    }

    private int getGravity(final int bannerPosConst) {
        int bannerPos = Gravity.NO_GRAVITY; //POS_NONE
        switch (bannerPosConst) {
            case POS_TOP_LEFT:
                bannerPos = Gravity.TOP | Gravity.LEFT;
                break;
            case POS_TOP_CENTER:
                bannerPos = Gravity.TOP | Gravity.CENTER_HORIZONTAL;
                break;
            case POS_TOP_RIGHT:
                bannerPos = Gravity.TOP | Gravity.RIGHT;
                break;
            case POS_BOTTOM_LEFT:
                bannerPos = Gravity.BOTTOM | Gravity.LEFT;
                break;
            case POS_BOTTOM_CENTER:
                bannerPos = Gravity.BOTTOM | Gravity.CENTER_HORIZONTAL;
                break;
            case POS_BOTTOM_RIGHT:
                bannerPos = Gravity.BOTTOM | Gravity.RIGHT;
                break;
            case POS_CENTER:
                bannerPos = Gravity.CENTER;
                break;
        }
        return bannerPos;
    }

    private ISBannerSize getISBannerSize(final int bannerSizeConst) {
        switch (bannerSizeConst) {
            case SIZE_RECTANGLE:
                return ISBannerSize.RECTANGLE;
            case SIZE_LARGE:
                return ISBannerSize.LARGE;
            case SIZE_SMART:
                return ISBannerSize.SMART;
            case SIZE_BANNER:
            default:
                return ISBannerSize.BANNER;
        }
    }

    private void removeBannerLayout() {
        if (mBannerContainer != null) {
            mBannerContainer.removeAllViews();
            mActivity.getWindowManager().removeView(mBannerContainer);
            mBannerContainer = null;
        }
    }

    private void recreateBannerLayout(IronSourceBannerLayout banner) {
        removeBannerLayout();
        mBannerContainer = new RelativeLayout(mActivity);
        mBannerContainer.setVisibility(View.VISIBLE);
        mBannerContainer.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
        mBannerContainer.addView(banner, getAdLayoutParams(banner));
        mActivity.getWindowManager().addView(mBannerContainer, getWindowLayoutParams());
    }

    private WindowManager.LayoutParams getWindowLayoutParams() {
        WindowManager.LayoutParams windowParams = new WindowManager.LayoutParams();
        windowParams.x = WindowManager.LayoutParams.WRAP_CONTENT;
        windowParams.y = WindowManager.LayoutParams.WRAP_CONTENT;
        windowParams.width = WindowManager.LayoutParams.WRAP_CONTENT;
        windowParams.height = WindowManager.LayoutParams.WRAP_CONTENT;
        windowParams.flags = WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL | WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;
        windowParams.gravity = mBannerGravity;
        return windowParams;
    }

    private RelativeLayout.LayoutParams getAdLayoutParams(IronSourceBannerLayout banner) {
        RelativeLayout.LayoutParams adParams = new RelativeLayout.LayoutParams(
            RelativeLayout.LayoutParams.MATCH_PARENT,
            RelativeLayout.LayoutParams.WRAP_CONTENT);
        adParams.setMargins(0, 0, 0, 0);

        return adParams;
    }
}
