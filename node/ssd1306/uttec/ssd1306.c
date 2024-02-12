#include <stdio.h>

#include "uttec.h"
#include "led.h"

#include "shared_data.h"
#include "shared_appconfig.h"
#include "wirepas.h"

#include "button.h"
// #include "bike.h"
#include "i2c.h"

#define DEBUG_LOG_MODULE_NAME "DBUG APP"
/* The maximum log level printed for this module. */
#define DEBUG_LOG_MAX_LEVEL LVL_INFO

#include "debug_log.h"

#include "ssd1306.h"
#include "ssd1309_fonts.h"

// static i2c_conf_t m_i2c_conf = {
//     .clock = 100000,
//     // .clock = 150000,
//     // .pullup = NRF_GPIO_PIN_NOPULL,
//     .pullup = NRF_GPIO_PIN_PULLUP,
// };

// void initI2c(void)
// {
//     i2c_res_e res;
//     res = I2C_init(&m_i2c_conf);

//     if (res == I2C_RES_OK || res == I2C_RES_ALREADY_INITIALIZED){
//         Printf("i2c ok\r\n");
//         Printf("sda: %d, scl: %d\r\n", BOARD_I2C_SDA_PIN, BOARD_I2C_SCL_PIN);
//     }
//     else{
//         Printf("Fail I2C at initI2c\r\n");
//     }
// }

// void writeReg(uint8_t addr, uint8_t reg, uint8_t data){
//     i2c_res_e res;
//     uint8_t w_data[2] = {reg, data}; //RGBC measurement time = 160 ms
//     i2c_xfer_t xfer_tx = {
//     .address = addr,
//     .write_ptr = w_data,
//     .write_size = 2,
//     .read_ptr = NULL,
//     .read_size = 0};

//     res = I2C_transfer(&xfer_tx, NULL);
//     if(res != I2C_RES_OK){
//         Printf("--I2C Error @writeReg addr: %x, reg: %x\r\n", addr, reg);
//     }
// }

void ssd1306_write_cmd(uint8_t ssd1306_cmd)
{
    i2c_res_e res;
    uint8_t w_data[2] = {ssd1306_cmd_select, ssd1306_cmd}; //RGBC measurement time = 160 ms
    i2c_xfer_t xfer_tx = {
    .address = SSD1306_ADDR,
    .write_ptr = w_data,
    .write_size = 2,
    .read_ptr = NULL,
    .read_size = 0};

    res = I2C_transfer(&xfer_tx, NULL);
    if(res != I2C_RES_OK){
        Printf("--I2C Error @writeReg addr: %x, reg: %x\r\n", SSD1306_ADDR, ssd1306_cmd_select);
    }
}

void ssd1306_write_data(uint8_t* data_buffer, int buf_length)
{
    static uint8_t clear_buffer[128]={0xff};	//GDDRAM占쎌넚占쎌씠踰꾪띁
    for(int i = 0; i < 128; i++){
      clear_buffer[i] = 0;
    }

    uint8_t* pTemp = clear_buffer;
    uint8_t* pDest;
    pDest = pTemp;
    
    *pTemp++ = ssd1306_data_select;
    for(int i = 0; i < buf_length; i++){
        *pTemp++ = *data_buffer++;
    }

    i2c_res_e res;
    i2c_xfer_t xfer_tx = {
    .address = SSD1306_ADDR,
    .write_ptr = pDest,
    .write_size = buf_length + 1,
    .read_ptr = NULL,
    .read_size = 0};

    res = I2C_transfer(&xfer_tx, NULL);
    if(res != I2C_RES_OK){
        Printf("--I2C Error @writeReg addr: %x, reg: %x\r\n", SSD1306_ADDR, ssd1306_data_select);
    }
}

void ssd1306_init(void)
{
	ssd1306_write_cmd(0xA8);	//Set Mux Ratio
	ssd1306_write_cmd(0x3F);	//64MUX

	ssd1306_write_cmd(0xD3);	//Set Display Offset
	ssd1306_write_cmd(0x00);	//COM0

	ssd1306_write_cmd(0x40);	//Set Display Start Line

	ssd1306_write_cmd(0xA1);	//Set Segment re-map, Default 0xA0
					//column address 127 is mapped to SEG0 (醫뚯슦 諛섏쟾)

	ssd1306_write_cmd(0xC8);	//Set COM Output Scan Direction, default 0xC0
					//remapped mode. Scan from COM[N-1] to COM0 (占쏀븯 諛섏쟾)

	ssd1306_write_cmd(0xDA);	//Set COM Pins hardware configuration
	ssd1306_write_cmd(0x12);

	ssd1306_write_cmd(0x20);	//Set Memory Addressing Mode
	ssd1306_write_cmd(0x02);	//Page Addressing Mode

	ssd1306_write_cmd(0x81);	//Set Contrast Control
	ssd1306_write_cmd(0x7F);	//1~256

	ssd1306_write_cmd(0xA4);	//Disable Entire Display On

	ssd1306_write_cmd(0xA6);	//Set Normal Display

	ssd1306_write_cmd(0xD5);	//Set Osc Frequency
	ssd1306_write_cmd(0x80);

	ssd1306_write_cmd(0x8D);	//Enable charge pump regulator
	ssd1306_write_cmd(0x14);

	ssd1306_write_cmd(0xAF);	//Display ON
}

