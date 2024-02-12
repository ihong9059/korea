#ifndef MPU9250__H__
#define MPU9250__H__

#define MPU9250	(0x68)

#define SCAL_GYRO 	131
#define SCAL_ACC	16384
#define SCAL_MAG	0.6 
#define SCAL_TEMP	321 
#define OFFSET_TEMP 800 

#define pmgt_1	0x6b
#define pmgt_2  0x6c

typedef struct{
    float acc[3];
    float gyro[3];
    float temp;
    int pitch;
    int roll;
} mpu9250_t;

void initMpu9250(void);

void resetMpu(void);
void calAcc(void);

void readMpu(void);
void dispMpu(void);
void readAcc(void);

mpu9250_t* getMpuData(void);

#endif 


