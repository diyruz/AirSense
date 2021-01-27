#include "AF.h"
#include "OSAL.h"
#include "ZComDef.h"
#include "ZDConfig.h"

#include "zcl.h"
#include "zcl_general.h"
#include "zcl_ha.h"
#include "zcl_ms.h"

#include "zcl_app.h"

#include "version.h"

#include "bdb_touchlink.h"
#include "bdb_touchlink_target.h"
#include "stub_aps.h"

/*********************************************************************
 * CONSTANTS
 */

#define APP_DEVICE_VERSION 2
#define APP_FLAGS 0

#define APP_HWVERSION 1
#define APP_ZCLVERSION 1

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

// Global attributes
const uint16 zclApp_clusterRevision_all = 0x0002;

// Basic Cluster
const uint8 zclApp_HWRevision = APP_HWVERSION;
const uint8 zclApp_ZCLVersion = APP_ZCLVERSION;
const uint8 zclApp_ApplicationVersion = 3;
const uint8 zclApp_StackVersion = 4;

//{lenght, 'd', 'a', 't', 'a'}
const uint8 zclApp_ManufacturerName[] = {9, 'm', 'o', 'd', 'k', 'a', 'm', '.', 'r', 'u'};
const uint8 zclApp_ModelId[] = {15, 'D', 'I', 'Y', 'R', 'u', 'Z', '_', 'A', 'i', 'r', 'S', 'e', 'n', 's', 'e'};
const uint8 zclApp_PowerSource = POWER_SOURCE_MAINS_1_PHASE;

#define DEFAULT_LedFeedback TRUE
#define DEFAULT_EnableABC TRUE
// FYI: https://www.kane.co.uk/knowledge-centre/what-are-safe-levels-of-co-and-co2-in-rooms
#define DEFAULT_Threshold1_PPM 1000
#define DEFAULT_Threshold2_PPM 2000
#define DEFAULT_TemperatureOffset 0
#define DEFAULT_PressureOffset 0
#define DEFAULT_HumidityOffset 0



application_config_t zclApp_Config = {
    .LedFeedback = DEFAULT_LedFeedback,
    .EnableABC = DEFAULT_EnableABC,
    .Threshold1_PPM = DEFAULT_Threshold1_PPM,
    .Threshold2_PPM = DEFAULT_Threshold2_PPM,
    .TemperatureOffset = DEFAULT_TemperatureOffset,
    .PressureOffset = DEFAULT_PressureOffset,
    .HumidityOffset = DEFAULT_HumidityOffset
};

sensors_state_t zclApp_Sensors = {
    .CO2 = 0.0,
    .CO2_PPM = 0,
    .Temperature = 0,
    .BME280_PressureSensor_MeasuredValue = 0,
    .BME280_HumiditySensor_MeasuredValue = 0,
    .BME280_PressureSensor_ScaledValue   = 0,
    .BME280_PressureSensor_Scale = -1
};

/*********************************************************************
 * ATTRIBUTE DEFINITIONS - Uses REAL cluster IDs
 */

