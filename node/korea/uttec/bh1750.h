#ifndef __BH1750_H_
#define __BH1750_H_

#define BH1750_ADDR 0x23

#define POWER_DOWN  0x00 //# No active state
#define POWER_ON    0x01 //# Power on
#define RESET       0x07 //# Reset data register value
    // # Start measurement at 4lx resolution. Time typically 16ms.
#define CONTINUOUS_LOW_RES_MODE  0x13
    // # Start measurement at 1lx resolution. Time typically 120ms
#define CONTINUOUS_HIGH_RES_MODE_1  0x10
    // # Start measurement at 0.5lx resolution. Time typically 120ms
#define CONTINUOUS_HIGH_RES_MODE_2  0x11
    // # Start measurement at 1lx resolution. Time typically 120ms
    // # Device is automatically set to Power Down after measurement.
#define ONE_TIME_HIGH_RES_MODE_1  0x20
    // # Start measurement at 0.5lx resolution. Time typically 120ms
    // # Device is automatically set to Power Down after measurement.
#define ONE_TIME_HIGH_RES_MODE_2  0x21
    // # Start measurement at 1lx resolution. Time typically 120ms
    // # Device is automatically set to Power Down after measurement.
#define ONE_TIME_LOW_RES_MODE  0x23

#define RESOLUTION_MODE ONE_TIME_HIGH_RES_MODE_2
void bh1750Flow(void);

#endif /* __BH1750_H_ */