#include "Weather.h"
#include <lvgl/lvgl.h>
#include "../DisplayApp.h"

using namespace Pinetime::Applications::Screens;


Weather::Weather(
  Pinetime::Applications::DisplayApp *app, Drivers::SpiNorFlash& spiNorFlash) : 
  Screen(app),
  spiNorFlash{spiNorFlash}
{
  // CF1F - 53023

  llabel = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_recolor(llabel, true);
  lv_label_set_text_fmt(llabel, "#0000FF Screen# for Weather\n%x", CalculateCrc()); 
  lv_label_set_align(llabel, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(llabel, NULL, LV_ALIGN_CENTER, 0, 0);


}


uint16_t Weather::CalculateCrc() {
  uint32_t chunkSize = 200;
  uint8_t tempBuffer[chunkSize];
  size_t currentOffset = 0;
  uint16_t crc = 0;

  size_t totalSize = 524685;
  static constexpr size_t writeOffset = 0x0B4000;

  bool first = true;
  while (currentOffset < totalSize) {
    uint32_t readSize = (totalSize - currentOffset) > chunkSize ? chunkSize : (totalSize - currentOffset);

    spiNorFlash.Read(writeOffset + currentOffset, tempBuffer, readSize);
    if (first) {
      crc = ComputeCrc(tempBuffer, readSize, NULL);
      first = false;
    } else
      crc = ComputeCrc(tempBuffer, readSize, &crc);
    currentOffset += readSize;
  }

  return crc;

}

uint16_t Weather::ComputeCrc(uint8_t const *p_data, uint32_t size, uint16_t const *p_crc) {
  uint16_t crc = (p_crc == NULL) ? 0xFFFF : *p_crc;

  for (uint32_t i = 0; i < size; i++) {
    crc = (crc >> 8) | (crc << 8);
    crc ^= p_data[i];
    crc ^= ((uint8_t) crc) >> 4;
    crc ^= crc << 12;
    crc ^= (crc & 0xFF) << 5;
  }

  return crc;
}

Weather::~Weather() {
  lv_obj_clean(lv_scr_act());
}

bool Weather::Refresh() {
  return running;
}