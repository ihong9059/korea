#include "shared_data.h"

#include "uttec.h"
#include "wirepas.h"

/* The module name to be printed each lines. */
#define DEBUG_LOG_MODULE_NAME "DBUG APP"
/* The maximum log level printed for this module. */
#define DEBUG_LOG_MAX_LEVEL LVL_INFO

#include "debug_log.h"
#include "led.h"

#include "myJob.h"
#include "ccs811.h"
#include "aht20.h"
#include "twi.h"
#include "lps22hb.h"
#include "bmp280.h"
#include "bh1750.h"

sensor_t temp_Data = {0,};
sensor_t co2_Data = {0,};
sensor_t tvoc_Data = {0,};
sensor_t humidity_Data = {0,};
sensor_t pressure_Data = {0,};
sensor_t red_Data = {0,};
sensor_t green_Data = {0,};
sensor_t blue_Data = {0,};
sensor_t gray_Data = {0,};
sensor_t battery_Data = {0,};

data_t mySensor = {0,};
uttecTime_t myTime = {0,};

void averageInt(sensor_t* psData, int16_t input, int16_t* pResult){
    int32_t total = 0;
    psData->data[psData->count%MAX_DATA] = input;
    psData->count++;
    if(psData->count < MAX_DATA){
        for(uint32_t i = 0; i < psData->count; i++){
            total += psData->data[i] ;
        }
        *pResult = total / psData->count;
    }
    else{
        for(uint32_t i = 0; i < MAX_DATA; i++){
            total += psData->data[i] ;
        }
        *pResult = total / MAX_DATA;
    }
}

void average(sensor_t* psData, uint16_t input, uint16_t* pResult){
    int32_t total = 0;
    psData->data[psData->count%MAX_DATA] = input;
    psData->count++;
    if(psData->count < MAX_DATA){
        for(uint32_t i = 0; i < psData->count; i++){
            total += psData->data[i] ;
        }
        *pResult = total / psData->count;
    }
    else{
        for(uint32_t i = 0; i < MAX_DATA; i++){
            total += psData->data[i] ;
        }
        *pResult = total / MAX_DATA;
    }
}

void procAverage(void){
    data_t* pData = getMySensor();
    averageInt(&temp_Data, pData->temp, &pData->temp_result);
    average(&co2_Data, pData->co2, &pData->co2_result);
    average(&tvoc_Data, pData->tvoc, &pData->tvoc_result);
    average(&humidity_Data, pData->humidity, &pData->humidity_result);
    average(&pressure_Data, pData->pressure, &pData->pressure_result);
    average(&red_Data, pData->red, &pData->red_result);
    average(&green_Data, pData->green, &pData->green_result);
    average(&blue_Data, pData->blue, &pData->blue_result);
    average(&gray_Data, pData->gray, &pData->gray_result);
    average(&battery_Data, pData->battery, &pData->battery_result);
}

uttecTime_t* getTime(void){
    return &myTime;
}

data_t* getMySensor(void){
    return &mySensor;
}

void testLed(void){
    Led_toggle(redLed);
    Led_toggle(blueLed);
    // Led_toggle(testPort1);
    // Led_toggle(testPort2);
}

void setLed(uint8_t led, bool ctr){
    Led_set(led - 1, ctr);
}

#include "ssd1306.h"
#include "bh1745.h"
#include "sx1509.h"

#include <stdio.h>

