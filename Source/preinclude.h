#define TC_LINKKEY_JOIN
#define NV_INIT
#define NV_RESTORE

#define TP2_LEGACY_ZC
// patch sdk
// #define ZDSECMGR_TC_ATTEMPT_DEFAULT_KEY TRUE

#define NWK_AUTO_POLL
#define MULTICAST_ENABLED FALSE

#define ZCL_READ
#define ZCL_WRITE
#define ZCL_BASIC
#define ZCL_IDENTIFY
#define ZCL_REPORTING_DEVICE

#define DISABLE_GREENPOWER_BASIC_PROXY
#define BDB_FINDING_BINDING_CAPABILITY_ENABLED 1
#define BDB_REPORTING TRUE


#define HAL_BUZZER FALSE
#define HAL_KEY TRUE
#define ISR_KEYINTERRUPT


#define HAL_LED TRUE
#define HAL_ADC FALSE
#define HAL_LCD FALSE

#define BLINK_LEDS TRUE

// one of this boards
// #define HAL_BOARD_TARGET
// #define HAL_BOARD_CHDTECH_DEV

#if !defined(HAL_BOARD_TARGET) && !defined(HAL_BOARD_CHDTECH_DEV)
#error "Board type must be defined"
#endif

#if defined(HAL_BOARD_TARGET)
    #define HAL_KEY_P2_INPUT_PINS BV(0)
    #define CO2_UART_PORT 0x00
    #define HAL_UART_DMA 1
    #define HAL_UART_ISR 0
    #define INT_HEAP_LEN (2256 - 0xE)
#elif defined(HAL_BOARD_CHDTECH_DEV)
    #define HAL_UART_DMA 1
    #define HAL_UART_ISR 2
    #define CO2_UART_PORT  0x01
    #define HAL_KEY_P0_INPUT_PINS BV(1)
    #define DO_DEBUG_UART
#endif

#define FACTORY_RESET_HOLD_TIME_LONG 5000


#ifdef DO_DEBUG_UART
    #define HAL_UART TRUE
    #define HAL_UART_DMA 1
    #define INT_HEAP_LEN 2060
#endif

// #define INT_HEAP_LEN (2685 - 0x4B - 0xBB-0x50-0xae)
// #define HAL_UART TRUE
// #define HAL_UART_DMA 2
#define HAL_UART TRUE

#define BME280_32BIT_ENABLE
//i2c bme280
#define OCM_CLK_PORT 1
#define OCM_CLK_PIN 6

#define OCM_DATA_PORT 1
#define OCM_DATA_PIN 7


#define DS18B20_PORT 0
#define TSENS_SBIT P0_0
#define TSENS_BV BV(0)
#define TSENS_DIR P0DIR




#include "hal_board_cfg.h"
