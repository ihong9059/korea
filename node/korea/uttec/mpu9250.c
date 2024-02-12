#include <math.h>
#include "uttec.h"
#include "twi.h"
#include "lps22hb.h"
#include "myJob.h"

#include "mpu9250.h"

// uint8_t readReg(uint8_t reg){
// 	// i2c_write_read(i2c_dev, MPU9250, &writeBuf, 1, &readBuf, 1);
// 	uint8_t value = 0;
// 	int err = i2c_write_read(i2c_dev, MPU9250, &reg, 1, &value, 1);
// 	if(err < 0){
// 		Printf("read reg: %d, error: %d\n", reg, err);
// 	}		
// 	return value;
// }

void writeWord(uint8_t reg, uint16_t data){
	uint8_t bHigh = (data & 0xff00) >> 8;
	uint8_t bLow = (data & 0x00ff);
	writeReg(MPU9250, reg, bHigh);
	writeReg(MPU9250, reg+1, bLow);
	// i2c_reg_write_byte(i2c_dev, MPU9250, reg, bHigh);
	// i2c_reg_write_byte(i2c_dev, MPU9250, reg+1, bLow);
}

uint16_t uRead2Reg(uint8_t reg){
	uint8_t value[2];
	readReg(MPU9250, reg, value, 2);
	// int err = i2c_write_read(i2c_dev, MPU9250, &reg, 1, &value, 2);
	// if(err < 0){
	// 	Printf("read reg: %d, error: %d\n", reg, err);
	// }	

	return value[0]*256 + value[1];
}

int16_t read2Reg(uint8_t reg){
	uint8_t value[2];
	readReg(MPU9250, reg, value, 2);
	// int err = i2c_write_read(i2c_dev, MPU9250, &reg, 1, &value, 2);
	// if(err < 0){
	// 	Printf("read reg: %d, error: %d\n", reg, err);
	// }	

	return value[0]*256 + value[1];
}

mpu9250_t myMpu = {0, };

mpu9250_t* getMpuData(void){
	return &myMpu;
}

void readTemp(void){
	float temp = (float)read2Reg(0x41)/(float)SCAL_TEMP;
	myMpu.temp = (temp - ((float)OFFSET_TEMP/(float)SCAL_TEMP) + 21);
}

void readGyro(void){
	myMpu.gyro[0] = (float)read2Reg(0x43)/(float)SCAL_GYRO;
	myMpu.gyro[1] = (float)read2Reg(0x45)/(float)SCAL_GYRO;
	myMpu.gyro[2] = (float)read2Reg(0x47)/(float)SCAL_GYRO;	
}

#include "myJob.h"

void readAcc(void){
	data_t* pData = getMySensor();

	float pi = 3.14159265358979323846;
	Printf("------ pi: %d --------------\r\n", (int)pi);
	myMpu.acc[0] = (float)read2Reg(0x3b)/(float)SCAL_ACC;
	myMpu.acc[1] = (float)read2Reg(0x3d)/(float)SCAL_ACC;
	myMpu.acc[2] = (float)read2Reg(0x3f)/(float)SCAL_ACC;
	float roll = atan2(-myMpu.acc[0], 
		sqrt(myMpu.acc[2] * myMpu.acc[2] + myMpu.acc[1] * myMpu.acc[1]));
	float pitch = atan2(-myMpu.acc[1], 
		sqrt(myMpu.acc[0] * myMpu.acc[0] + myMpu.acc[2] * myMpu.acc[2]));
	// float pitch = atan2(myMpu.acc[2], sin(myMpu.acc[1]) * 
	// 	sqrt((0.01 * myMpu.acc[0] * myMpu.acc[0] + myMpu.acc[1] * myMpu.acc[1])));
	myMpu.roll = (int)((roll * (float)180 / pi) * 100);
	myMpu.pitch = (int)((pitch * (float)180 / pi) * 100);;

	// Printf("high: %x, low: %x\r\n", acc_x_h, acc_x_l);
	// uint16_t uAcc_x = (uint16_t)read2Reg(0x3b);
	// Printf("acc_x: %d, %x\r\n", uAcc_x, uAcc_x);
	// Printf("acc_x: %f\r\n", myMpu.acc[0]);
	// Printf("acc_y: %f\r\n", myMpu.acc[1]);
	// Printf("acc_z: %f\r\n", myMpu.acc[2]);
	pData->humidity = myMpu.pitch;
	pData->pressure = myMpu.roll;
	
	Printf("pitch: %d\r\n", (int)myMpu.pitch);
	Printf("roll: %d\r\n", (int)myMpu.roll);
}