void dispDevice(void){
    device_t* pDevice = getDeviceFlag();
    data_t* pData = getMySensor();
    char line[15] = {0,};

    int16_t temp = pData->temp;
    uint16_t pressure = pData->pressure;
    uint8_t remain = temp % 100;
    uint16_t humidity = pData->humidity;

    if(pDevice->deviceFlag){
        // Printf("dispDevice:%02x\r\n", pDevice->deviceAddr);
        switch(pDevice->deviceAddr){
            case AHT20_ADDRESS:
            case BMP280_ADDR:
            case LPS22HB_ADDR:
                pData->category = 0;
                temp /= 100;
                humidity /= 100;
                sprintf(line,"%02d.%01d H:%02d", temp, remain, humidity);
                dispLcdLine(line, 2);
                
                remain = pressure % 10;
                pressure /= 10;
                sprintf(line,"p:%04d.%01d", pressure, remain);
                dispLcdLine(line, 3);
            break;

            case BH1745_ADDR:
                pData->category = 2;
                sprintf(line,"%04d,%04d", pData->red, pData->green);
                dispLcdLine(line, 2);
                sprintf(line,"%04d,%04d", pData->blue, pData->gray);
                dispLcdLine(line, 3);
            break;

            case BH1750_ADDR:
                pData->category = 2;
                sprintf(line,"lx:%d", pData->gray);
                dispLcdLine(line, 2);
                int16_t bat = pData->battery;
                uint16_t remain = bat % 100;
                bat /= 100;
                sprintf(line,"Bat:%d.%02d", bat, remain);
                dispLcdLine(line, 3);
            break;

            case SX1509_ADDR:
                dispCountDown();
            break;

            case CCS811_ADDR:
                pData->category = 1;
            break;
        }
    }
    else{
        if(pDevice->lcdFlag){
            dispCountDown();
        }
        // else{
        //     Printf("no Lcd\r\n");
        // }
    }
}

void runDevice(void){
    data_t* pData = getMySensor();
    device_t* pDevice = getDeviceFlag();
    if(pDevice->deviceFlag){
        // Printf("runDevice: %02x\r\n", pDevice->deviceAddr);
        switch(pDevice->deviceAddr){
            case AHT20_ADDRESS:
                aht20Flow();
                bmp280Flow();
            break;
            case BMP280_ADDR:
                aht20Flow();
                bmp280Flow();
            break;
            case LPS22HB_ADDR:
                lps22hbFlow();
            break;

            case BH1745_ADDR:
                bh1745Flow();
                pData->category = 2;
            break;

            case BH1750_ADDR:
                bh1750Flow();
            break;

            case CCS811_ADDR:
                pData->category = 1;
            break;
        }
        // bh1750Flow();
        // ccs811Flow();
        // lps22hbFlow();
    }
}

void beep(uint8_t beepTime){
    static uint8_t myTime = 0;
    if(beepTime) myTime = beepTime;
    else{
        if(myTime){
            myTime--;
            Led_set(2, true);        
        }
        else{
            Led_set(2, false);        
        } 
    }
}

#include "voltage.h"
#include "button.h"
#include "mpu9250.h"

int16_t getAdc(uint8_t channel){
    setAdcChannel(channel);
    return Mcu_voltageGet();
}

void doMyJob(void){
    data_t* pData = getMySensor();
    static uint32_t countMyJob = 0;
    static bool testFlag = false;

    beep(0);
    if(!(countMyJob%10)){
        static uint8_t myJobSec = 0;
        static bool toggle = false;
        procAverage();
        if(!(myJobSec % 10)){
            toggle = !toggle;
        }
    
        int16_t battery = getAdc(0)*2;
        int16_t solar = getAdc(2)*2;

        Printf("myJobCount: %d, bat: %d, sol: %d\r\n", myJobSec, battery, solar);
        Printf("temp: %d\r\n", pData->temp);
        Printf("finalTemp: %d\r\n", pData->temp_result);

        if(battery < 0) battery = 0;
        pData->battery = battery*200 / 376;
        Printf("bat------------>: %d\r\n", pData->battery);
        int16_t red = getAdc(2)*2;
        if(red < 0) red = 0;
        pData->red = red;

        myJobSec++;
        // dispDevice();
        bool myButton;
        Button_getState(0, &myButton);
        if(myButton){
            Printf("Button High ----------------\r\n");
            NVIC_SystemReset();
        } 
        Printf("temp: %d, humid: %d, press: %d, bat: %d, rssi: %d, count: %d\r\n", 
        pData->temp, pData->humidity, pData->pressure, pData->battery,
        pData->rssi, pData->count);
        testFlag = !testFlag;
        // if(testFlag)
        //     setPortB(0xff);
        // else
        //     setPortB(0x00);
        Printf("---------portA: %x, portB: %x\r\n", 
            getPortA(), getPortB());    
        // readAcc();
    }
    runDevice();
    countMyJob++;
}