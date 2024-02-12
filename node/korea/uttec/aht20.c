#include "shared_data.h"

#include "uttec.h"
#include "wirepas.h"

#include "aht20.h"
#include "twi.h"

static uint8_t a_aht20_iic_read(uint8_t *data, uint16_t len)
{
    readRegOnly(AHT20_ADDRESS, data, len);
    return 0;                                          /* success return 0 */
}

static uint8_t a_aht20_calc_crc(uint8_t *data, uint8_t len)
{
    uint8_t i;
    uint8_t byte;
    uint8_t crc = 0xFF;
    
    for (byte = 0; byte < len; byte++)          /* len times */
    {
        crc ^= data[byte];                      /* xor byte */
        for (i = 8; i > 0; --i)                 /* one byte */
        {
            if ((crc & 0x80) != 0)              /* if high*/
            {
                crc = (crc << 1) ^ 0x31;        /* xor 0x31 */
            }
            else
            {
                crc = crc << 1;                 /* skip */
            }
        }
    }
    
    return crc;                                 /* return crc */
}

uint8_t aht20_read_temperature_humidity(uint32_t *temperature_raw, float *temperature_s,
                                        uint32_t *humidity_raw, uint16_t *humidity_s)
{
    uint8_t status;
    uint8_t buf[7];
    
    a_aht20_iic_read(&status, 1);                    /* read the status */
    if ((status & 0x80) == 0)                                         /* check the status */
    {
        uint8_t reg = 0x71;
        readData(AHT20_ADDRESS, &reg, buf, 7);
        // a_aht20_iic_read(buf, 7);                    /* read data */
        if (a_aht20_calc_crc(buf, 6) != buf[6])                       /* check the crc */
        {
            Printf("aht20: crc is error.\r\n");
        }
        // Printf("~~~~~~~~~~~ %02x,%02x,%02x,%02x\r\n", buf[0], buf[1], buf[2], buf[3] );        
        // Printf("~~~~~~~~~~~ %02x,%02x,%02x\r\n", buf[4], buf[5], buf[6]);        
        *humidity_raw = (((uint32_t)buf[1]) << 16) |
                        (((uint32_t)buf[2]) << 8) |
                        (((uint32_t)buf[3]) << 0);                    /* set the humidity */
        *humidity_raw = (*humidity_raw) >> 4;                         /* right shift 4 */
        *humidity_s = (uint16_t)((float)(*humidity_raw)
                                / 1048576.0f * 10000.0f);               /* convert the humidity */
                                // / 1048576.0f * 100.0f);               /* convert the humidity */
        *temperature_raw = (((uint32_t)buf[3]) << 16) |
                           (((uint32_t)buf[4]) << 8) |
                           (((uint32_t)buf[5]) << 0);                 /* set the temperature */
        *temperature_raw = (*temperature_raw) & 0xFFFFF;              /* cut the temperature part */
        *temperature_s = (float)(*temperature_raw) 
                                 / 1048576.0f * 200.0f
                                 - 50.0f;                             /* right shift 4 */
        
        return 0;                                                     /* success return 0 */
    }
    else
    {
        Printf("aht20: data is not ready.\r\n");
        return 4;                                                     /* return error */
    }
    return 0;                                                       /* success return 0 */
}

aht20_info_t myInfo = {0,};

uint8_t aht20_info(void)
{
    
    memset(&myInfo, 0, sizeof(aht20_info_t));                          /* initialize aht20 info structure */
    strncpy(myInfo.chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(myInfo.manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(myInfo.interface, "IIC", 8);                             /* copy interface name */
    // strncpy(myInfo.supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    // strncpy(myInfo.supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    // strncpy(myInfo.max_current_ma = MAX_CURRENT;                             /* set maximum current */
    // strncpy(myInfo.temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    // strncpy(myInfo.temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    // strncpy(myInfo.driver_version = DRIVER_VERSION;                          /* set driver version */
    Printf("chip_name:%s\r\n", myInfo.chip_name);
    Printf("manufacturer_name:%s\r\n", myInfo.manufacturer_name);
    Printf("interface:%s\r\n", myInfo.interface);

    Printf("max_current_ma:%s\r\n", MAX_CURRENT);
    Printf("supply_voltage_min_v:%s\r\n", SUPPLY_VOLTAGE_MIN);
    Printf("supply_voltage_max_v:%s\r\n", SUPPLY_VOLTAGE_MAX);
    Printf("temperature_max:%s\r\n", TEMPERATURE_MAX);
    Printf("temperature_min:%s\r\n", TEMPERATURE_MIN);
    Printf("driver_version:%s\r\n", DRIVER_VERSION);
    return 0;                                                       /* success return 0 */
}

aht20_info_t* getAht20Info(void){
    return &myInfo;
}

#define AHT20_INIT  2
#define AHT20_RUN   20
#define AHT20_READ  2
#define STATUS_CAL  0x08
#define STATUS_READ 0x80
#define CMD_SIZE    4

void aht20Reset(void){
    // uint8_t command[3] = {0xbe, 0x08, 0x00};
    uint8_t command[CMD_SIZE] = {0x00, 0xbe, 0x08, 0x00};
    writeData(AHT20_ADDRESS, command, CMD_SIZE);
}

uint8_t aht20Status(void){
    uint8_t r_data = 0;
    readReg(AHT20_ADDRESS, 0x71, &r_data, 1);
    // Printf("++++++++++++++++ aht20Status: %02x\r\n", r_data);
    if(r_data & 0x08){
        // Printf("+++++++++++++ ok status: ");
    }
    else{
        Printf("--------------- Fail Status: %02x\r\n", r_data);
    }
    return r_data;
}

void aht20Run(void){
    // uint8_t command[3] = {0xac, 0x33, 0x00};
    uint8_t command[CMD_SIZE] = {0x00, 0xac, 0x33, 0x00};
    writeData(AHT20_ADDRESS, command, CMD_SIZE);
}

#include "myJob.h"

void aht20Flow(void){
    static uint32_t aht20Count = 0;
    if(aht20Count == AHT20_INIT){
        Printf("--------------- aht20Reset ------------\r\n");
        aht20Reset();
    }
    else if(aht20Count > AHT20_RUN){
        if(!(aht20Count % 10)){
            if(aht20Status() & STATUS_CAL){
                // Printf("+++++++++++++ aht20Run\r\n");
                aht20Run();
            }
            else{
                Printf("------------- error at status\r\n");
            }
        }
        else if((aht20Count % 10) == AHT20_READ){
            if(aht20Status() & STATUS_READ){
                Printf("------------- not completed.\r\n");
            }
            else{
                // Printf("+++++++++++++ read measure.\r\n");
                data_t* pData = getMySensor();
                uint32_t temperature_raw = 0;
                float temperature_s = 0.0;
                uint32_t humidity_raw = 0;
                // uint8_t humidity_s = 0;
                uint16_t humidity_s = 0;
                aht20_read_temperature_humidity(&temperature_raw, &temperature_s, 
                &humidity_raw, &humidity_s);
                int8_t temp = (int8_t)temperature_s;
                int8_t remain_temp = (int8_t)((temperature_s - temp) * 100);
                // Printf("temp: %d.%d\r\n", temp, remain_temp);
                // Printf("humidity: %d, %d\r\n", humidity_raw, humidity_s);
                Printf("aht20->temp: %d.%d, humidity: %d\r\n", temp, remain_temp, humidity_s );
                // aht20_read_temperature_humidity();
                // pData->temp = temperature_s * 100;
                pData->humidity = humidity_s;
            }
        }
    }
    aht20Count++;
}

