#ifndef __UTTEC_H_
#define __UTTEC_H_
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "mcu.h"
#include "board.h"

#define OP_LED 22 //for bike
// #define OP_LED 22 //for diaper
#define MSG_LED 23

#define LED_A 15

#define SOLENOID0 22
#define SOLENOID1 22
#define POWER 24 

#define SOLENOID00 22
#define SOLENOID11 22

#define LEAF_SW 20  

#define BikeOnTime 5
// #define UNLOCK_DELAY_TIMEOUT 60
#define UNLOCK_DELAY_TIMEOUT 5 //by tokai, when switch on, locking after 5 Sec

typedef struct{
    uint8_t company;
    uint8_t site;
    uint16_t flag;
    uint16_t myId;
    uint16_t areaId;
    uint32_t count;
    uint32_t macId;
} flash_t;

void initUttec(void);

bool isFirst(void);
void displayDeviceId(void);

uint16_t getMyId(void);
uint16_t getAreaId(void);

flash_t* getFlash(void);

void parseAddress(char* pAddr);

uint32_t task_uttec(void);

void nrf_temp_init(void);
int32_t read_temp(void);

#endif /* __UTTEC_H_ */
