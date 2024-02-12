#ifndef __TWI_H_
#define __TWI_H_

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "mcu.h"
#include "board.h"

/* The module name to be printed each lines. */
#define DEBUG_LOG_MODULE_NAME "DBUG APP"
/* The maximum log level printed for this module. */
#define DEBUG_LOG_MAX_LEVEL LVL_INFO

#include "debug_log.h"

typedef struct{
    bool lcdFlag;
    bool deviceFlag;
    uint8_t deviceAddr;
    char* pDeviceName;
} device_t;

void initI2c(void);

bool i2cScan(void);

void ponitReg(uint8_t addr, uint8_t reg);
void writeReg(uint8_t addr, uint8_t reg, uint8_t data);
void writeData(uint8_t addr, uint8_t* pData, uint8_t length);

void readReg(uint8_t addr, uint8_t reg, uint8_t* pData, uint8_t length);
void readRegOnly(uint8_t addr, uint8_t* pData, uint8_t length);

void readData(uint8_t addr, uint8_t* pReg, uint8_t* pData, uint8_t length);


void setDevice(uint8_t id);
void clearDevice(uint8_t id);
uint8_t isDevice(uint8_t id);
device_t* getDeviceFlag(void);

#endif /* __TWI_H_ */
