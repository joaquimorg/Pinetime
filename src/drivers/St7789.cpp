#include "St7789.h"
#include <cassert>
#include <hal/nrf_gpio.h>
#include <libraries/delay/nrf_delay.h>
#include <nrfx_log.h>
#include "Spi.h"
#include "board_config.h"

// ST7789 specific commands used in init
#define ST7789_NOP			0x00
#define ST7789_SWRESET	0x01
#define ST7789_RDDID		0x04
#define ST7789_RDDST		0x09

#define ST7789_RDDPM		0x0A      // Read display power mode
#define ST7789_RDD_MADCTL	0x0B      // Read display MADCTL
#define ST7789_RDD_COLMOD	0x0C      // Read display pixel format
#define ST7789_RDDIM		0x0D      // Read display image mode
#define ST7789_RDDSM		0x0E      // Read display signal mode
#define ST7789_RDDSR		0x0F      // Read display self-diagnostic result (ST7789V)

#define ST7789_SLPIN		0x10
#define ST7789_SLPOUT		0x11
#define ST7789_PTLON		0x12
#define ST7789_NORON		0x13

#define ST7789_INVOFF		0x20
#define ST7789_INVON		0x21
#define ST7789_GAMSET		0x26      // Gamma set
#define ST7789_DISPOFF	0x28
#define ST7789_DISPON		0x29
#define ST7789_CASET		0x2A
#define ST7789_RASET		0x2B
#define ST7789_RAMWR		0x2C
#define ST7789_RGBSET		0x2D      // Color setting for 4096, 64K and 262K colors
#define ST7789_RAMRD		0x2E

#define ST7789_PTLAR		0x30
#define ST7789_VSCRDEF	0x33      // Vertical scrolling definition (ST7789V)
#define ST7789_TEOFF		0x34      // Tearing effect line off
#define ST7789_TEON			0x35      // Tearing effect line on
#define ST7789_MADCTL		0x36      // Memory data access control
#define ST7789_VSCSAD		0x37
#define ST7789_IDMOFF		0x38      // Idle mode off
#define ST7789_IDMON		0x39      // Idle mode on
#define ST7789_RAMWRC		0x3C      // Memory write continue (ST7789V)
#define ST7789_RAMRDC		0x3E      // Memory read continue (ST7789V)
#define ST7789_COLMOD		0x3A

#define ST7789_RAMCTRL	0xB0      // RAM control
#define ST7789_RGBCTRL	0xB1      // RGB control
#define ST7789_PORCTRL	0xB2      // Porch control
#define ST7789_FRCTRL1	0xB3      // Frame rate control
#define ST7789_INVCTRL	0xB4
#define ST7789_PARCTRL	0xB5      // Partial mode control
#define ST7789_GCTRL		0xB7      // Gate control
#define ST7789_GTADJ		0xB8      // Gate on timing adjustment
#define ST7789_DGMEN		0xBA      // Digital gamma enable
#define ST7789_VCOMS		0xBB      // VCOMS setting

