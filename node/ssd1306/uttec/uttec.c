/* The module name to be printed each lines. */
#define DEBUG_LOG_MODULE_NAME "DBUG APP"
/* The maximum log level printed for this module. */
#define DEBUG_LOG_MAX_LEVEL LVL_INFO

#include "debug_log.h"
#include "uttec.h"
#include "persistent.h"
#include "Uart.h"
#include "voltage.h"
#include "button.h"
#include "led.h"

#include "ssd1306.h"
#include "twi.h"
#include "lps22hb.h"
#include "ccs811.h"
#include "aht20.h"
#include "myJob.h"

flash_t myFlash = {0,};

void initUttec(void)
{
    Printf("+++++++++++++++ This is initUttec\r\n");
    initI2c();
    Led_init();
    Button_init();
    Mcu_voltageInit();
}

bool isFirst(void)
{

    Mcu_Persistent_read((uint8_t *)&myFlash, 0, sizeof(flash_t));
    
    if(myFlash.macId == (NRF_FICR->DEVICEID[0]&0x7fffffff)){
        Printf("++++++++++++ pass\r\n");
    }
    else{
        myFlash.flag = 1;
        myFlash.myId = 1;
        myFlash.areaId = 2;
        myFlash.company = 0;
        myFlash.macId = NRF_FICR->DEVICEID[0]&0x7fffffff;
        Mcu_Persistent_write((uint8_t *)&myFlash, 0, sizeof(flash_t));
        Printf("++++++++++++ First\r\n");
    }
    return true;
}

void parseAddress(char* pAddr){
    char addr[2][5] = {0,};
    int array = 0;
    int count = 0;
    char* pTemp = pAddr;
    for(int i = 0; i < 10; i++){
        char ch = *pTemp;
        if(((ch >= '0')&&(ch <= '9'))||(ch == ',')||(ch == ' ')||(ch == 0)) pTemp++;
        else{
            Printf("Error at data: %d\r\n", i);
            return;
        } 
    }

    for(int i = 0; i < 10; i++){
        if(*pAddr == ','){
            array++; count = 0;
            pAddr++;
        }
        else{
            addr[array][count++] = *pAddr++;
        }
    }
    
    myFlash.flag = 1;
    myFlash.myId = atoi(addr[1]);
    if(myFlash.myId >= 500){
        Printf("MyId Error: %d\r\n", myFlash.myId);
        myFlash.myId = 0;
        myFlash.areaId = 0;
        return;
    } 

    myFlash.areaId = atoi(addr[0]);
    Printf("{\"area\":%d, \"rack\":%d}\r\n", myFlash.areaId, myFlash.myId);
    // Printf("\r\n +++++++++++ Area: %d, MyId: %d\r\n", myFlash.areaId, myFlash.myId);
    myFlash.macId = NRF_FICR->DEVICEID[0]&0x7fffffff;
    Mcu_Persistent_write((uint8_t *)&myFlash, 0, sizeof(flash_t));
    Printf("++++++++++++++++++++++++++++++++++ end of flash address +++++++++++++++++\r\n");

}


void displayDeviceId(void)
{
    Printf("DEVICEID0: %08X\r\n", NRF_FICR->DEVICEID[0]);
    Printf("DEVICEID1: %08X\r\n", NRF_FICR->DEVICEID[1]);
    Printf("DEVICEID0: %ld\r\n", NRF_FICR->DEVICEID[0]&0x7fffffff);
    Printf("persistent size: %d\r\n", Mcu_Persistent_getMaxSize());
    Mcu_Persistent_read((uint8_t *)&myFlash, 0, sizeof(flash_t));
    Printf("flag: %d, myId: %d, mac: %ld\r\n", 
        myFlash.flag, myFlash.myId, myFlash.macId&0x7fffffff);
}

uint16_t getMyId(void)
{
    return myFlash.myId;
}

uint16_t getAreaId(void)
{
    return myFlash.areaId;
}

flash_t* getFlash(void){
    return &myFlash;
}

