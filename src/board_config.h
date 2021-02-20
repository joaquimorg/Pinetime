#pragma once

#include <nrfx_saadc.h>


#define P8CLONE

#ifdef P8CLONE

#   define SPI_SCK 28
#   define SPI_MOSI 29
#   define SPI_MISO 

#   define SPIF_SCK 12
#   define SPIF_MOSI 13
#   define SPIF_MISO 16

#   define FLASH_CSN 15

#   define LCD_CSN 30
#   define LCD_DC 27
#   define LCD_RST 31

#   define LCD_LIGHT_1 11
#   define LCD_LIGHT_2 18
#   define LCD_LIGHT_3 16

#   define TWI0_SCL 20
#   define TWI0_SDA 19

#   define TWI_SCL 10
#   define TWI_SDA 9

#   define TP_TWI_ADDR 0x15
#   define TP_IRQ 26
#   define TP_RST 21

#   define HR_TWI_ADDR 0x44

#   define BMA421_TWI_ADDR 0x18
#   define BMA421_IRQ 17

#   define KEY_ACTION 25
#   define KEY_ENABLE 25

#   define PWR_CTRL 25             // 3V3 POWER CONTROL
#   define CHARGE_BASE_IRQ 3      // POWER PRESENCE INDICATION
#   define CHARGE_IRQ 4           // CHARGE INDICATION
#   define VIBRATOR_CTRL 25        // 

#   define BATTERY_VOL NRF_SAADC_INPUT_AIN0   // Analog

#else

#   define SPI_SCK 2
#   define SPI_MOSI 3
#   define SPI_MISO 4

#   define FLASH_CSN 5

#   define LCD_CSN 25
#   define LCD_DC 18
#   define LCD_RST 26

#   define LCD_LIGHT_1 14
#   define LCD_LIGHT_2 22
#   define LCD_LIGHT_3 23

#   define TWI_SCL 7
#   define TWI_SDA 6

#   define TP_TWI_ADDR 0x15
#   define TP_IRQ 28
#   define TP_RST 10

#   define HR_TWI_ADDR 0x44

#   define BMA421_TWI_ADDR 0x18
#   define BMA421_IRQ 8

#   define KEY_ACTION 13
#   define KEY_ENABLE 15

#   define PWR_CTRL 24             // 3V3 POWER CONTROL - NC
#   define CHARGE_BASE_IRQ 19      // POWER PRESENCE INDICATION
#   define CHARGE_IRQ 12           // CHARGE INDICATION

#   define VIBRATOR_CTRL 16        // 

#   define BATTERY_VOL NRF_SAADC_INPUT_AIN7   // 31 Analog

#endif