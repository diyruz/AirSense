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
#define ZCL_ON_OFF

#define DISABLE_GREENPOWER_BASIC_PROXY
#define BDB_FINDING_BINDING_CAPABILITY_ENABLED 1
#define BDB_REPORTING TRUE

#ifdef DEFAULT_CHANLIST
#undef DEFAULT_CHANLIST
#endif
#define DEFAULT_CHANLIST 0x07FFF800

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
    #define APP_TX_POWER TX_PWR_PLUS_19
    #define SENSEAIR_UART_PORT 0x00
    #define HAL_UART_DMA 1
    #define HAL_UART_ISR 0
    #define INT_HEAP_LEN (2685-0x26-0xCC)
#elif defined(HAL_BOARD_CHDTECH_DEV)
    #define HAL_KEY_P0_INPUT_PINS BV(1)
    #define APP_TX_POWER TX_PWR_PLUS_4
    #define DO_DEBUG_UART
    #define HAL_UART_ISR 2
#endif

#define FACTORY_RESET_HOLD_TIME_LONG 3000


#ifdef DO_DEBUG_UART
    #define HAL_UART TRUE
    #define HAL_UART_DMA 1
    #define INT_HEAP_LEN (2685 - 0x4B - 0xBB-0x50-0xae-0x50)
#endif

// #define INT_HEAP_LEN (2685 - 0x4B - 0xBB-0x50-0xae)
// #define HAL_UART TRUE
// #define HAL_UART_DMA 2
#define HAL_UART TRUE


#define TSENS_SBIT P0_0
#define TSENS_BV BV(0)
#define TSENS_DIR P0DIR




#include "hal_board_cfg.h"
