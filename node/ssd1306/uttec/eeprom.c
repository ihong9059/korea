#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "shared_data.h"
#include "wirepas.h"
#include "debug_log.h"

#include "uttec.h"

#include "twi.h"
#include "bh1745.h"
#include "myJob.h"
#include "eeprom.h"

static uint16_t length = 0;

void writeEeprom(void){
  static int writeCount = 3;
  Printf("write eeprom simulator with nordic\n\r");
  char buffer[TEST_SIZE]; /* Addr + data */
  uint16_t addr = 0;
  memcpy(buffer, &addr, 2); //set eeprom address 
  length = sprintf(buffer + 2, "writeEeprom:%d\r\n", writeCount);
  // memcpy(buffer + 2, pdata, size);
    // ret = nrf_drv_twi_tx(&m_twi_master, EEPROM_SIM_ADDR, buffer, size + EEPROM_SIM_ADDRESS_LEN_BYTES, false);
  writeData(EEPROM_SIM_ADDR, (uint8_t *)buffer, length+2);
  writeCount++;
  Printf("end of write eeprom\n\r");
}

void readEeprom(void){
  // static int readCount = 0;
  Printf("read eeprom simulator with nordic\n\r");
  uint8_t buffer[length]; /* Addr + data */
  // uint16_t addr16 = addr;
  // ret = nrf_drv_twi_tx(&m_twi_master, EEPROM_SIM_ADDR, (uint8_t *)&addr16, EEPROM_SIM_ADDRESS_LEN_BYTES, true);
  // if (NRF_SUCCESS != ret)
  // {
  //     break;
  // }
  // ret = nrf_drv_twi_rx(&m_twi_master, EEPROM_SIM_ADDR, pdata, size);
  writeReg(EEPROM_SIM_ADDR, 0, 0); //set eeprom address: 0
  readRegOnly(EEPROM_SIM_ADDR, buffer, length);
  // sprintf((char *)buffer, "writeEeprom:%d\r\n", readCount);
  Printf("read data: %s", buffer);
  Printf("length: %d\r\n", length);
  Printf("end of read eeprom\n\r");
}

