#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "shared_appconfig.h"
#include "api.h"
#include "node_configuration.h"

// #include "button.h"
#include "shared_data.h"
#include "app_scheduler.h"

#include "voltage.h"
#include "uttec.h"

#include "wirepas.h"
#include "mesh.h"
#include "Uart.h"

#include "debug_log.h"
#include "persistent.h"

#include "myJob.h"

diaper_t diaper = {0, 0, 0, 0};

uint32_t task_send_periodic_msg(void)
{
    static int16_t period_count = 0;
    payload_sensor_t payload; /* Message payload data. */
    data_t* pData = getMySensor();
    // flash_t* pFlash = getFlash();

    payload.area_id = getAreaId();
    payload.rack_id = getMyId();
    // payload.sensor0 = 1;
    // payload.sensor1 = 2;
    // payload.sensor3 = 3;
    // payload.sensor2 = 4;
    // payload.sensor4 = 5;
    payload.sensor0 = pData->category;
    switch(payload.sensor0){
        case 0:
            payload.sensor1 = pData->temp_result;
            payload.sensor2 = pData->humidity_result;
            payload.sensor3 = pData->pressure_result;
            payload.sensor4 = pData->battery_result;
        break;
        case 1:
            payload.sensor1 = pData->temp_result;
            payload.sensor2 = pData->co2_result;
            payload.sensor3 = pData->tvoc_result;
            payload.sensor4 = pData->battery_result;
        break;
        case 2:
            payload.sensor1 = pData->temp_result;
            payload.sensor2 = pData->gray_result;
            payload.sensor3 = pData->red_result;
            payload.sensor4 = pData->battery_result;
        break;
        case 3:
            payload.sensor1 = pData->temp_result;
            payload.sensor2 = pData->green_result;
            payload.sensor3 = pData->blue_result;
            payload.sensor4 = pData->battery_result;
        break;
    }
    payload.count = period_count++;

    if (send_uplink_msg(MSG_ID_PERIODIC_MSG,
                        (uint8_t *)&payload) != APP_LIB_DATA_SEND_RES_SUCCESS)
    {
    }
    return m_period_ms;
}

void send_echo_response_msg(uint32_t delay)
{
    payload_uttec_t payload; 
    send_uplink_msg(MSG_ID_ECHO_RESPONSE_MSG,(uint8_t *)&payload);
}

void send_uttec_response_msg(payload_uttec_t* pUttec)
{
    // payload_uttec_t payload; 
    /* Send message. */
    Printf("~~~~~~~~~~ send_uplink_msg\r\n");
    send_uplink_msg(MSG_ID_UTTEC_COMMAND_MSG,(uint8_t *)pUttec);
}

void set_periodic_msg_period(uint32_t new_period_ms)
{
    if ((new_period_ms >= PERIODIC_MSG_PERIOD_SET_MIN_VAL_MS) &&
            (new_period_ms <= PERIODIC_MSG_PERIOD_SET_MAX_VAL_MS))
    {
        m_period_ms = new_period_ms;

        /* Reschedule task to apply new period value. */
        App_Scheduler_addTask_execTime(task_send_periodic_msg,
                                       APP_SCHEDULER_SCHEDULE_ASAP,
                                       PERIODIC_WORK_EXECUTION_TIME_US);
    }
}
#define BIKE_RESET 99
app_lib_data_receive_res_e uttec_broadcast_data_received_cb(
        const shared_data_item_t * item,
        const app_lib_data_received_t * data)
{
    static uint32_t globalCount = 0;
    Printf("++++++++++ uttec_broadcast_data_received_cb: %d ++++++++++\r\n"
    , globalCount++);
    int8_t rssi = data->rssi;
    // int8_t tx_power = data->tx_power;
    size_t num_bytes = data->num_bytes;
    uint8_t* pData = (uint8_t *)data->bytes;
    msg_t receive_msg = {};
    uint8_t* pReceive_msg = (uint8_t *)&receive_msg;

    uttecTime_t* pTime = getTime();

    for(uint8_t i = 0; i < num_bytes; i++){
        *pReceive_msg++ = *pData++;
    }
    Printf("cmd: %d\r\n", receive_msg.id);
    Printf("area: %d\r\n", receive_msg.payload.test.area_id);
    Printf("rack: %d\r\n", receive_msg.payload.test.rack_id);
    Printf("Year: %d\r\n", receive_msg.payload.test.year);
    Printf("Month: %d\r\n", receive_msg.payload.test.month);
    Printf("Day: %d\r\n", receive_msg.payload.test.day);
    Printf("Hour: %d\r\n", receive_msg.payload.test.hour);
    Printf("Minute: %d\r\n", receive_msg.payload.test.minute);
    Printf("Sec: %d\r\n", receive_msg.payload.test.sec);
    Printf("rssi: %d\r\n", rssi);
    pTime->year = receive_msg.payload.test.year;
    pTime->month = receive_msg.payload.test.month;
    pTime->day = receive_msg.payload.test.day;
    pTime->hour = receive_msg.payload.test.hour;
    pTime->minute = receive_msg.payload.test.minute;
    pTime->sec = receive_msg.payload.test.sec;
    if(receive_msg.payload.test.rack_id == BIKE_RESET){
        // NVIC_SystemReset();
    }
    return APP_LIB_DATA_RECEIVE_RES_HANDLED;
}

