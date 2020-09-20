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

// Carbon Dioxide (CO2)
#define ATTRID_MS_TEMPERATURE_MEASUREMENT_CO2_LEVEL_MEASURED_VALUE 0x0200
#define ATTRID_CO2_TOLERANCE 0x0201
#define ATTRID_DISABLE_ABC 0x0202
#define ATTRID_LED_FEEDBACK 0x0203
#define ATTRID_THRESHOLD1_PPM 0x0204
#define ATTRID_THRESHOLD2_PPM 0x0205

#define ATTRID_MS_PRESSURE_MEASUREMENT_MEASURED_VALUE_HPA               0x0200


#define ZCL_UINT8 ZCL_DATATYPE_UINT8
#define ZCL_UINT16 ZCL_DATATYPE_UINT16
#define ZCL_INT16 ZCL_DATATYPE_INT16
#define ZCL_UINT32 ZCL_DATATYPE_UINT32
#define ZCL_SINGLE ZCL_DATATYPE_SINGLE_PREC
/*********************************************************************
 * TYPEDEFS
 */

typedef struct {
    uint16 SensorTolerance;
    uint8 LedFeedback;
    uint8 DisableABC;
    uint16 Threshold1_PPM;
    uint16 Threshold2_PPM;
} application_config_t;

typedef struct {
    uint16 CO2_PPM;
    int16 Temperature;
    int16 BME280_Temperature_Sensor_MeasuredValue;
    int16 BME280_PressureSensor_MeasuredValue;
    uint32 BME280_PressureSensor_MeasuredValueHPA;
    uint16 BME280_HumiditySensor_MeasuredValue;
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