// void setUnlockDelayTime(void){
//     occupy_unlock_delay = UNLOCK_DELAY_TIMEOUT;
// }
#define SOH '{'
#define EOT '}'
#define OK_FLAG '@'
#define ADDR_SET_TIMEOUT 100 // 10 Seconds time out 
#define ADDR_BUF_LENGTH   10
void setAddress(void){
    static char addrBuf[ADDR_BUF_LENGTH] = {0,};
    static uint16_t uiUartTimeout = 0;
    static uint8_t ucAddressStatus = 0;
    static uint8_t ucAddrCount = 0;
    
    if(uiUartTimeout) uiUartTimeout--; //check address setting timeout
    else{ // when timeout, reset factor
        ucAddrCount = ucAddressStatus = 0;
        for(int i = 0; i < ADDR_BUF_LENGTH; i++) addrBuf[i] = 0;
    }

    if(isRx()){
        uint8_t rx = *getRx();
        if(rx == SOH){
            uiUartTimeout = ADDR_SET_TIMEOUT;
            // Printf("00000000 address ready\r\n");
            ucAddressStatus = 1; ucAddrCount = 0;
            for(int i = 0; i < ADDR_BUF_LENGTH; i++) addrBuf[i] = 0;
        }
        else if((rx == EOT)&&(ucAddressStatus == 1)){
            // Printf("11111111 address end\r\n");
            ucAddressStatus = 2;
        }
        else if((rx == OK_FLAG)&&(ucAddressStatus == 2)){
            // Printf("{\"rack\":%d}\r\n", rx);
            parseAddress(addrBuf);
        }
        else{
            if(ucAddressStatus == 1){
                addrBuf[ucAddrCount++] = rx;
            }
        }
        // Printf("++++++ input: %c\r\n", rx);
    }
}

uint32_t task_uttec(void)
{
    uint32_t m_uttec_time = 100;
    static uint32_t ulUttecCount = 1;

    setAddress(); //2022.07.18, set address
    if(!(ulUttecCount%10)){
        static bool toggle = false;
        toggle = !toggle;
        Printf("{\"area\":%d, \"rack\":%d}\r\n", myFlash.areaId, myFlash.myId);
    }
    doMyJob();
    ulUttecCount++;
    // Printf("*");
    return m_uttec_time;    //resetting task period:
}

void nrf_temp_init(void)
{
    /**@note Workaround for PAN_028 rev2.0A anomaly 31 - TEMP: Temperature offset value has to be manually loaded to the TEMP module */
    *(uint32_t *) 0x4000C504 = 0;
}

/** @brief Workaround specific define - sign mask.*/
#define MASK_SIGN           (0x00000200UL)

/** @brief Workaround specific define - sign extension mask.*/
#define MASK_SIGN_EXTENSION (0xFFFFFC00UL)

int32_t get_temperature(void){
    return ((NRF_TEMP->TEMP & MASK_SIGN) != 0) ?
                (int32_t)(NRF_TEMP->TEMP | MASK_SIGN_EXTENSION) : (NRF_TEMP->TEMP);

}

int32_t read_temp(void)
{
    int32_t temp;

    NRF_TEMP->TASKS_START = 1; /** Start the temperature measurement. */

    /* Busy wait while temperature measurement is not finished, you can skip waiting if you enable interrupt for DATARDY event and read the result in the interrupt. */
    /*lint -e{845} // A zero has been given as right argument to operator '|'" */
    while (NRF_TEMP->EVENTS_DATARDY == 0)
    {
        // Do nothing.
    }
    NRF_TEMP->EVENTS_DATARDY = 0;

    /**@note Workaround for PAN_028 rev2.0A anomaly 29 - TEMP: Stop task clears the TEMP register. */
    // temp = (get_temperature() / 4);
    temp = (int32_t)((get_temperature() * 100)/4); //real temp * 100

    /**@note Workaround for PAN_028 rev2.0A anomaly 30 - TEMP: Temp module analog front end does not power down when DATARDY event occurs. */
    NRF_TEMP->TASKS_STOP = 1; /** Stop the temperature measurement. */

    return temp;
}