app_lib_data_receive_res_e unicast_broadcast_data_received_cb(
        const shared_data_item_t * item,
        const app_lib_data_received_t * data)
{
    Printf(" ------- uttec_unicast_data_received_cb ------- \r\n");
    return APP_LIB_DATA_RECEIVE_RES_HANDLED;
}

app_lib_data_receive_res_e uttec_unicast_data_received_cb(
        const shared_data_item_t * item,
        const app_lib_data_received_t * data)
{
    static int16_t receive_cnt = 0;
    Printf(" ------- uttec_unicast_data_received_cb ------- \r\n");
    int8_t rssi = data->rssi;
    int8_t tx_power = data->tx_power;
    size_t num_bytes = data->num_bytes;
    app_lib_settings_net_addr_t net_addr;
    app_lib_settings_net_channel_t net_ch;

    uint8_t* pData = (uint8_t *)data->bytes;
    msg_t receive_msg = {};
    uint8_t* pReceive_msg = (uint8_t *)&receive_msg;

    for(uint8_t i = 0; i < num_bytes; i++){
        *pReceive_msg++ = *pData++;
    }
    Printf("cmd: %d\r\n", receive_msg.id);
    Printf("area: %d\r\n", receive_msg.payload.uttec.area_id);
    Printf("rack: %d\r\n", receive_msg.payload.uttec.rack_id);
    Printf("cmd: %d\r\n", receive_msg.payload.uttec.rack_status);
    Printf("receive rssi: %d, tx_power: %d, num_bytes: %d\r\n", rssi, tx_power, num_bytes);
    app_addr_t node_addr = getUniqueAddress();
    lib_settings->getNodeAddress(&node_addr);
    Printf("nodeAddress: %d, %x\r\n", node_addr, node_addr);
    lib_settings->getNetworkAddress(&net_addr);
    lib_settings->getNetworkChannel(&net_ch);
    Printf("NetworkAddress: %d, NetworkChannel: %d\r\n", net_addr, net_ch);
    node_addr = getUniqueAddress();
    Printf("uuid: %d, %x\r\n", node_addr, node_addr);

    // send_uplink_msg('A', );
    payload_uttec_t tempMsg;
    tempMsg.area_id = getAreaId();
    tempMsg.rack_id = getMyId();
    tempMsg.rack_status = 0;
    tempMsg.temperature = read_temp();
    tempMsg.sensor_0 = 25;
    tempMsg.sensor_1 = 26;
    tempMsg.rssi = rssi;
    // setRssi(rssi);
    tempMsg.count = receive_cnt++;
    // for(uint8_t i = 0; i < sizeof(payload_uttec_t); i++){
    //     tempMsg.test[i] = 'A' + i;
    // }
    send_uttec_response_msg(&tempMsg);
    return APP_LIB_DATA_RECEIVE_RES_HANDLED;
}



