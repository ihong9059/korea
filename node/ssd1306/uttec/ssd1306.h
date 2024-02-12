#ifndef __SSD1306_H__
#define __SSD1306_H__

#define SSD1306_ADDR 0x3C		//SSD1306 Slave Address
#define ssd1306_data_select 0x40	//SSD1306 Data 낅젰ъ슜
#define ssd1306_cmd_select 0x00		//SSD1306 Command 낅젰ъ슜

#define font_width 12	//ъ슜고듃臾몄옄 덈퉬
#define SSD1306_LINE 15

void initI2c(void);

void ssd1306_init(void);
void ssd1306_clear(void);
void ssd1306_fill(void);
void ssd1306_Set_Coord(uint8_t page, uint8_t col);
void ssd1306_W_Char(uint8_t character_Code, uint8_t page, uint16_t column);
void ssd1306_W_String(char *str, uint8_t page, uint8_t col);

void init_Ssd1306(void);
void dispCountDown(void);
void clearLine(uint8_t line);
void dispLcdLine(char* pChar, uint8_t line);

#endif