#define ST7789_LCMCTRL	0xC0      // LCM control
#define ST7789_IDSET		0xC1      // ID setting
#define ST7789_VDVVRHEN	0xC2      // VDV and VRH command enable
#define ST7789_VRHS			0xC3      // VRH set
#define ST7789_VDVSET		0xC4      // VDV setting
#define ST7789_VCMOFSET	0xC5      // VCOMS offset set
#define ST7789_FRCTR2		0xC6      // FR Control 2
#define ST7789_CABCCTRL	0xC7      // CABC control
#define ST7789_REGSEL1	0xC8      // Register value section 1
#define ST7789_REGSEL2	0xCA      // Register value section 2
#define ST7789_PWMFRSEL	0xCC      // PWM frequency selection
#define ST7789_PWCTRL1	0xD0      // Power control 1
#define ST7789_VAPVANEN	0xD2      // Enable VAP/VAN signal output
#define ST7789_CMD2EN		0xDF      // Command 2 enable
#define ST7789_PVGAMCTRL	0xE0      // Positive voltage gamma control
#define ST7789_NVGAMCTRL	0xE1      // Negative voltage gamma control
#define ST7789_DGMLUTR		0xE2      // Digital gamma look-up table for red
#define ST7789_DGMLUTB		0xE3      // Digital gamma look-up table for blue
#define ST7789_GATECTRL		0xE4      // Gate control
#define ST7789_SPI2EN		0xE7      // SPI2 enable
#define ST7789_PWCTRL2	0xE8      // Power control 2
#define ST7789_EQCTRL		0xE9      // Equalize time control
#define ST7789_PROMCTRL	0xEC      // Program control
#define ST7789_PROMEN		0xFA      // Program mode enable
#define ST7789_NVMSET		0xFC      // NVM setting
#define ST7789_PROMACT	0xFE      // Program action


using namespace Pinetime::Drivers;

St7789::St7789(Spi &spi, uint8_t pinDataCommand) : spi{spi}, pinDataCommand{pinDataCommand} {

}


void St7789::Init() {
  spi.Init();
  nrf_gpio_cfg_output(pinDataCommand);
  nrf_gpio_cfg_output(LCD_RST);
  nrf_gpio_pin_set(LCD_RST);

  HardwareReset();

  SoftwareReset();
  
  SleepOut();
  ColMod();
  MemoryDataAccessControl();

  ColumnAddressSet();
  RowAddressSet();
  DisplayInversionOn();
  NormalModeOn();

  VerticalScrollDefinition(0, 320, 0);
  VerticalScrollStartAddress(0);

  DisplayOn();

}

void St7789::WriteCommand(uint8_t cmd) {
  nrf_gpio_pin_clear(pinDataCommand);
  WriteSpi(&cmd, 1);
}

void St7789::WriteData(uint8_t data) {
  nrf_gpio_pin_set(pinDataCommand);
  WriteSpi(&data, 1);
}


void St7789::WriteSpi(const uint8_t* data, size_t size) {
  spi.Write(data, size);
}

void St7789::SoftwareReset() {
  WriteCommand(ST7789_SWRESET);
  nrf_delay_ms(150);
}

void St7789::SleepOut() {
  WriteCommand(ST7789_SLPOUT);
  nrf_delay_ms(10);
}

void St7789::SleepIn() {
  WriteCommand(ST7789_SLPIN);
  nrf_delay_ms(10);
}

void St7789::ColMod() {
  WriteCommand(ST7789_COLMOD);
  WriteData(0x55);
  nrf_delay_ms(10);
}

void St7789::MemoryDataAccessControl() {
  WriteCommand(ST7789_MADCTL);
  WriteData(0x00);
}

void St7789::ColumnAddressSet() {
  WriteCommand(ST7789_CASET);
  WriteData(0x00);
  WriteData(0x00);
  WriteData(Width >> 8u);
  WriteData(Width & 0xffu);
}

void St7789::RowAddressSet() {
  WriteCommand(ST7789_RASET);
  WriteData(0x00);
  WriteData(0x00);
  WriteData(320u >> 8u);
  WriteData(320u & 0xffu);
}

void St7789::DisplayInversionOn() {
  WriteCommand(ST7789_INVON);
  nrf_delay_ms(10);
}

void St7789::NormalModeOn() {
  WriteCommand(ST7789_NORON);
  nrf_delay_ms(10);
}

void St7789::DisplayOn() {
  WriteCommand(ST7789_DISPON);
}

void St7789::SetAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
  WriteCommand(ST7789_CASET);
  WriteData(x0 >> 8);
  WriteData(x0 & 0xff);
  WriteData(x1 >> 8);
  WriteData(x1 & 0xff);

  WriteCommand(ST7789_RASET);
  WriteData(y0>>8);
  WriteData(y0 & 0xff);
  WriteData(y1 >> 8);
  WriteData(y1 & 0xff);

  WriteToRam();
}

