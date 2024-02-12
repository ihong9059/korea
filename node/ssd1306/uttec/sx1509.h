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

void setSX1509(void);

void testSx1509Port(void);

void sx1509Flow(void);

uint8_t getPortA(void);
uint8_t getPortB(void);

#endif /* __SX1509_H_ */
