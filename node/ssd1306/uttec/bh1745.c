#include "uttec.h"

#include "twi.h"
#include "bh1745.h"
#include "myJob.h"

void setBH1745(void){
  Printf("setBH1745\n\r");
  writeReg(BH1745_ADDR, 0x41, 0x00);//RGBC measurement time = 160 ms
  writeReg(BH1745_ADDR, 0x42, 0x90);//RGBC measurement active, Gain = 1X
  writeReg(BH1745_ADDR, 0x44, 0x02);//Default value
  Printf("end of setBH1745\n\r");
}
#define BH1745_RATE 10
void readBH1745(void){
    uint8_t r_data[8] = {0,};
    // writeReg(BH1745_ADDR, 0x50, 0x00);//set read register 0x50
    // ponitReg(BH1745_ADDR, 0x50);

    readReg(BH1745_ADDR, 0x50, r_data, sizeof(r_data));// 8bytes 

    uint16_t red = r_data[1] * 256 + r_data[0];
    uint16_t green = r_data[3] * 256 + r_data[2];
    uint16_t blue = r_data[5] * 256 + r_data[4];
    uint16_t cData = r_data[7] * 256 + r_data[6];
    // Printf("red: %d, green: %d, blue: %d, cData: %d\r\n", red, green, blue, cData);
    data_t* pData = getMySensor();
    pData->red = red/BH1745_RATE; pData->green = green/BH1745_RATE; 
    pData->blue = blue/BH1745_RATE; pData->gray = cData/BH1745_RATE;

}

#define BH1745_INIT    1
#define BH1745_RUN     20
#define BH1745_DELAY   2

#include "myJob.h"

void bh1745Flow(void){
    static uint32_t bh1745Count = 0;

    if(bh1745Count == BH1745_INIT){
        setBH1745();
    }
    else if(bh1745Count > BH1745_RUN){
      readBH1745();
    }
    bh1745Count++;
}