void St7789::WriteToRam() {
  WriteCommand(ST7789_RAMWR);
}

void St7789::DisplayOff() {
  WriteCommand(ST7789_DISPOFF);
  nrf_delay_ms(500);
}

void St7789::VerticalScrollDefinition(uint16_t topFixedLines, uint16_t scrollLines, uint16_t bottomFixedLines) {
  WriteCommand(ST7789_VSCRDEF);
  WriteData(topFixedLines >> 8u);
  WriteData(topFixedLines & 0x00ffu);
  WriteData(scrollLines >> 8u);
  WriteData(scrollLines & 0x00ffu);
  WriteData(bottomFixedLines >> 8u);
  WriteData(bottomFixedLines & 0x00ffu);
}

void St7789::VerticalScrollStartAddress(uint16_t line) {
  //if ( isLocked ) return ;
  isLocked = true;
  verticalScrollingStartAddress = line;
  //assert(line < 320);
  WriteCommand(ST7789_VSCSAD);
  WriteData(line >> 8u);
  WriteData(line & 0x00ffu);
  isLocked = false;
}


void St7789::PartialDisplay(uint8_t mode) 
{
  WriteCommand(mode == 1 ? ST7789_PTLON : ST7789_NORON);
}

void St7789::setPartArea(uint16_t sr, uint16_t er) 
{
  WriteCommand(ST7789_PTLAR);
  WriteData(sr >> 8u);
  WriteData(sr & 0x00ffu);
  WriteData(er >> 8u);
  WriteData(er & 0x00ffu);
}

void St7789::Uninit() {

}

void St7789::DrawPixel(uint16_t x, uint16_t y, uint32_t color) {
  if((x < 0) ||(x >= Width) || (y < 0) || (y >= Height)) return;

  SetAddrWindow(x, y, x+1, y+1);

  nrf_gpio_pin_set(pinDataCommand);
  WriteSpi(reinterpret_cast<const uint8_t *>(&color), 2);
}

void St7789::BeginDrawBuffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {

  SetAddrWindow(0+x, ST7789_ROW_OFFSET+y, x+width-1, y+height-1);
  nrf_gpio_pin_set(pinDataCommand);
}


void St7789::DrawBuffer(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2, const uint8_t *data) {
  if ( isLocked ) return ;
  isLocked = true;
  size_t num_pix = (x2 - x1 + 1) * (y2 - y1 + 1);
  SetAddrWindow( x1, y1, x2, y2);  
  nrf_gpio_pin_set(pinDataCommand);
  WriteSpi(data, num_pix * 2);
  isLocked = false;
}

void St7789::NextDrawBuffer(const uint8_t *data, size_t size) {
  WriteSpi(data, size);
}

void St7789::HardwareReset() {
  nrf_gpio_pin_clear(LCD_RST);
  nrf_delay_ms(10);
  nrf_gpio_pin_set(LCD_RST);
}

void St7789::Sleep() {
  SleepIn();
  nrf_delay_ms(50);
  nrf_gpio_cfg_default(pinDataCommand);
  //NRF_LOG_INFO("[LCD] Sleep");
}

void St7789::Wakeup() {
  nrf_gpio_cfg_output(pinDataCommand);
  nrf_delay_ms(50);
  // TODO why do we need to reset the controller?
  //HardwareReset();
  //SoftwareReset();
  SleepOut();
  //ColMod();
  //MemoryDataAccessControl();
  //ColumnAddressSet();
  //RowAddressSet();
  //DisplayInversionOn();
  //NormalModeOn();
  VerticalScrollDefinition(0, 320, 0);
  VerticalScrollStartAddress(verticalScrollingStartAddress);
  DisplayOn();
  //NRF_LOG_INFO("[LCD] Wakeup")
}