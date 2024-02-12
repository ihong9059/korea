#ifndef __SX1509_H_
#define __SX1509_H_

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

#define SX1509_ADDR 0x3E


#define BIT_0 0B11111110
#define BIT_1 0B11111101
#define BIT_2 0B11111011
#define BIT_3 0B11110111
#define BIT_4 0B11101111
#define BIT_5 0B11011111
#define BIT_6 0B10111111
#define BIT_7 0B01111111


void setSX1509(void);

void testSx1509Port(void);

void sx1509Flow(void);

uint8_t getPortA(void);
uint8_t getPortB(void);
void setPortB(uint8_t);
void writeOutSx(uint8_t bitNum, bool output);

#endif /* __SX1509_H_ */
