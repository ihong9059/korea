#include "uttec.h"
#include "twi.h"
#include "lps22hb.h"
#include "myJob.h"

void initLPS22HB(void){
  uint8_t r_data = 0;
  uint8_t r2_data[2] = {0,};
  //uint8_t LPS_CTRL_REG1         =  0x10;
  //uint8_t LPS_CTRL_REG2         =  0x11;
  //uint8_t who_am_i         =  0x0f;
  readReg(LPS22HB_ADDR, LPS_CTRL_REG2, r2_data, sizeof(r2_data));
  // Printf("data0: %d, data1: %d\n\r", r2_data[0], r2_data[1]);

  readReg(LPS22HB_ADDR, LPS_CTRL_REG2, &r_data, sizeof(r_data));
  // Printf("data: %d\n\r", r_data);

  readReg(LPS22HB_ADDR, who_am_i, &r_data, sizeof(r_data));
  // Printf("who_am_i: %x\n\r", r_data);

  writeReg(LPS22HB_ADDR, LPS_CTRL_REG1, 0x02); 
  //#Low-pass filter disabled , output registers not updated until MSB and LSB

  Printf("initLPS22HB\n\r");
}

void setOneShotLPS22HB(void){
  uint8_t r_data = 0;
  readReg(LPS22HB_ADDR, LPS_CTRL_REG2, &r_data, sizeof(r_data));
  r_data |= 0x01;
  writeReg(LPS22HB_ADDR, LPS_CTRL_REG2, r_data);
  // Printf("setOneShotLPS22HB\r\n");
}


void readLPS22HB(void){
  data_t* pData = getMySensor();
  uint8_t r3_data[3] = {0,};
  uint8_t r_data = 0;
  // setOneShotLPS22HB();
  // nrf_delay_ms(10);
  readReg(LPS22HB_ADDR, LPS_STATUS, &r_data, sizeof(r_data));
  if((r_data&0x01)==0x01){
    // Printf("get new pressure data\r\n");
    readReg(LPS22HB_ADDR, LPS_PRESS_OUT_XL, &r_data, sizeof(r_data));
    r3_data[0] = r_data;
    readReg(LPS22HB_ADDR, LPS_PRESS_OUT_L, &r_data, sizeof(r_data));
    r3_data[1] = r_data;
    readReg(LPS22HB_ADDR, LPS_PRESS_OUT_H, &r_data, sizeof(r_data));
    r3_data[2] = r_data;
    uint16_t PRESS_DATA=((r3_data[2]<<16)+(r3_data[1]<<8)+r3_data[0])/4096;
    uint16_t PRESS_DATA_1=((r3_data[2]<<16)+(r3_data[1]<<8)+r3_data[0])%4096;
    // Printf("pressure: %d.%d\r\n", PRESS_DATA, PRESS_DATA_1);
    pData->pressure = PRESS_DATA * 10 + PRESS_DATA_1 / 100;
    Printf("pressure1: %d\r\n", pData->pressure);
  }
  readReg(LPS22HB_ADDR, LPS_STATUS, &r_data, sizeof(r_data));
  if((r_data&0x02)==0x02){
    readReg(LPS22HB_ADDR, LPS_TEMP_OUT_L, &r_data, sizeof(r_data));
    r3_data[0] = r_data;
    readReg(LPS22HB_ADDR, LPS_TEMP_OUT_H, &r_data, sizeof(r_data));
    r3_data[1] = r_data;
    uint16_t TEMP_DATA=((r3_data[1]<<8)+r3_data[0])/100;
    uint16_t TEMP_DATA_1=((r3_data[1]<<8)+r3_data[0])%100;
    // Printf("LPS22HB, temperature: %d.%d\r\n", TEMP_DATA, TEMP_DATA_1);
    pData->temp = TEMP_DATA*100 + TEMP_DATA_1;
    Printf("temp1: %d\r\n", pData->temp);
  }
}

#include <stdio.h>

#include "ssd1306.h"

#define LPS22HB_INIT    1
#define LPS22HB_RUN     10

void lps22hbFlow(void){
    static uint32_t lpcCount = 0;
    if(lpcCount == LPS22HB_INIT){
        initLPS22HB();
    }
    else if(lpcCount > LPS22HB_RUN){
        if(!(lpcCount % 10)){
            setOneShotLPS22HB();
        }
        else if((lpcCount % 10) == 5){
            readLPS22HB();
        }
    }
    lpcCount++;
}



