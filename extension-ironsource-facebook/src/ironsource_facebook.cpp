#define EXTENSION_NAME IronSourceFacebookExt
#define LIB_NAME "IronSourceFacebook"
#define MODULE_NAME "ironsource"

#define DLIB_LOG_DOMAIN LIB_NAME
#include <dmsdk/sdk.h>

static dmExtension::Result InitializeIronSource(dmExtension::Params* params)
{
    dmLogInfo("Registered extension IronSource mediation adapter - Facebook");
    return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeIronSource(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, 0, 0, InitializeIronSource, 0, 0, FinalizeIronSource)
