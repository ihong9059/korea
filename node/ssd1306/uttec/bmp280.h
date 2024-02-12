#ifndef __BMP280_H_
#define __BMP280_H_

#define BMP280_ADDR 0x77 

// hardware registers
#define REG_CONFIG 0xF5
#define REG_CTRL_MEAS 0xF4
#define REG_RESET 0xE0

#define REG_TEMP_XLSB 0xFC 
#define REG_TEMP_LSB 0xFB 
#define REG_TEMP_MSB 0xFA 

#define REG_PRESSURE_XLSB 0xF9 
#define REG_PRESSURE_LSB 0xF8 
#define REG_PRESSURE_MSB 0xF7 

// calibration registers
#define REG_DIG_T1_LSB 0x88 
#define REG_DIG_T1_MSB 0x89 
#define REG_DIG_T2_LSB 0x8A 
#define REG_DIG_T2_MSB 0x8B 
#define REG_DIG_T3_LSB 0x8C 
#define REG_DIG_T3_MSB 0x8D 
#define REG_DIG_P1_LSB 0x8E 
#define REG_DIG_P1_MSB 0x8F 
#define REG_DIG_P2_LSB 0x90 
#define REG_DIG_P2_MSB 0x91 
#define REG_DIG_P3_LSB 0x92 
#define REG_DIG_P3_MSB 0x93 
#define REG_DIG_P4_LSB 0x94 
#define REG_DIG_P4_MSB 0x95 
#define REG_DIG_P5_LSB 0x96 
#define REG_DIG_P5_MSB 0x97 
#define REG_DIG_P6_LSB 0x98 
#define REG_DIG_P6_MSB 0x99 
#define REG_DIG_P7_LSB 0x9A 
#define REG_DIG_P7_MSB 0x9B 
#define REG_DIG_P8_LSB 0x9C 
#define REG_DIG_P8_MSB 0x9D 
#define REG_DIG_P9_LSB 0x9E 
#define REG_DIG_P9_MSB 0x9F 

// number of calibration registers to be read
#define NUM_CALIB_PARAMS 24

typedef struct bmp280_calib_param {
    // temperature params
    uint16_t dig_t1;
    int16_t dig_t2;
    int16_t dig_t3;

    // pressure params
    uint16_t dig_p1;
    int16_t dig_p2;
    int16_t dig_p3;
    int16_t dig_p4;
    int16_t dig_p5;
    int16_t dig_p6;
    int16_t dig_p7;
    int16_t dig_p8;
    int16_t dig_p9;
} bmp280_calib_param_t;

void bmp280_init(void);
int32_t bmp280_convert_pressure(int32_t pressure, int32_t temp, bmp280_calib_param_t* params);
int32_t bmp280_convert_temp(int32_t temp, struct bmp280_calib_param* params);

void bmp280_get_calib_params(bmp280_calib_param_t* params);
void bmp280_read_raw(int32_t* temp, int32_t* pressure) ;
bmp280_calib_param_t* getBmp280Param(void);

void bmp280Flow(void);

#endif /* __BMP280_H_ */