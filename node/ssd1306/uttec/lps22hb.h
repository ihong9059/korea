#ifndef __LPS22HB_H_
#define __LPS22HB_H_

#define LPS22HB_ADDR 0x5C
// #define LPS22HB_ADDR 0x5d

#define LPS_PRESS_OUT_XL 0x28
#define LPS_PRESS_OUT_L 0x29
#define LPS_PRESS_OUT_H 0x2A

#define LPS_TEMP_OUT_L 0x2B
#define LPS_TEMP_OUT_H 0x2C

#define LPS_CTRL_REG1 0x10
#define LPS_CTRL_REG2 0x11
#define LPS_STATUS    0x27
#define who_am_i      0x0f

void initLPS22HB(void);
void setOneShotLPS22HB(void);
void readLPS22HB(void);

void lps22hbFlow(void);

#endif /* __LPS22HB_H_ */