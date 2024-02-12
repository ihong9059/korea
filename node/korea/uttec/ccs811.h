#ifndef __CCS811_H_
#define __CCS811_H_

#define CCS811_ADDR  0x5A

#define Who_CCS811  0x20
#define MEAS_MODE   0x01
#define CCS811_STATUS 0x00 

void initCCS811(void);
void shutdownCCS811(void);
void ccs811ReadValues(void);
void runCCS811(void);

void dispCcs811(void);

void ccs811Flow(void);

#endif /* __CCS811_H_ */