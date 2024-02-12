#include "shared_data.h"

#include "uttec.h"
#include "wirepas.h"

#include "i2c.h"
#include "debug_log.h"

bool i2cOkFlag = false;

static i2c_conf_t m_i2c_conf = {
    .clock = 100000,
    // .clock = 150000,
    // .pullup = NRF_GPIO_PIN_NOPULL,
    .pullup = NRF_GPIO_PIN_PULLUP,
};

void initI2c(void)
{
    i2c_res_e res;
    res = I2C_init(&m_i2c_conf);

    if (res == I2C_RES_OK || res == I2C_RES_ALREADY_INITIALIZED){
        Printf("i2c ok\r\n");
        Printf("sda: %d, scl: %d\r\n", BOARD_I2C_SDA_PIN, BOARD_I2C_SCL_PIN);
    }
    i2cOkFlag = true;
}


void ponitReg(uint8_t addr, uint8_t reg){
    i2c_res_e res;
    uint8_t pointReg = reg;
    i2c_xfer_t xfer_tx = {
    .address = addr,
    .write_ptr = &pointReg,
    .write_size = 1,
    .read_ptr = NULL,
    .read_size = 0};

    res = I2C_transfer(&xfer_tx, NULL);
    if(res != I2C_RES_OK){
        if(i2cOkFlag)
            NVIC_SystemReset();
        Printf("--I2C Error @pointReg addr: %x, reg: %x\r\n", addr, reg);
    }
}

void writeReg(uint8_t addr, uint8_t reg, uint8_t data){
    i2c_res_e res;
    uint8_t w_data[2] = {reg, data}; //RGBC measurement time = 160 ms
    i2c_xfer_t xfer_tx = {
    .address = addr,
    .write_ptr = w_data,
    .write_size = 2,
    .read_ptr = NULL,
    .read_size = 0};

    res = I2C_transfer(&xfer_tx, NULL);
    if(res != I2C_RES_OK){
        if(i2cOkFlag)
            NVIC_SystemReset();
        Printf("--I2C Error @writeReg addr: %x, reg: %x\r\n", addr, reg);
    }
}

void writeData(uint8_t addr, uint8_t* pData, uint8_t length){
    i2c_res_e res;
    i2c_xfer_t xfer_tx = {
    .address = addr,
    .write_ptr = pData,
    .write_size = length,
    .read_ptr = NULL,
    .read_size = 0};

    res = I2C_transfer(&xfer_tx, NULL);
    if(res != I2C_RES_OK){
        if(i2cOkFlag)
            NVIC_SystemReset();
        Printf("--I2C Error @writeReg addr: %x, reg: %x\r\n", addr);
    }
}

void readReg(uint8_t addr, uint8_t reg, uint8_t* pData, uint8_t length){
    i2c_res_e res;
    uint8_t r_reg = reg;
    i2c_xfer_t xfer_rx = {
    .address =  addr,          
    .write_ptr = &r_reg,          
    .write_size = 1,         
    .read_ptr = pData,     
    .read_size = length,
    .custom = 0};

    res = I2C_transfer(&xfer_rx, NULL);
    if(res != I2C_RES_OK){
        if(i2cOkFlag)
            NVIC_SystemReset();
        Printf("--I2C Error @readReg addr: %x, reg: %x\r\n", addr, reg);
    }
}

void readRegOnly(uint8_t addr, uint8_t* pData, uint8_t length){
    i2c_res_e res;
    i2c_xfer_t xfer_rx = {
    .address =  addr,          
    .write_ptr = NULL,          
    .write_size = 0,         
    .read_ptr = pData,     
    .read_size = length,
    .custom = 0};

    res = I2C_transfer(&xfer_rx, NULL);
    if(res != I2C_RES_OK){
        if(i2cOkFlag)
            NVIC_SystemReset();
        Printf("--I2C Error @readReg addr: %x\r\n", addr);
    }
}

