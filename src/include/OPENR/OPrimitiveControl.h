#ifndef _OPrimitiveControl_h_DEFINED
#define _OPrimitiveControl_h_DEFINED

#include <Types.h>

typedef longword OPrimitiveRequest;

//
// OPrimitiveRequest
//
const OPrimitiveRequest oprmreqCOMMON_BASE         =   0;
const OPrimitiveRequest oprmreqSPEAKER_BASE        = 128;
const OPrimitiveRequest oprmreqMIC_BASE            = 256;
const OPrimitiveRequest oprmreqCAM_BASE            = 384;
const OPrimitiveRequest oprmreqBT_BASE             = 512;
const OPrimitiveRequest oprmreqJOINT2_BASE         = 640;
const OPrimitiveRequest oprmreqLED2_BASE           = 768;
const OPrimitiveRequest oprmreqSENSOR_BASE         = 896;
const OPrimitiveRequest oprmreqLED3_BASE           = 1024;
const OPrimitiveRequest oprmreqJOINT4_BASE         = 1152;
const OPrimitiveRequest oprmreqPRIVATE_BASE        = 1048576;

//
// Camera
//
const OPrimitiveRequest oprmreqCAM_SET_WHITE_BALANCE = oprmreqCAM_BASE + 0;
const OPrimitiveRequest oprmreqCAM_SET_GAIN          = oprmreqCAM_BASE + 1;
const OPrimitiveRequest oprmreqCAM_SET_SHUTTER_SPEED = oprmreqCAM_BASE + 2;
const OPrimitiveRequest oprmreqCAM_GET_CDT_TABLE     = oprmreqCAM_BASE + 3;
const OPrimitiveRequest oprmreqCAM_AE_ON             = oprmreqCAM_BASE + 4;
const OPrimitiveRequest oprmreqCAM_AE_OFF            = oprmreqCAM_BASE + 5;
const OPrimitiveRequest oprmreqCAM_AWB_ON            = oprmreqCAM_BASE + 6;
const OPrimitiveRequest oprmreqCAM_AWB_OFF           = oprmreqCAM_BASE + 7;

const longword ocamparamUNDEF           = 0;
const longword ocamparamWB_INDOOR_MODE  = 1;
const longword ocamparamWB_OUTDOOR_MODE = 2;
const longword ocamparamWB_FL_MODE      = 3; // Fluorescent Lamp

const longword ocamparamGAIN_LOW        = 1;
const longword ocamparamGAIN_MID        = 2;
const longword ocamparamGAIN_HIGH       = 3;

const longword ocamparamSHUTTER_SLOW    = 1;
const longword ocamparamSHUTTER_MID     = 2;
const longword ocamparamSHUTTER_FAST    = 3;

struct OPrimitiveControl_CameraParam {
    longword camparam;

    OPrimitiveControl_CameraParam() : camparam(ocamparamUNDEF) {}
    OPrimitiveControl_CameraParam(longword param) : camparam(param) {}
};



#endif /* _OPrimitiveControl_h_DEFINED */