void calAcc(void){
	int16_t acc_x_bias = read2Reg(0x77) >> 1;
	int16_t acc_y_bias = read2Reg(0x7a) >> 1;
	int16_t acc_z_bias = read2Reg(0x7d) >> 1;

	int16_t acc_x = read2Reg(0x3b)/16;
	int16_t acc_y = -(16384 - read2Reg(0x3d))/16;
	int16_t acc_z = read2Reg(0x3f)/16;

	int16_t acc_x_new_bias = (acc_x_bias - acc_x) << 1;
	int16_t acc_y_new_bias = (acc_y_bias - acc_y) << 1;
	int16_t acc_z_new_bias = (acc_z_bias - acc_z) << 1;

	writeWord(0x77, (uint16_t)acc_x_new_bias);
	writeWord(0x7a, (uint16_t)acc_y_new_bias);
	writeWord(0x7d, (uint16_t)acc_z_new_bias);
}

void resetMpu(void){
	Printf("-------- resetMpu ------------\r\n");
	writeReg(MPU9250, pmgt_1, 0x80);
	// i2c_reg_write_byte(i2c_dev, MPU9250, pmgt_1, 0x80);
}

void readMpu(void){
	readAcc();
	readGyro();
	readTemp();
}

void dispMpu(void){
	// readMpu();
	Printf("------------ temp ------------\r\n");
	Printf("temp: %0.4f\r\n", myMpu.temp);
	Printf("------------ acc ------------\r\n");
	Printf("x: %0.4f, y: %0.4f, z: %0.4f\r\n", myMpu.acc[0], myMpu.acc[1], myMpu.acc[2]);
	Printf("------------ gyro ------------\r\n");
	Printf("x: %0.4f, y: %0.4f, z: %0.4f\r\n", myMpu.gyro[0], myMpu.gyro[1], myMpu.gyro[2]);
	Printf("------------ end ------------\r\n");
}

void initMpu9250(void){
	// if(!device_is_ready(i2c_dev)){
	// 	Printf("i2c_dev not ready\r\n");
	// 	return;
	// }
	Printf("pass mpu9250 i2c initial\r\n");    

	Printf("---------- before reset ----------\r\n");
	readAcc();
	// k_msleep(1000);
	Printf("---------- after reset ----------\r\n");
	resetMpu();
	// calAcc();
	readAcc();
	// k_msleep(1000);

}

#define MPU_PRIORITY     2
#define STACKSIZE 1024

// void mpu_test(void){
// 	uint32_t count = 0;
// 	char uartData[200];

// 	while(true){
// 		readMpu();
// 		mpu9250_t* pMpu = getMpuData();

// 		sprintf(uartData, "%d, %0.2f, %0.2f, %0.2f, %0.2f, %0.2f, %0.2f, %0.2f, %0.2f, %0.2f\r\n", 
// 			1, pMpu->acc[0], pMpu->acc[1], pMpu->acc[2],
// 			pMpu->gyro[0], pMpu->gyro[1], pMpu->gyro[2],
// 			pMpu->pitch, pMpu->roll, pMpu->temp);
// 		// sprintf(uartData, "pitch: %0.2f, roll: %0.2f\r\n", pMpu->pitch, pMpu->roll);

// 		print_all_uart(uartData);

// 		Printf("mpu_test: %d\r\n", count);
// 		count++;
// 		k_msleep(1000);
// 	}
// }
// K_THREAD_DEFINE(mpu_test_id, STACKSIZE, mpu_test, NULL, NULL, NULL,
// 		MPU_PRIORITY, 0, 0);


