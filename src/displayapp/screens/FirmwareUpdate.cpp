#include "FirmwareUpdate.h"
#include <lvgl/lvgl.h>
#include "components/ble/BleController.h"
#include "../DisplayApp.h"

using namespace Pinetime::Applications::Screens;
//extern lv_font_t jetbrains_mono_extrabold_compressed;
//extern lv_font_t jetbrains_mono_bold_20;


FirmwareUpdate::FirmwareUpdate(Pinetime::Applications::DisplayApp *app, Pinetime::Controllers::Ble& bleController) :
      Screen(app), bleController{bleController} {

  titleLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_static(titleLabel, "Firmware update");
  lv_obj_set_auto_realign(titleLabel, true);
  lv_obj_align(titleLabel, nullptr, LV_ALIGN_IN_TOP_MID, 0, 50);

  bar1 = lv_bar_create(lv_scr_act(), nullptr);
  lv_obj_set_size(bar1, 200, 30);
  lv_obj_align(bar1, nullptr, LV_ALIGN_CENTER, 0, 0);
  lv_bar_set_anim_time(bar1, 10);
  lv_bar_set_range(bar1, 0, 100);
  lv_bar_set_value(bar1, 0, LV_ANIM_OFF);
  lv_obj_set_style_local_bg_color(bar1, LV_BAR_PART_INDIC , LV_STATE_DEFAULT, lv_color_hex(0x00FF00));

  percentLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_static(percentLabel, "");
  lv_obj_set_auto_realign(percentLabel, true);
  lv_obj_align(percentLabel, bar1, LV_ALIGN_OUT_TOP_MID, 0, 70);
}

FirmwareUpdate::~FirmwareUpdate() {
  lv_obj_clean(lv_scr_act());
}

bool FirmwareUpdate::Refresh() {
  switch(bleController.State()) {
    default:
    case Pinetime::Controllers::Ble::FirmwareUpdateStates::Idle:
    case Pinetime::Controllers::Ble::FirmwareUpdateStates::Running:
      if(state != States::Running)
        state = States::Running;
      return DisplayProgression();
    case Pinetime::Controllers::Ble::FirmwareUpdateStates::Validated:
      if(state != States::Validated) {
        UpdateValidated();
        state = States::Validated;
      }
      return running;
    case Pinetime::Controllers::Ble::FirmwareUpdateStates::Error:
      if(state != States::Error) {
        UpdateError();
        state = States::Error;
      }
      return running;
  }
}

bool FirmwareUpdate::DisplayProgression() const {
  float current = bleController.FirmwareUpdateCurrentBytes() / 1024.0f;
  float total = bleController.FirmwareUpdateTotalBytes() / 1024.0f;
  int16_t pc = (current / total) * 100.0f;

  lv_label_set_text_fmt(percentLabel, "%d %%", pc);

  lv_bar_set_value(bar1, pc, LV_ANIM_OFF);
  return running;
}

void FirmwareUpdate::UpdateValidated() {
  lv_label_set_recolor(percentLabel, true);
  lv_label_set_text_static(percentLabel, "#00ff00 Image Ok!#");
}

void FirmwareUpdate::UpdateError() {
  lv_label_set_recolor(percentLabel, true);
  lv_label_set_text_static(percentLabel, "#ff0000 Error!#");
  lv_obj_set_style_local_bg_color(bar1, LV_BAR_PART_INDIC , LV_STATE_DEFAULT, lv_color_hex(0xFF0000));
}
