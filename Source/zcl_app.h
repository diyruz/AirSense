#ifndef ZCL_APP_H
#define ZCL_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
 * INCLUDES
 */
#include "version.h"
#include "zcl.h"

/*********************************************************************
 * CONSTANTS
 */
#define APP_REPORT_DELAY ((uint32)60 * (uint32)1000) // 1 minute

// Application Events
#define APP_REPORT_EVT 0x0001
#define APP_SAVE_ATTRS_EVT 0x0002
#define APP_READ_SENSORS_EVT 0x0004
/*********************************************************************
 * MACROS
 */
#define NW_APP_CONFIG 0x0402

#define R ACCESS_CONTROL_READ
// ACCESS_CONTROL_AUTH_WRITE
#define RW (R | ACCESS_CONTROL_WRITE | ACCESS_CONTROL_AUTH_WRITE)
#define RR (R | ACCESS_REPORTABLE)

#define BASIC ZCL_CLUSTER_ID_GEN_BASIC
#define GEN_ON_OFF ZCL_CLUSTER_ID_GEN_ON_OFF
#define POWER_CFG ZCL_CLUSTER_ID_GEN_ON
#define TEMP ZCL_CLUSTER_ID_MS_TEMPERATURE_MEASUREMENT
#define HUMIDITY    ZCL_CLUSTER_ID_MS_RELATIVE_HUMIDITY
#define PRESSURE    ZCL_CLUSTER_ID_MS_PRESSURE_MEASUREMENT

//Carbon Dioxide (CO2)
#define ZCL_CO2     0x040d

enum {
  EBME280 = 0,
  EDS18B20 = 1,
  ENOTFOUND = 2
};

enum {  
  ABC_DISABLED = 0,
  ABC_ENABLED = 1,
  ABC_NOT_AVALIABLE = 0xFF,
};
  
// Carbon Dioxide (CO2)
#define ATTRID_CO2_MEASURED_VALUE   0x0000
#define ATTRID_CO2_TOLERANCE        0x0003
#define ATTRID_ENABLE_ABC           0x0202
#define ATTRID_LED_FEEDBACK         0x0203
#define ATTRID_THRESHOLD1_PPM       0x0204
#define ATTRID_THRESHOLD2_PPM       0x0205

#define ATTRID_TemperatureOffset    0x0210
#define ATTRID_PressureOffset       0x0210
#define ATTRID_HumidityOffset       0x0210
   
#define ZCL_UINT8 ZCL_DATATYPE_UINT8
#define ZCL_UINT16 ZCL_DATATYPE_UINT16
#define ZCL_INT16 ZCL_DATATYPE_INT16
#define ZCL_INT8  ZCL_DATATYPE_INT8
#define ZCL_INT32 ZCL_DATATYPE_INT32
#define ZCL_UINT32 ZCL_DATATYPE_UINT32
#define ZCL_SINGLE ZCL_DATATYPE_SINGLE_PREC
/*********************************************************************
 * TYPEDEFS
 */

typedef struct {
    uint8 LedFeedback;
    uint8 EnableABC;
    uint16 Threshold1_PPM;
    uint16 Threshold2_PPM;
    int16 TemperatureOffset;
    int32 PressureOffset;
    int16 HumidityOffset;
} application_config_t;

typedef struct {
    float CO2;
    int16 CO2_PPM;
    int16 Temperature;
    int16 BME280_Temperature_Sensor_MeasuredValue;
    uint16 BME280_HumiditySensor_MeasuredValue;
    int16 BME280_PressureSensor_MeasuredValue;
    int16 BME280_PressureSensor_ScaledValue;
    int8 BME280_PressureSensor_Scale;
} sensors_state_t;

/*********************************************************************
 * VARIABLES
 */

extern SimpleDescriptionFormat_t zclApp_FirstEP;
extern CONST zclAttrRec_t zclApp_AttrsFirstEP[];
extern CONST uint8 zclApp_AttrsCount;


extern const uint8 zclApp_ManufacturerName[];
extern const uint8 zclApp_ModelId[];
extern const uint8 zclApp_PowerSource;

extern application_config_t zclApp_Config;
extern sensors_state_t zclApp_Sensors;
// APP_TODO: Declare application specific attributes here

/*********************************************************************
 * FUNCTIONS
 */

/*
 * Initialization for the task
 */
extern void zclApp_Init(byte task_id);

/*
 *  Event Process for the task
 */
extern UINT16 zclApp_event_loop(byte task_id, UINT16 events);

extern void zclApp_ResetAttributesToDefaultValues(void);

/*********************************************************************
 *********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* ZCL_APP_H */