CONST zclAttrRec_t zclApp_AttrsFirstEP[] = {
    {BASIC, {ATTRID_BASIC_ZCL_VERSION, ZCL_UINT8, R, (void *)&zclApp_ZCLVersion}},
    {BASIC, {ATTRID_BASIC_APPL_VERSION, ZCL_UINT8, R, (void *)&zclApp_ApplicationVersion}},
    {BASIC, {ATTRID_BASIC_STACK_VERSION, ZCL_UINT8, R, (void *)&zclApp_StackVersion}},
    {BASIC, {ATTRID_BASIC_HW_VERSION, ZCL_UINT8, R, (void *)&zclApp_HWRevision}},
    {BASIC, {ATTRID_BASIC_MANUFACTURER_NAME, ZCL_DATATYPE_CHAR_STR, R, (void *)zclApp_ManufacturerName}},
    {BASIC, {ATTRID_BASIC_MODEL_ID, ZCL_DATATYPE_CHAR_STR, R, (void *)zclApp_ModelId}},
    {BASIC, {ATTRID_BASIC_DATE_CODE, ZCL_DATATYPE_CHAR_STR, R, (void *)zclApp_DateCode}},
    {BASIC, {ATTRID_BASIC_POWER_SOURCE, ZCL_DATATYPE_ENUM8, R, (void *)&zclApp_PowerSource}},
    {BASIC, {ATTRID_BASIC_SW_BUILD_ID, ZCL_DATATYPE_CHAR_STR, R, (void *)zclApp_DateCode}},
    {BASIC, {ATTRID_CLUSTER_REVISION, ZCL_UINT16, R, (void *)&zclApp_clusterRevision_all}},

    {TEMP, {ATTRID_MS_TEMPERATURE_MEASURED_VALUE, ZCL_INT16, RR, (void *)&zclApp_Sensors.Temperature}},
    {TEMP, {ATTRID_TemperatureOffset, ZCL_INT16, RW, (void *)&zclApp_Config.TemperatureOffset}},

    {PRESSURE, {ATTRID_MS_PRESSURE_MEASUREMENT_MEASURED_VALUE, ZCL_INT16, RR, (void *)&zclApp_Sensors.BME280_PressureSensor_MeasuredValue}},
    {PRESSURE, {ATTRID_MS_PRESSURE_MEASUREMENT_SCALED_VALUE, ZCL_INT16, RR, (void *)&zclApp_Sensors.BME280_PressureSensor_ScaledValue}},
    {PRESSURE, {ATTRID_MS_PRESSURE_MEASUREMENT_SCALE, ZCL_INT8, R, (void *)&zclApp_Sensors.BME280_PressureSensor_Scale}},
    {PRESSURE, {ATTRID_PressureOffset, ZCL_INT32, RW, (void *)&zclApp_Config.PressureOffset}},


    {HUMIDITY, {ATTRID_MS_RELATIVE_HUMIDITY_MEASURED_VALUE, ZCL_UINT16, RR, (void *)&zclApp_Sensors.BME280_HumiditySensor_MeasuredValue}},
    {HUMIDITY, {ATTRID_HumidityOffset, ZCL_INT16, RW, (void *)&zclApp_Config.HumidityOffset}},

    {ZCL_CO2, {ATTRID_CO2_MEASURED_VALUE, ZCL_SINGLE, RR, (void *)&zclApp_Sensors.CO2}},
    {ZCL_CO2, {ATTRID_ENABLE_ABC, ZCL_DATATYPE_BOOLEAN, RW, (void *)&zclApp_Config.EnableABC}},
    {ZCL_CO2, {ATTRID_LED_FEEDBACK, ZCL_DATATYPE_BOOLEAN, RW, (void *)&zclApp_Config.LedFeedback}},
    {ZCL_CO2, {ATTRID_THRESHOLD1_PPM, ZCL_UINT16, RW, (void *)&zclApp_Config.Threshold1_PPM}},
    {ZCL_CO2, {ATTRID_THRESHOLD2_PPM, ZCL_UINT16, RW, (void *)&zclApp_Config.Threshold2_PPM}}
};


uint8 CONST zclApp_AttrsCount = (sizeof(zclApp_AttrsFirstEP) / sizeof(zclApp_AttrsFirstEP[0]));

const cId_t zclApp_InClusterList[] = {ZCL_CLUSTER_ID_GEN_BASIC};

#define APP_MAX_INCLUSTERS (sizeof(zclApp_InClusterList) / sizeof(zclApp_InClusterList[0]))

const cId_t zclApp_OutClusterList[] = {TEMP, HUMIDITY, PRESSURE, ZCL_CO2};


#define APP_MAX_OUT_CLUSTERS (sizeof(zclApp_OutClusterList) / sizeof(zclApp_OutClusterList[0]))


SimpleDescriptionFormat_t zclApp_FirstEP = {
    1,                             //  int Endpoint;
    ZCL_HA_PROFILE_ID,             //  uint16 AppProfId[2];
    ZCL_HA_DEVICEID_SIMPLE_SENSOR, //  uint16 AppDeviceId[2];
    APP_DEVICE_VERSION,            //  int   AppDevVer:4;
    APP_FLAGS,                     //  int   AppFlags:4;
    APP_MAX_INCLUSTERS,            //  byte  AppNumInClusters;
    (cId_t *)zclApp_InClusterList, //  byte *pAppInClusterList;
    APP_MAX_OUT_CLUSTERS,          //  byte  AppNumInClusters;
    (cId_t *)zclApp_OutClusterList //  byte *pAppInClusterList;
};


void zclApp_ResetAttributesToDefaultValues(void) {
    zclApp_Config.LedFeedback = DEFAULT_LedFeedback;
    zclApp_Config.EnableABC = DEFAULT_EnableABC;
    zclApp_Config.Threshold1_PPM = DEFAULT_Threshold1_PPM;
    zclApp_Config.Threshold2_PPM = DEFAULT_Threshold2_PPM;
    zclApp_Config.TemperatureOffset = DEFAULT_TemperatureOffset;
    zclApp_Config.HumidityOffset = DEFAULT_HumidityOffset;
    zclApp_Config.PressureOffset = DEFAULT_PressureOffset;
}