#ifndef __MY_JOB_H__
#define __MY_JOB_H__

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "mcu.h"
#include "board.h"
// #include "led.h"
#define MAX_DATA    10

typedef struct{
    bool flag;
    uint32_t count;
    uint16_t data[MAX_DATA];
    uint16_t result;
} sensor_t;

typedef struct{
    uint8_t category;
    int16_t temp;
    uint16_t co2;
    uint16_t tvoc;
    uint16_t humidity;
    uint16_t pressure;
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    uint16_t gray;
    uint16_t battery;

    int16_t temp_result;
    uint16_t co2_result;
    uint16_t tvoc_result;
    uint16_t humidity_result;
    uint16_t pressure_result;
    uint16_t red_result;
    uint16_t green_result;
    uint16_t blue_result;
    uint16_t gray_result;
    uint16_t battery_result;
} data_t;

typedef struct{
    uint16_t year;
    uint16_t month;
    uint16_t day;
    uint16_t hour;
    uint16_t minute;
    uint16_t sec;
} uttecTime_t;

typedef enum{
    testPort1 = 0,
    testPort2 = 1,
    redLed = 2,
    blueLed = 3,
    BatCtr = 4,
} test_t;


#define HIGH    false 
#define LOW     true 

data_t* getMySensor(void);
uttecTime_t* getTime(void);

void testLed(void);
void doMyJob(void);


#endif //__MY_JOB_H__

