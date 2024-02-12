#ifndef __AHT20_H_
#define __AHT20_H_

#include "uttec.h"

#define CHIP_NAME                 "ASAIR AHT20"        /**< chip name */
#define MANUFACTURER_NAME         "ASAIR"              /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        "2.2"                 /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        "5.5"                 /**< chip max supply voltage */
#define MAX_CURRENT               "0.98"                /**< chip max current */
#define TEMPERATURE_MIN           "-40.0"               /**< chip min operating temperature */
#define TEMPERATURE_MAX           "85.0"                /**< chip max operating temperature */
#define DRIVER_VERSION            "1000"                 /**< driver version */

/**
 * @brief chip address definition
 */
#define AHT20_ADDRESS             0x38        /**< iic device address */


typedef struct aht20_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    // float supply_voltage_min_v;        /**< chip min supply voltage */
    // float supply_voltage_max_v;        /**< chip max supply voltage */
    // float max_current_ma;              /**< chip max current */
    // float temperature_min;             /**< chip min operating temperature */
    // float temperature_max;             /**< chip max operating temperature */
    // uint32_t driver_version;           /**< driver version */
} aht20_info_t;


uint8_t aht20_info(void);
aht20_info_t* getAht20Info(void);

uint8_t aht20_read_temperature_humidity(uint32_t *temperature_raw, float *temperature_s,
                                        uint32_t *humidity_raw, uint16_t *humidity_s);
uint8_t aht20_init(void);

void setAht20Trigger(void);

void aht20Flow(void);

#endif /* __AHT20_H_ */