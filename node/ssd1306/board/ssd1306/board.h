/* Copyright 2017 Wirepas Ltd. All Rights Reserved.
 *
 * See file LICENSE.txt for full license details.
 *
 */

/**
 * @file
 *
 * Board definition for the
 * <a href="https://www.nordicsemi.com/Software-and-Tools/Development-Kits/nRF52-DK">Nordic semiconductor PCA10040 evaluation board</a>
 */
#ifndef BOARD_PCA10040_BOARD_H_
#define BOARD_PCA10040_BOARD_H_

// Interrupt pin for dual mcu app, unread indication
#define BOARD_UART_IRQ_PIN              12

// Serial port pins
#define BOARD_USART_TX_PIN              6
#define BOARD_USART_RX_PIN              8
#define BOARD_USART_CTS_PIN             7  /* For USE_USART_HW_FLOW_CONTROL */
#define BOARD_USART_RTS_PIN             5  /* For USE_USART_HW_FLOW_CONTROL */

#define USE_I2C0

#define BOARD_I2C_SDA_PIN   24
#define BOARD_I2C_SCL_PIN   22
// #define BOARD_I2C_SDA_PIN   7
// #define BOARD_I2C_SCL_PIN   8

// Pwm output for pwm_driver app
#define BOARD_PWM_OUTPUT_GPIO           28

// List of GPIO pins for the LEDs on the board: LED 1 to LED 4
// #define BOARD_LED_PIN_LIST           {22, 23, 19, 20} //for sink. 2023.05.12 24:00
// #define BOARD_LED_PIN_LIST           {18, 15, 20, 23} //for lamp demo sample. 2023.05.12 24:00
// #define BOARD_LED_PIN_LIST           {24, 15, 11, 13, 18, 23} //for bike old. 2023.05.28 10:30{POWER, LED_A, SOL+, SOL-, RED, BLUE}
#define BOARD_LED_PIN_LIST           {11, 13, 18, 23, 15} //for ssd1306 test. 2023.09.08 10:30{test1, test3, RED, BLUE, batteryPowerCtr}
// #define BOARD_LED_PIN_LIST           {13, 11, 22, 23} //for diaper and demo kit. 2023.05.31 15:00 {buzzer, LED3, OP_LED, LED}

// Active low polarity for LEDs
#define BOARD_LED_ACTIVE_LOW            true

// List of GPIO pins for buttons on the board: Button 1 to Button 4
// #define BOARD_BUTTON_PIN_LIST           {13, 14, 15, 16}
// #define BOARD_BUTTON_PIN_LIST           {15} //for sink only. 2023.05.22 23:00
// #define BOARD_BUTTON_PIN_LIST           {2, 14, 15, 16} //for lamp demo. 2023.05.12 24:00
#define BOARD_BUTTON_PIN_LIST           {17} //for bike old. 2023.05.28 10:30
// #define BOARD_BUTTON_PIN_LIST           {2} //for diaper and demo kit. 2023.05.31 15:00

// Active low polarity for buttons
#define BOARD_BUTTON_ACTIVE_LOW         true

// Active internal pull-up for buttons
#define BOARD_BUTTON_INTERNAL_PULL      true

#define USE_SPI0

#define BOARD_SPI_SCK_PIN   24
#define BOARD_SPI_MOSI_PIN   13
#define BOARD_SPI_MISO_PIN   15
#define BOARD_SPI_CS_PIN   11

#define MAX_READ_SIZE 32
#define MAX_WRITE_SIZE MAX_READ_SIZE


// The board supports DCDC (#define BOARD_SUPPORT_DCDC)
// Since SDK v1.2 (bootloader > v7) this option has been move to
// board/<board_name>/config.mk. Set board_hw_dcdc to yes to enable DCDC.
#ifdef BOARD_SUPPORT_DCDC
#error This option has been moved to board/<board_name>/config.mk
#endif


#endif /* BOARD_PCA10040_BOARD_H_ */
