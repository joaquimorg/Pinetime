#include "About.h"
#include <lvgl/lvgl.h>
#include "../DisplayApp.h"
#include "Label.h"
#include "Version.h"
#include "components/battery/BatteryController.h"
#include "components/ble/BleController.h"
#include "components/brightness/BrightnessController.h"
#include "components/datetime/DateTimeController.h"
#include "drivers/Watchdog.h"
#include "drivers/BMA421.h"
#include "displayapp/Apps.h"

using namespace Pinetime::Applications::Screens;

About::About(Pinetime::Applications::DisplayApp *app,
                       Pinetime::Controllers::DateTime &dateTimeController,
                       Pinetime::Controllers::Battery& batteryController,
                       Pinetime::Controllers::BrightnessController& brightnessController,
                       Pinetime::Controllers::Ble& bleController,
                       Pinetime::Drivers::WatchdogView& watchdog,
                       Pinetime::Drivers::BMA421& stepCounter) :
        Screen(app),
        dateTimeController{dateTimeController}, batteryController{batteryController},
        brightnessController{brightnessController}, bleController{bleController}, watchdog{watchdog},
        stepCounter{stepCounter},
        screens{app, 
          0,
          {
                [this]() -> std::unique_ptr<Screen> { return CreateScreen1(); },
                [this]() -> std::unique_ptr<Screen> { return CreateScreen2(); },
                [this]() -> std::unique_ptr<Screen> { return CreateScreen3(); },
                [this]() -> std::unique_ptr<Screen> { return CreateScreen4(); }
          },
          Screens::ScreenListModes::UpDown
        } {}


About::~About() {
  lv_obj_clean(lv_scr_act());
}

bool About::Refresh() {
  if(running)
    running = screens.Refresh();
  return running;
}

bool About::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  return screens.OnTouchEvent(event);
}

void About::CreateContainer() {

  if ( container1 ) {
    container1 = lv_cont_create(lv_scr_act(), nullptr);

    //lv_obj_set_style_local_bg_color(container1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x111111));
    lv_obj_set_style_local_bg_opa(container1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_obj_set_style_local_pad_all(container1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 10);
    lv_obj_set_style_local_pad_inner(container1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 5);
    lv_obj_set_style_local_border_width(container1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);

    lv_obj_set_pos(container1, 0, 0);
    lv_obj_set_width(container1, LV_HOR_RES - 10);
    lv_obj_set_height(container1, LV_VER_RES);
    //lv_obj_set_auto_realign(container1, true);                  
    //lv_obj_align_origo(container1, NULL, LV_ALIGN_CENTER, 0, 0);
    //lv_cont_set_fit(container1, LV_FIT_MAX);
    lv_cont_set_layout(container1, LV_LAYOUT_CENTER);
  }
}


std::unique_ptr<Screen> About::CreateScreen1() {
  CreateContainer();

  lv_obj_t * label = lv_label_create(container1, nullptr);
  lv_label_set_recolor(label, true);
  lv_label_set_text_fmt(label, 
              "#FFFF00 Pinetime Lite#\n\n"
              "#444444 Version# %ld.%ld.%ld\n\n"
              "#444444 Build date#\n"
              "\t%s\n"
              "\t%s\n",
          Version::Major(), Version::Minor(), Version::Patch(),
          __DATE__, __TIME__);
  lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
  return std::unique_ptr<Screen>(new Screens::Label(0, 4, app, label));
}

std::unique_ptr<Screen> About::CreateScreen2() {

  CreateContainer();

  auto batteryPercent = static_cast<int8_t>(batteryController.PercentRemaining());
  float batteryVoltage = batteryController.Voltage();

  // uptime
  static constexpr uint32_t secondsInADay = 60*60*24;
  static constexpr uint32_t secondsInAnHour = 60*60;
  static constexpr uint32_t secondsInAMinute = 60;
  uint32_t uptimeSeconds = dateTimeController.Uptime().count();
  uint32_t uptimeDays = (uptimeSeconds / secondsInADay);
  uptimeSeconds = uptimeSeconds % secondsInADay;
  uint32_t uptimeHours = uptimeSeconds / secondsInAnHour;
  uptimeSeconds = uptimeSeconds % secondsInAnHour;
  uint32_t uptimeMinutes = uptimeSeconds / secondsInAMinute;
  uptimeSeconds = uptimeSeconds % secondsInAMinute;
  // TODO handle more than 100 days of uptime

  if (batteryPercent == -1) batteryPercent = 0;

  lv_obj_t * label = lv_label_create(container1, nullptr);
  lv_label_set_recolor(label, true);
  lv_label_set_text_fmt(label, 
              "#444444 Date#\t%02d/%02d/%04d\n"
              "#444444 Time#\t%02d:%02d:%02d\n"
              "#444444 Uptime#\t%02lud %02lu:%02lu:%02lu\n"
              "#444444 Battery#\t%d%% / %.2f v\n"
              "#444444 Backlight#\t%s\n"
              "#444444 Last reset#\t%s\n",
          dateTimeController.Day(), static_cast<uint8_t>(dateTimeController.Month()), dateTimeController.Year(),
          dateTimeController.Hours(), dateTimeController.Minutes(), dateTimeController.Seconds(),
          uptimeDays, uptimeHours, uptimeMinutes, uptimeSeconds,
          batteryPercent, batteryVoltage, brightnessController.ToString(), watchdog.ResetReasonToString(watchdog.ResetReason()));
  return std::unique_ptr<Screen>(new Screens::Label(1, 4, app, label));
}

std::unique_ptr<Screen> About::CreateScreen3() {
  lv_mem_monitor_t mon;
  lv_mem_monitor(&mon);
  stepCounter.Update();
  CreateContainer();

  lv_obj_t * label = lv_label_create(container1, nullptr);
  lv_label_set_recolor(label, true);
  auto& bleAddr = bleController.Address();
  lv_label_set_text_fmt(label, 
              "#444444 BLE MAC# \n"
              "\t%02x:%02x:%02x:%02x:%02x:%02x"
              "\n"
              "#444444 Memory#\n"
              "\t#444444 used# %6d (%3d %%)\n"
              "\t#444444 frag# %3d %%\n"
              "\t#444444 biggest free# %6d"
              "\n"
              "#444444 Steps#\t%li\n"
              "#444444 Temp#\t%.2f\n",
          bleAddr[5], bleAddr[4], bleAddr[3], bleAddr[2], bleAddr[1], bleAddr[0],
          (int)mon.total_size - mon.free_size,
          mon.used_pct,
          mon.frag_pct,
          (int)mon.free_biggest_size,
          stepCounter.GetSteps(),
          stepCounter.GetTemp()
          );

  return std::unique_ptr<Screen>(new Screens::Label(2, 4, app, label));
}

std::unique_ptr<Screen> About::CreateScreen4() {
  CreateContainer();
  lv_obj_t * label = lv_label_create(container1, nullptr);
  lv_label_set_recolor(label, true);
  lv_label_set_text_static(label,
              "Software Licensed\n"
              "under the terms of\n"
              "the GNU General\n"
              "Public License v3\n"
              "#444444 Source code#\n"
              "#FFFF00 https://github.com/#\n"
              "#FFFF00 joaquimorg/PinetimeLite#");
  lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
  return std::unique_ptr<Screen>(new Screens::Label(3, 4, app, label));
}
