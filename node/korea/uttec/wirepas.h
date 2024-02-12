#ifndef APP_WIREPAS_H_
#define APP_WIREPAS_H_

#include "shared_appconfig.h"

/** 최소 periodic msg time 2Sec **/
#define PERIODIC_MSG_PERIOD_SET_MIN_VAL_MS      (2000ul)
/** 최대 periodic msg time 1200Sec **/
#define PERIODIC_MSG_PERIOD_SET_MAX_VAL_MS      (1200000ul)

/** Endpoint used to communicate. */
#define DATA_EP        (1u)
// #define DATA_EP        (2u)

/** Convert message travel delay from 1/128th second to milliseconds. */
#define COARSE_TO_MS(delay)    (1000u * (delay) >> 7)

/** Application periodic message fixed data pattern byte length. */
// #define PERIODIC_MSG_DATA_PATTERN_LEN   (8u)
#define PERIODIC_MSG_DATA_PATTERN_LEN   (10u)

/** Application periodic message sending period default value in seconds. */
#define DEFAULT_PERIOD_S    (2u)
// #define DEFAULT_PERIOD_S    (10u)
// #define DEFAULT_PERIOD_S    (60u)
// #define DEFAULT_PERIOD_S    (120u) //2Minutes
// #define DEFAULT_PERIOD_S    (300u) //5Minutes
// #define DEFAULT_PERIOD_S    (1200u) //20Minutes
// #define DEFAULT_PERIOD_S    (1800u) //30Minutes
// #define DEFAULT_PERIOD_S    (3600u) //60Minutes
// #define DEFAULT_PERIOD_S    (300u) //5Minutes
/**
 * Application periodic message sending period default value in milliseconds.
 */
#define DEFAULT_PERIOD_MS   (DEFAULT_PERIOD_S*1000u)

/** The application will register for this type of app_config, corresponding to the measurement rate. */
#define CUSTOM_PERIOD_TYPE 0xC3 

/** Time needed to execute the periodic work, in us. */
#define PERIODIC_WORK_EXECUTION_TIME_US (250u)

/** Time needed to execute the send "button pressed message" task, in us. */
#define TASK_EXEC_TIME_US_SEND_BUTTON_PRESSED_MSG (250u)


/* The module name to be printed each lines. */
#define DEBUG_LOG_MODULE_NAME "DBUG APP"
/* The maximum log level printed for this module. */
#define DEBUG_LOG_MAX_LEVEL LVL_INFO
/* debug_log.h can be included in multiple c files with an other name or log
 * level
 */


#define UTTEC_FRAME_LENGTH 16
#define TEST_FRAME_LENGTH 8

typedef struct  __attribute__((packed))
{
    uint32_t interval;    /** Measurment rate in seconds. */
} app_config_t;

typedef enum
{
    MSG_ID_PERIODIC_MSG = 0,
    MSG_ID_ECHO_RESPONSE_MSG = 2,
    MSG_ID_PERIODIC_MSG_PERIOD_SET_MSG = 128,
    MSG_ID_ECHO_COMMAND_MSG = 131,
    MSG_ID_UTTEC_COMMAND_MSG = 133
} message_id_e;

// typedef struct __attribute__((packed))
// {
//     uint32_t counter;
//     short sensor;
//     short battery;
//     short myId;
//     short test1;
//     uint8_t status;
//     uint8_t solenoid;
// } payload_periodic_t;
typedef struct __attribute__((packed))
{
    int16_t area_id;        //1
    int16_t rack_id;        //2
    int16_t rack_status;    //3
    int16_t temperature;    //4
    int16_t sensor_0;       //5
    int16_t sensor_1;       //6
    int16_t rssi;           //7
    int16_t count;          //8
} payload_periodic_t;

typedef struct __attribute__((packed))
{
    uint32_t new_period_ms;
} payload_periodic_set_t;

typedef struct __attribute__((packed))
{
    /** Sink to node "echo command" travel time in milliseconds. */
    uint32_t travel_time_ms;
} payload_response_echo_t;

typedef struct __attribute__((packed))
{
    /** Sink to node "echo command" travel time in milliseconds. */
    int16_t area_id;        //1
    int16_t rack_id;        //2
    int16_t rack_status;    //3
    int16_t temperature;    //4
    int16_t sensor_0;       //5
    int16_t sensor_1;       //6
    int16_t rssi;           //7
    int16_t count;          //8
    // uint8_t test[UTTEC_FRAME_LENGTH];
} payload_uttec_t;

typedef struct __attribute__((packed))
{
    /** Sink to node "echo command" travel time in milliseconds. */
    int16_t area_id;        //1
    int16_t rack_id;        //2
    int16_t ctr;    //3
    int16_t relay;    //4
    int16_t sensor_0;       //5
    int16_t sensor_1;       //6
    int16_t rssi;           //7
    int16_t battery;          //8
    // uint8_t test[UTTEC_FRAME_LENGTH];
} payload_demo_t;

typedef struct __attribute__((packed))
{
    int16_t area_id;        //1
    int16_t rack_id;        //2
    int16_t sensor0;    //3
    int16_t sensor1;    //4
    int16_t sensor2;       //5
    int16_t sensor3;       //6
    int16_t sensor4;           //7
    int16_t count;          //8
} payload_sensor_t;

typedef struct __attribute__((packed))
{
    /** Sink to node "echo command" travel time in milliseconds. */
    int16_t area_id;        //1
    int16_t rack_id;        //2
    int16_t year;    //3
    int16_t month;    //4
    int16_t day;       //5
    int16_t hour;       //6
    int16_t minute;           //7
    int16_t sec;          //8
} payload_test_t;

typedef struct __attribute__((packed))
{
/** @brief Application message data structure. */
    uint8_t id;
    union
    {
        payload_periodic_t               periodic;
        payload_periodic_set_t           periodic_set_period;
        payload_response_echo_t          resp_echo;
        payload_uttec_t                  uttec;   
        payload_test_t                   test;   
        payload_demo_t                   demo;   
    } payload;
} msg_t;


extern shared_data_item_t unicast_packets_filter;
extern shared_data_item_t broadcast_packets_filter;
extern uint16_t m_filter_id;
extern uint32_t m_period_ms;
// extern uint32_t m_uttec_time;

app_lib_data_send_res_e send_uplink_msg(message_id_e id,
                                               uint8_t * payload);



#endif /* APP_CONFIG_H_ */

