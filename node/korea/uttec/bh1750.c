#include "uttec.h"

#include "twi.h"
#include "bh1750.h"
#include "myJob.h"

void _set_mode(uint8_t mode){
  ponitReg(BH1750_ADDR, mode);
    // self.bus.write_byte(self.addr, self.mode)
}

void powerDown(void){
  _set_mode(POWER_DOWN);
}

void powerOn(void){
  _set_mode(POWER_ON);
}

void reset(void){
  powerOn();
  _set_mode(RESET);
}

void bh1750Init(void){
  powerDown();
}

void trigger(void){
  reset();
  _set_mode(RESOLUTION_MODE);
  //wait 0.128 sec
}

uint16_t getResult(uint8_t mode){
  uint8_t r_data[2] = {0,};
  readReg(BH1750_ADDR, mode, r_data, sizeof(r_data));// 8bytes 
  uint16_t result = (uint16_t)(((r_data[0]<<8) | r_data[1])/1.2);
  powerDown();
  return result;
}

#define BH1750_INIT    1
#define BH1750_RUN     20
#define BH1750_DELAY   2

#include "myJob.h"

void bh1750Flow(void){
    static uint32_t bh1750Count = 0;

    if(bh1750Count == BH1750_INIT){
        bh1750Init();
    }
    else if(bh1750Count > BH1750_RUN){
        if(!(bh1750Count % 10)){
          trigger();
        }
        else if((bh1750Count % 10) == BH1750_DELAY){
          data_t* pData = getMySensor();
          uint16_t result = getResult(RESOLUTION_MODE);
          Printf("result: %d\r\n", result);
          pData->gray = result;
          pData->temp = read_temp();
        }
    }
    bh1750Count++;
}
