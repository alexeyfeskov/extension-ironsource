#pragma once

#include "ironsource_private.h"
#include <dmsdk/sdk.h>

namespace dmIronSource {

// The same events and messages are in IronSourceJNI.java
// If you change enums here, pls make sure you update them here as well

enum MessageId
{
    MSG_INTERSTITIAL =              1,
    MSG_REWARDED =                  2,
    MSG_BANNER =                    3,
    MSG_INITIALIZATION =            4,
};

enum MessageEvent
{
    EVENT_CLOSED =                  1,
    EVENT_FAILED_TO_SHOW =          2,
    EVENT_OPENING =                 3,
    EVENT_FAILED_TO_LOAD =          4,
    EVENT_LOADED =                  5,
    EVENT_NOT_LOADED =              6,
    EVENT_EARNED_REWARD =           7,
    EVENT_COMPLETE =                8,
    EVENT_CLICKED =                 9,
    EVENT_DESTROYED =               10,
    EVENT_EXPANDED =                11,
    EVENT_COLLAPSED =               12,
    EVENT_JSON_ERROR =              13,
};

struct CallbackData
{
    MessageId msg;
    char* json;
};

void SetLuaCallback(lua_State* L, int pos);
void UpdateCallback();
void InitializeCallback();
void FinalizeCallback();

void AddToQueueCallback(MessageId type, const char*json);

} //namespace dmIronSource
