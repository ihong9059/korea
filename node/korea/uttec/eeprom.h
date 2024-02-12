#ifndef __EEPROM_H_
#define __EEPROM_H_

#define EEPROM 0x50
#define EEPROM_SIM_ADDR EEPROM
#define TEST_SIZE   100

void writeEeprom(void);
void readEeprom(void);


#endif /* __EEPROM_H_ */