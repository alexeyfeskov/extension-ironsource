#define EXTENSION_NAME IronSourceExt
#define LIB_NAME "IronSource"
#define MODULE_NAME "ironsource"

#define DLIB_LOG_DOMAIN LIB_NAME
#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_ANDROID) //|| defined(DM_PLATFORM_IOS)

#include "utils/LuaUtils.h"
#include "ironsource_private.h"
#include "ironsource_callback_private.h"

namespace dmIronSource {

static int Lua_Initialize(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    if (lua_type(L, 1) != LUA_TSTRING) {
        char msg[256];
        snprintf(msg, sizeof(msg), "Expected string, got %s. Wrong type for Initialize appKey variable '%s'.", luaL_typename(L, 1), lua_tostring(L, 1));
        luaL_error(L, msg);
        return 0;
    }
    const char* appKey_lua = luaL_checkstring(L, 1);
    Initialize(appKey_lua);
    return 0;
}

static int Lua_SetCallback(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    SetLuaCallback(L, 1);
    return 0;
}

static int Lua_ValidateIntegration(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    ValidateIntegration();
    return 0;
}

static int Lua_LoadInterstitial(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    LoadInterstitial();
    return 0;
}

static int Lua_ShowInterstitial(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    const char* placement_lua = luaL_checkstringd(L, 1, NULL);
    ShowInterstitial(placement_lua);
    return 0;
}

static int Lua_IsInterstitialLoaded(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    bool is_loaded = IsInterstitialLoaded();
    lua_pushboolean(L, is_loaded);
    return 1;
}

static int Lua_LoadRewarded(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    LoadRewarded();
    return 0;
}

static int Lua_ShowRewarded(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    const char* placement_lua = luaL_checkstringd(L, 1, NULL);
    ShowRewarded(placement_lua);
    return 0;
}

static int Lua_IsRewardedLoaded(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    bool is_loaded = IsRewardedLoaded();
    lua_pushboolean(L, is_loaded);
    return 1;
}

static int Lua_LoadBanner(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    BannerSize bannerSize_lua = SIZE_BANNER;
    if (lua_type(L, 2) != LUA_TNONE) {
        bannerSize_lua = (BannerSize)luaL_checknumber(L, 2);
    }
    LoadBanner(bannerSize_lua);
    return 0;
}

static int Lua_DestroyBanner(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    DestroyBanner();
    return 0;
}

static int Lua_ShowBanner(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    BannerPosition bannerPos_lua = POS_NONE;
    if (lua_type(L, 1) != LUA_TNONE) {
        bannerPos_lua = (BannerPosition)luaL_checknumber(L, 1);
    }
    const char* placement_lua = luaL_checkstringd(L, 2, NULL);
    ShowBanner(bannerPos_lua, placement_lua);
    return 0;
}

static int Lua_HideBanner(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    HideBanner();
    return 0;
}

static int Lua_IsBannerLoaded(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    bool is_loaded = IsBannerLoaded();
    lua_pushboolean(L, is_loaded);
    return 1;
}

static int Lua_IsBannerShown(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    bool is_loaded = IsBannerShown();
    lua_pushboolean(L, is_loaded);
    return 1;
}

static const luaL_reg Module_methods[] =
{
    {"initialize", Lua_Initialize},
    {"set_callback", Lua_SetCallback},
    {"validate_integration", Lua_ValidateIntegration},

    {"load_interstitial", Lua_LoadInterstitial},
    {"show_interstitial", Lua_ShowInterstitial},
    {"is_interstitial_loaded", Lua_IsInterstitialLoaded},

    {"load_rewarded", Lua_LoadRewarded},
    {"show_rewarded", Lua_ShowRewarded},
    {"is_rewarded_loaded", Lua_IsRewardedLoaded},

    {"load_banner", Lua_LoadBanner},
    {"destroy_banner", Lua_DestroyBanner},
    {"show_banner", Lua_ShowBanner},
    {"hide_banner", Lua_HideBanner},
    {"is_banner_loaded", Lua_IsBannerLoaded},
    {"is_banner_shown", Lua_IsBannerShown},
    {0, 0}
};

static void LuaInit(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    luaL_register(L, MODULE_NAME, Module_methods);

    #define SETCONSTANT(name) \
    lua_pushnumber(L, (lua_Number) name); \
    lua_setfield(L, -2, #name); \

    SETCONSTANT(MSG_INTERSTITIAL)
    SETCONSTANT(MSG_REWARDED)
    SETCONSTANT(MSG_BANNER)
    SETCONSTANT(MSG_INITIALIZATION)

    SETCONSTANT(EVENT_CLOSED)
    SETCONSTANT(EVENT_FAILED_TO_SHOW)
    SETCONSTANT(EVENT_OPENING)
    SETCONSTANT(EVENT_FAILED_TO_LOAD)
    SETCONSTANT(EVENT_LOADED)
    SETCONSTANT(EVENT_NOT_LOADED)
    SETCONSTANT(EVENT_EARNED_REWARD)
    SETCONSTANT(EVENT_COMPLETE)
    SETCONSTANT(EVENT_CLICKED)
    SETCONSTANT(EVENT_DESTROYED)
    SETCONSTANT(EVENT_EXPANDED)
    SETCONSTANT(EVENT_COLLAPSED)

    SETCONSTANT(SIZE_BANNER)
    SETCONSTANT(SIZE_LARGE)
    SETCONSTANT(SIZE_RECTANGLE)
    SETCONSTANT(SIZE_SMART)

    SETCONSTANT(POS_NONE)
    SETCONSTANT(POS_TOP_LEFT)
    SETCONSTANT(POS_TOP_CENTER)
    SETCONSTANT(POS_TOP_RIGHT)
    SETCONSTANT(POS_BOTTOM_LEFT)
    SETCONSTANT(POS_BOTTOM_CENTER)
    SETCONSTANT(POS_BOTTOM_RIGHT)
    SETCONSTANT(POS_CENTER)

    #undef SETCONSTANT

    lua_pop(L, 1);
}

static dmExtension::Result AppInitializeIronSource(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result InitializeIronSource(dmExtension::Params* params)
{
    LuaInit(params->m_L);
    Initialize_Ext();
    InitializeCallback();
    dmLogInfo("Registered extension IronSource");
    return dmExtension::RESULT_OK;
}

static dmExtension::Result AppFinalizeIronSource(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeIronSource(dmExtension::Params* params)
{
    FinalizeCallback();
    return dmExtension::RESULT_OK;
}

static dmExtension::Result UpdateIronSource(dmExtension::Params* params)
{
    UpdateCallback();
    return dmExtension::RESULT_OK;
}

static void OnEventIronSource(dmExtension::Params* params, const dmExtension::Event* event)
{
    if(event->m_Event == dmExtension::EVENT_ID_ACTIVATEAPP)
    {
        OnActivateApp();
    }
    else if(event->m_Event == dmExtension::EVENT_ID_DEACTIVATEAPP)
    {
        OnDeactivateApp();
    }
}

} //namespace dmIronSource

DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, dmIronSource::AppInitializeIronSource, dmIronSource::AppFinalizeIronSource, dmIronSource::InitializeIronSource, dmIronSource::UpdateIronSource, dmIronSource::OnEventIronSource, dmIronSource::FinalizeIronSource)

#else

static  dmExtension::Result InitializeIronSource(dmExtension::Params* params)
{
    dmLogInfo("Registered extension IronSource (null)");
    return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeIronSource(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, 0, 0, InitializeIronSource, 0, 0, FinalizeIronSource)

#endif // IOS/Android
