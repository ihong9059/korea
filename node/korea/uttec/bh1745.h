#ifndef __BH1745_H_
#define __BH1745_H_

// #define BH1745_ADDR 0x38
#define BH1745_ADDR 0x39 //address change

void setBH1745(void);
void readBH1745(void);

void bh1745Flow(void);

#endif /* __BH1745_H_ */