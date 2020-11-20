#include <libs/lvgl/lvgl.h>
#include "WatchFaceDigital.h"

using namespace Pinetime::Applications::Screens;

WatchFaceDigital::WatchFaceDigital(Pinetime::Applications::DisplayApp *app,
                  Controllers::DateTime& dateTimeController,
                  Controllers::Battery& batteryController,
                  Controllers::Ble& bleController,
                  Controllers::NotificationManager& notificatioManager)  : Screen(app) {
  label = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_align(label, LV_LABEL_ALIGN_LEFT);
  lv_obj_set_size(label, 240, 240);
  lv_label_set_text(label, "demo");
}

WatchFaceDigital::~WatchFaceDigital() {
  lv_obj_clean(lv_scr_act());
}