void readData(uint8_t addr, uint8_t* pReg, uint8_t* pData, uint8_t length){
    i2c_res_e res;
    i2c_xfer_t xfer_rx = {
    .address =  addr,          
    .write_ptr = pReg,          
    .write_size = 1,         
    .read_ptr = pData,     
    .read_size = length,
    .custom = 0};

    res = I2C_transfer(&xfer_rx, NULL);
    if(res != I2C_RES_OK){
        if(i2cOkFlag)
            NVIC_SystemReset();
        Printf("--I2C Error @readReg addr: %x\r\n", addr);
    }
}

i2c_res_e testI2C(uint8_t addr){
    i2c_res_e res;
    uint8_t r_data = 0;
    i2c_xfer_t xfer_rx = {
    .address =  addr,          
    .write_ptr = NULL,          
    .write_size = 0,         
    .read_ptr = &r_data,     
    .read_size = 1,
    .custom = 0};

    res = I2C_transfer(&xfer_rx, NULL);
    return res;
}

#include "twi.h"

#include "aht20.h"
#include "bmp280.h"
#include "ssd1306.h"
#include "bh1745.h"
#include "bh1750.h"
#include "ccs811.h"
#include "eeprom.h"
#include "lps22hb.h"
#include "sx1509.h"

static uint8_t devices[127] = {0,};
static device_t deviceFlag = {0,};
static char deviceName[14] = {0,};

void setDevice(uint8_t id){
    devices[id] = 1;
}

void clearDevice(uint8_t id){
    devices[id] = 0;
}

uint8_t isDevice(uint8_t id){
    uint8_t uId = id;
    uint8_t result = devices[uId];
    return result;
}

device_t* getDeviceFlag(void){
    return &deviceFlag;
}

#include <stdio.h>

void checkDevice(void){
    uint8_t count = 126;
    deviceFlag.pDeviceName = deviceName;
    while(count--){
        uint8_t temp = isDevice(count);
        if(temp){
            switch(count){
                case AHT20_ADDRESS: //0x38
                    Printf("Device AHT20_ADDRESS\r\n");
                    deviceFlag.deviceFlag = true;
                    deviceFlag.deviceAddr = AHT20_ADDRESS;
                    sprintf(deviceName, "AHT20,BMP");
                break;
                case BH1745_ADDR: //0x39
                    deviceFlag.deviceFlag = true;
                    deviceFlag.deviceAddr = BH1745_ADDR;
                    sprintf(deviceName, "BH1745");
                    Printf("Device BH1745_ADDR\r\n");
                break;
                case BH1750_ADDR: //0x23
                    deviceFlag.deviceFlag = true;
                    deviceFlag.deviceAddr = BH1750_ADDR;              
                    sprintf(deviceName, "BH1750");
                    Printf("Device BH1750_ADDR\r\n");
                break;
                case BMP280_ADDR: //0x77
                    Printf("Device BMP280_ADDR\r\n");
                    deviceFlag.deviceFlag = true;
                    deviceFlag.deviceAddr = BMP280_ADDR;
                    sprintf(deviceName, "AHT20,BMP");
                break;
                case SX1509_ADDR: //0x3e
                    Printf("Device SX1509_ADDR\r\n");
                    deviceFlag.deviceFlag = true;
                    deviceFlag.deviceAddr = SX1509_ADDR;
                    sprintf(deviceName, "SX1509");
                break;
                case CCS811_ADDR: //0x5A
                    Printf("Device CCS811_ADDR\r\n");
                break;
                case EEPROM: //0x50
                    Printf("Device EEPROM\r\n");
                break;
                case LPS22HB_ADDR: //0x5C
                    Printf("Device LPS22HB\r\n");
                break;
                case SSD1306_ADDR: //0x3C
                    Printf("Device SSD1306_ADDR\r\n");
                    deviceFlag.lcdFlag = true;
                break;
            }
        }
    }
}

bool i2cScan(void){
    // Printf("-------------- start of i2cScan --------------\r\n");
    bool errorFlag = true;
    i2c_res_e err_code;
    for (uint8_t address = 1; address <= 127; address++)
    {
      err_code = testI2C(address);
      if (err_code == I2C_RES_OK)
      {
        setDevice(address);
        Printf("------ %02x\r\n", address);
      }      
    }

    checkDevice();

    return errorFlag;
}


