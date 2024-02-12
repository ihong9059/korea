#include "uttec.h"

// #include "bike.h"
#include "twi.h"

#include "sx1509.h"


void setDirectionA(uint8_t direction){
  writeReg(SX1509_ADDR, 0x0f, direction);//Direction A input
}

void setDirectionB(uint8_t direction){
  writeReg(SX1509_ADDR, 0x0e, direction);//Direction B
}

void setSX1509(void){
  // writeReg(SX1509_ADDR, 0x00, 0xff);// Input B disable
  writeReg(SX1509_ADDR, 0x00, 0x00);// Input B enable
  // writeReg(SX1509_ADDR, 0x01, 0xff);// Input A disable
  writeReg(SX1509_ADDR, 0x01, 0x00);// Input A enable

  writeReg(SX1509_ADDR, 0x06, 0xff);//Disable Pullup B
  writeReg(SX1509_ADDR, 0x07, 0xff);//Disable Pullup A
  // writeReg(SX1509_ADDR, 0x0a, 0xff);//Enable open drain B
  // writeReg(SX1509_ADDR, 0x0b, 0xff);//Enable open drain A
  writeReg(SX1509_ADDR, 0x0e, 0x00);//Direction B
  // writeReg(SX1509_ADDR, 0x0f, 0x00);//Direction A output
  writeReg(SX1509_ADDR, 0x0f, 0xff);//Direction A input
  // writeReg(SX1509_ADDR, 0x1e, 0x0f);//Enable OSC
  writeReg(SX1509_ADDR, 0x1f, 0x00);//MISC
  writeReg(SX1509_ADDR, 0x20, 0x00);//Disable LED Driver operation B
  writeReg(SX1509_ADDR, 0x21, 0x00);//Disable LED Driver operation A
  

  writeReg(SX1509_ADDR, 0x10, 0xff);//LED Driver Data B
  writeReg(SX1509_ADDR, 0x11, 0xff);//LED Driver Data A

  setDirectionA(0xff);//input
  setDirectionB(0x00);//input

  Printf("end of testTwi\n");
//   NRF_LOG_FLUSH();
}

void setPortA(uint8_t port){
  writeReg(SX1509_ADDR, 0x11, port);//LED Driver Data A
}

void setPortB(uint8_t port){
  writeReg(SX1509_ADDR, 0x10, port);//LED Driver Data B
}

// #include "shared_appconfig.h"

#include "myJob.h"
// #include "mesh.h"
uint32_t task_send_periodic_msg(void);

uint8_t getPortA(void){
  uint8_t nowPortA;
  static uint8_t before = 0;
  data_t* pData = getMySensor();
  readReg(SX1509_ADDR, 0x11, &nowPortA, 1);
  if(nowPortA != before){
    before = nowPortA;
    pData->pressure = before;
    task_send_periodic_msg();
  }
  return nowPortA;
}

uint8_t getPortB(void){
  uint8_t result = 0;
  data_t* pData = getMySensor();
  readReg(SX1509_ADDR, 0x10, &result, 1);
  pData->humidity = result;

  return result;
}


void testSx1509Port(void){
  static bool toggle = false;
  if(toggle){
    Printf("sx1509 port ALL HIGH\r\n");
    writeReg(SX1509_ADDR, 0x10, 0xff);//LED Driver Data B
    // writeReg(SX1509_ADDR, 0x11, 0xff);//LED Driver Data A
  }
  else{
    Printf("sx1509 port ALL LOW\r\n");
    writeReg(SX1509_ADDR, 0x10, 0x00);//LED Driver Data B
    // writeReg(SX1509_ADDR, 0x11, 0x00);//LED Driver Data A
  }
  toggle = !toggle;
}

void test1_sx1509Port(uint8_t value){
    writeReg(SX1509_ADDR, 0x10, value);//LED Driver Data B
    // writeReg(SX1509_ADDR, 0x11, value);//LED Driver Data A
}

#define SX1509_INIT    1
#define SX1509_RUN     10

void sx1509Flow(void){
    static uint32_t sx1509Count = 0;

    if(sx1509Count == SX1509_INIT){
      setSX1509();
    }
    else if(sx1509Count > SX1509_RUN){
      static uint8_t test = 1;
      if(!(sx1509Count % 20)){
        test = 1;
        Printf("PortB: %02x\r\n", getPortA());
        testSx1509Port();
      }
      // test1_sx1509Port(test);
      test <<= 1;
    }
    sx1509Count++;
}

// void setRelay(uint8_t port, ){
//   setPortB();
// }
void writeOutSx(uint8_t bitNum, bool output){
	uint8_t regB = getPortB();
	// int err;
  bitNum = bitNum - 1;
  Printf("port: %d, outpu: %d\r\n", bitNum, output);
	// Printf("~BIT_0: %x\r\n", (uint8_t)~BIT_0);

	switch(bitNum){
		case 0:
			if(output) regB = regB | (uint8_t)~BIT_0;
			else regB = regB & BIT_0;
		break;
		case 1:
			if(output) regB = regB | (uint8_t)~BIT_1;
			else regB = regB & BIT_1;
		break;
		case 2:
			if(output) regB = regB | (uint8_t)~BIT_2;
			else regB = regB & BIT_2;
		break;
		case 3:
			if(output) regB = regB | (uint8_t)~BIT_3;
			else regB = regB & BIT_3;
		break;
		case 4:
			if(output) regB = regB | (uint8_t)~BIT_4;
			else regB = regB & BIT_4;
		break;
		case 5:
			if(output) regB = regB | (uint8_t)~BIT_5;
			else regB = regB & BIT_5;
		break;
		case 6:
			if(output) regB = regB | (uint8_t)~BIT_6;
			else regB = regB & BIT_6;
		break;
		case 7:
			if(output) regB = regB | (uint8_t)~BIT_7;
			else regB = regB & BIT_7;
		break;
	}
  setPortB(regB);
	Printf("regB: %x\r\n", regB);
}

