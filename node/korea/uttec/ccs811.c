#include "uttec.h"
#include "twi.h"

#include "ccs811.h"
#include "myJob.h"

void runCCS811(void){
  uint8_t r_data = 0;
  r_data = 0x10; // constant power mode (001), no interrupts
  // r_data = 0x40; // constant power mode (001), no interrupts
  writeReg(CCS811_ADDR, MEAS_MODE, r_data);
  Printf("+++++++++++++++ runCCS811 +++++++++++++++\r\n");
}

void initCCS811(void){
  uint8_t r_data = 0;
  //setSX1509();

  readReg(CCS811_ADDR, Who_CCS811, &r_data, sizeof(r_data));
  if(r_data == 0x81){
    Printf("Find CCS811_ADDR chip. 2026.06.24 12:10\r\n");
    ponitReg(CCS811_ADDR, 0xf4);
  }
  else{
    Printf("++++++++++ Not Fine CCS811 chip\r\n");
  }

  readReg(CCS811_ADDR, CCS811_STATUS, &r_data, sizeof(r_data));
  Printf("Status: %02x\r\n", r_data);

  Printf("end of initCCS811, and wait 2Sec\r\n");
  // nrf_delay_ms(2000);
  Printf("now read data from CCS811_ADDR\r\n");
}

void shutdownCCS811(void){
  uint8_t r_data = 0;
  writeReg(CCS811_ADDR, MEAS_MODE, r_data);
}

void ccs811ReadValues(void){
  uint8_t r5_data[5] = {0,};
  uint8_t r_data = 0;

  uint16_t CO2 = 0;
  uint16_t TVOC = 0;
  data_t* pData = getMySensor();

  readReg(CCS811_ADDR, 0x02, r5_data, sizeof(r5_data)); //5bytes

  if(r5_data[4] & 1){
    Printf("Dtat read Error!!!!!.\r\n");
    readReg(CCS811_ADDR, 0x00, &r_data, sizeof(r_data)); //1bytes
    Printf("Status: %x\r\n", r_data);
    readReg(CCS811_ADDR, 0xE0, &r_data, sizeof(r_data));
    Printf("ErrorID: %x\r\n", r_data);
  }

  if((r5_data[4] & 0x99) == 0x98){
    CO2 = (r5_data[0] << 8) + r5_data[1];
    TVOC = (r5_data[2] << 8) + r5_data[3];  
    // Printf("Ok Data\r\n");
    Printf("CO2: %d, TVOC: %d\r\n", CO2, TVOC);
    pData->co2 = CO2; pData->tvoc = TVOC;
  }
  else{
    CO2 = (r5_data[0] << 8) + r5_data[1];
    TVOC = (r5_data[2] << 8) + r5_data[3];  
    // Printf("Ok Data\r\n");
    Printf("CO2: %d, TVOC: %d\r\n", CO2, TVOC);
    Printf("------------ check data again: %x\r\n", r5_data[4]);
  }


  if(CO2 > 2000|| TVOC > 1200){
    Printf("------------ This Data Error data\r\n");
  }
}

void dispCcs811(void){
  Printf("dispCcs811. \r\n");
}

/*

ccs811 flow
- reset: low 
- 100msec delay
- reset: high

- 1 sec delay

- initCCS811();

- 2 sec delay
- ccs811ReadValues();

*/
#define CCS811_RESET_TIME   1
#define CCS811_INIT         11
#define CCS811_RUN          50
#define CCS811_READ         80
#define CCS811_READ_TIME    10

void ccs811Flow(void){
    static uint32_t ccs811Count = 0;
    if(ccs811Count < CCS811_RESET_TIME){
        // Led_set(testPort1, LOW);
        // Led_set(testPort2, HIGH);
    }
    else{
        // Led_set(testPort1, HIGH);
        // Led_set(testPort2, LOW);
    }
    if(ccs811Count == CCS811_INIT){
        initCCS811();
    }
    else if(ccs811Count == CCS811_RUN){
        runCCS811();
    }
    else if(ccs811Count > CCS811_READ){
        static uint8_t readCountDown = CCS811_READ_TIME;
        if(readCountDown) readCountDown--;
        else{
            readCountDown = CCS811_READ_TIME;
            ccs811ReadValues();
        }
    }
    ccs811Count++;
}



