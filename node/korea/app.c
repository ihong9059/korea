#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "shared_appconfig.h"
#include "api.h"
#include "node_configuration.h"

#include "shared_data.h"
#include "app_scheduler.h"
#include "mesh.h"
#include "persistent.h"

#include "uttec.h"
#include "wirepas.h"
#include "debug_log.h"
#include "Uart.h"

#include "ssd1306.h"
#include "twi.h"
#include "eeprom.h"
#include "lps22hb.h"
#include "ccs811.h"
#include "aht20.h"
#include "hw_delay.h"

#include "myJob.h"
#include "mpu9250.h"
#include "sx1509.h"

#define ENABLE_LOW_LATENCY_MODE

#define THIS_COMPANY    4 //for ssd1306

void setCiper(void){
    uint8_t authen_key[] = {NET_CIPHER_KEY};
    uint8_t cipher_key[] = {NET_CIPHER_KEY};
    
    flash_t *pFlash = getFlash();
    pFlash->company = THIS_COMPANY;
    // pFlash->areaId = 11;
    pFlash->site = pFlash->areaId % 256;
    // pFlash->site = 33; //for set site 2023.05.27 16:00
    Printf("pFlash->site: %d\r\n", pFlash->site);
    authen_key[0] = cipher_key[0] = pFlash->company;
    authen_key[1] = cipher_key[1] = pFlash->areaId;
    uint32_t channel = (pFlash->areaId - 1)%40 + 1;
    // NETWORK_ADDRESS = NETWORK_CHANNEL = channel; //Error
    
    app_res_e res = configureNode(getUniqueAddress(), channel, channel, authen_key, cipher_key);
    if(res == APP_RES_OK){
        Printf("--------- configureNode ----------- OK\r\n");
    }
    else{
        Printf("+++++++++ configureNode Fail\r\n ");
    }
    // configureNode(getUniqueAddress(), NETWORK_ADDRESS, NETWORK_CHANNEL, authen_key, cipher_key);
    Printf("channel: %d, site: %d\r\n", channel, authen_key[1]);
    // Printf("channel: %d, %d, %d, %d\r\n", sizeof(NETWORK_ADDRESS), sizeof(NETWORK_CHANNEL), sizeof(authen_key),sizeof(authen_key));
    // Printf("value: %08X, %08X\r\n", NETWORK_ADDRESS, NETWORK_CHANNEL);
    Printf("getUniqueAddress: %ld\r\n", getUniqueAddress());
}


void App_init(const app_global_functions_t * functions)
{
    initSystem();
#ifdef ENABLE_LOW_LATENCY_MODE
    Printf("\r\n-------- demo for korea by hks. 2024.02.09 14:00 -----------\r\n");
#else 
    Printf("\r\n++++++++++ Tokai Encryption type. 2022.12.02 10:55 LATENCY_MODE ++++++++++++++\r\n");
#endif
    
    displayDeviceId();

    // getFlash()->myId = 7; //for test 2022.06.10
    initUart();
    initUttec();

    isFirst();

    setCiper();

    i2cScan();
    // while(1);

    // init_Ssd1306();
    // initMpu9250();
    setSX1509();
    shared_app_config_filter_t app_config_period_filter;

    /* App config. */
    Shared_Appconfig_init();
    /* Prepare the app_config filter for measurement rate. */
    app_config_period_filter.type = CUSTOM_PERIOD_TYPE;
    app_config_period_filter.cb = appConfigPeriodReceivedCb;
    Shared_Appconfig_addFilter(&app_config_period_filter, &m_filter_id);

    // Printf("this is my printf: %d\r\n", 4321);
    // if (configureNodeFromBuildParameters() != APP_RES_OK)
    // {
    //     return;
    // }

#ifdef ENABLE_LOW_LATENCY_MODE
    lib_settings->setNodeRole(
            app_lib_settings_create_role(APP_LIB_SETTINGS_ROLE_HEADNODE,
                                        APP_LIB_SETTINGS_ROLE_FLAG_LL |
                                        APP_LIB_SETTINGS_ROLE_FLAG_AUTOROLE));
    Printf("+++++++++++++++++  ENABLE_LOW_LATENCY_MODE\r\n");
    // if(isLeafSw()){ // low latency test 2022.10.15
    //     lib_settings->setNodeRole(
    //             app_lib_settings_create_role(APP_LIB_SETTINGS_ROLE_HEADNODE,
    //                                         APP_LIB_SETTINGS_ROLE_FLAG_LL |
    //                                         // APP_LIB_SETTINGS_ROLE_FLAG_LE |
    //                                         APP_LIB_SETTINGS_ROLE_FLAG_AUTOROLE));
    //     Printf("+++++++++++++++++  ENABLE_LOW_LATENCY_MODE\r\n");
    // }
    // else{
    //     lib_settings->setNodeRole(
    //             app_lib_settings_create_role(APP_LIB_SETTINGS_ROLE_HEADNODE,
    //                                         APP_LIB_SETTINGS_ROLE_FLAG_AUTOROLE));
    //     Printf("-----------------  LATENCY_MODE\r\n");
    // }
#endif
    Shared_Data_init();
    App_Scheduler_init();

    m_period_ms = DEFAULT_PERIOD_MS;
    App_Scheduler_addTask_execTime(task_send_periodic_msg,
                                   APP_SCHEDULER_SCHEDULE_ASAP,
                                   PERIODIC_WORK_EXECUTION_TIME_US);

    App_Scheduler_addTask_execTime(task_uttec,
                                   APP_SCHEDULER_SCHEDULE_ASAP,
                                   PERIODIC_WORK_EXECUTION_TIME_US);

    Printf("PERIODIC_WORK_EXECUTION_TIME_US: %d\r\n", PERIODIC_WORK_EXECUTION_TIME_US);
    /* Set unicast & broadcast received messages callback. */
    Shared_Data_addDataReceivedCb(&unicast_packets_filter);
    Shared_Data_addDataReceivedCb(&broadcast_packets_filter);

    lib_state->startStack();
    // while(1);
    // hw_delay_init();
}