void ssd1306_clear(void)
{
    static uint8_t clear_buffer[128]={0xff};	//GDDRAM占쎌넚占쎌씠踰꾪띁
    for(int i = 0; i < 128; i++){
      clear_buffer[i] = 0;
    }
 /*
   */
    //占쎌씠占쎈젰占쎌옉二쇱냼占쎈쾲占쏙옙濡占쎌젙
    ssd1306_write_cmd(0x00);
    ssd1306_write_cmd(0x10);

    for(int i=0;i<8;i++)
    {
        ssd1306_write_cmd(0xB0+i);	//占쎌씠占쏙옙 占쎈젰占쎌씠吏 占쎌젙
        ssd1306_write_data(clear_buffer,sizeof(clear_buffer)/sizeof(uint8_t));	//占쎌씠踰꾪띁 占쎌넚
    }
}


void ssd1306_fill(void)
{
    static uint8_t fill_buffer[128];

    for(int i=0;i<128;i++)
    {
        fill_buffer[i]=0xff;
    }

    ssd1306_write_cmd(0x00);
    ssd1306_write_cmd(0x10);

    for(int i=0;i<8;i++)
    {
        ssd1306_write_cmd(0xB0+i);
        ssd1306_write_data(fill_buffer,sizeof(fill_buffer)/sizeof(uint8_t));
    }
}

void ssd1306_Set_Coord(uint8_t page, uint8_t col)
{
    uint8_t col_low=0x0F,col_high=0x1F;
    col_low=(col&0x0F);
    col_high=0x10|((col>>4)&0x0F);
    ssd1306_write_cmd(0xB0+page);
    ssd1306_write_cmd(col_low);
    ssd1306_write_cmd(col_high);
}

void ssd1306_W_Char(uint8_t character_Code, uint8_t page, uint16_t column)
{
    uint8_t char_Buffer[font_width*2]={0};
    for(uint8_t i=0;i<font_width*2;i++)
    {
        char_Buffer[i]=uttec_fonts[(character_Code-32)*(font_width*2)+i];
    }

    for(uint8_t i=0;i<2;i++)
    {
        ssd1306_Set_Coord(page+i,column);
        ssd1306_write_data(&char_Buffer[i*font_width],font_width);
    }
}

void ssd1306_W_String(char *str, uint8_t page, uint8_t col)
{
    while(*str)
    {
        if((127<col+font_width))
        {
            if(page==6)
            {
                    break;
            }
            page+=2;
            col=0;
        }
        ssd1306_W_Char(*str,page,col);

        col+=font_width;
        str++;
    }
}

void dispLcdLine(char* pChar, uint8_t line){
    ssd1306_W_String(pChar, line * 2, 0);
}

#include <string.h>
#include "twi.h"

void init_Ssd1306(void){
    device_t* pData = getDeviceFlag();
    flash_t *pFlash = getFlash();
    char line[15] = {0,};
    if(!pData->lcdFlag) return;

    ssd1306_init();
    Printf("ssd1306_init\r\n");
    ssd1306_clear();
    Printf("endof ssd1306_init\r\n");

    sprintf(line, "C:%02d, A:%02d", pFlash->company, pFlash->areaId);
    dispLcdLine(line, 0);

    if(!pData->deviceFlag){
        dispLcdLine("2023.09.10", 1);
    }
    else{
        dispLcdLine(pData->pDeviceName, 1);
    }

    clearLine(2);
    clearLine(3);
}

void clearLine(uint8_t line){
    char line3[SSD1306_LINE];
    for(int i = 0; i < SSD1306_LINE-1; i++) line3[i] = ' ';
    ssd1306_W_String(line3, line*2, 0);
}

#include "myJob.h"

void dispCountDown(void){
    data_t* pData = getMySensor();
    static uint32_t ulCount = 0;
    char line[SSD1306_LINE];
    ulCount++;
    // uint8_t uiHour = (uint8_t)(ulCount/3600);
    // uint8_t ucMinute = (uint8_t)(ulCount/60)%60;
    // uint8_t ucSec = (uint8_t)(ulCount%60);

    sprintf(line,"-> %ld", ulCount);
    // ssd1306_W_String(line3,4,0);
    dispLcdLine(line, 2);
    int16_t bat = pData->battery;
    uint16_t remain = bat % 100;
    bat /= 100;
    sprintf(line,"Bat:%d.%02d", bat, remain);
    // ssd1306_W_String(line3,6,0);
    dispLcdLine(line, 3);
}

