#include "Tile.h"
#include "../DisplayApp.h"
#include "Symbols.h"
#include "BatteryIcon.h"

using namespace Pinetime::Applications::Screens;

namespace {
  static void event_handler(lv_obj_t * obj, lv_event_t event) {
    
    Tile* screen = static_cast<Tile *>(obj->user_data);

    screen->OnObjectEvent(obj, event);

  }

}

static void lv_update_task(struct _lv_task_t *task) {  
  auto user_data = static_cast<Tile *>(task->user_data);
  user_data->UpdateScreen();
}

static std::array<std::array<lv_coord_t, 2>, 4> iconPos = {{{-55, -50}, {55, -50}, {-55, 60}, {55, 60}}};

Tile::Tile(uint8_t screenID, uint8_t numScreens,
    DisplayApp* app, 
    Controllers::DateTime& dateTimeController, 
    Controllers::Settings &settingsController,
    Pinetime::Controllers::Battery& batteryController,
    std::array<Applications, 4>& applications) : 
    Screen(app),
    dateTimeController{dateTimeController},
    settingsController{settingsController},
    batteryController{batteryController}
{

  // Set the background to Black
  lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0, 0, 0));

  settingsController.SetAppMenu(screenID);

  batteryPercent = batteryController.PercentRemaining();
  uint8_t hours = dateTimeController.Hours();
  uint8_t minutes = dateTimeController.Minutes();
  oldHours = hours;
  oldMinutes = minutes;

  // Time
  label_time = lv_label_create(lv_scr_act(), NULL);  
  lv_label_set_text_fmt(label_time,  "%02i:%02i", hours, minutes);      
  lv_label_set_align( label_time, LV_LABEL_ALIGN_CENTER );    
  lv_obj_align(label_time, lv_scr_act(), LV_ALIGN_IN_TOP_LEFT, 15, 4);

  batteryIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_font(batteryIcon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_20);
  lv_label_set_text(batteryIcon, BatteryIcon::GetBatteryIcon(batteryPercent));
  lv_obj_align(batteryIcon, NULL, LV_ALIGN_IN_TOP_RIGHT, -15, 4);

  if ( numScreens > 1 ) {
    pageIndicatorBasePoints[0].x = 240 - 3;
    pageIndicatorBasePoints[0].y = 6;
    pageIndicatorBasePoints[1].x = 240 - 3;
    pageIndicatorBasePoints[1].y = 240 - 6;
    
    pageIndicatorBase = lv_line_create(lv_scr_act(), NULL);
    lv_obj_set_style_local_line_width(pageIndicatorBase, LV_LINE_PART_MAIN, LV_STATE_DEFAULT, 6);
    lv_obj_set_style_local_line_color(pageIndicatorBase, LV_LINE_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x111111));
    lv_obj_set_style_local_line_rounded(pageIndicatorBase, LV_LINE_PART_MAIN, LV_STATE_DEFAULT, true);
    lv_line_set_points(pageIndicatorBase, pageIndicatorBasePoints, 2);

  
    uint16_t indicatorSize = 228 / numScreens;
    uint16_t indicatorPos = indicatorSize * screenID;

    pageIndicatorPoints[0].x = 240 - 3;
    pageIndicatorPoints[0].y = 6 + indicatorPos;
    pageIndicatorPoints[1].x = 240 - 3;
    pageIndicatorPoints[1].y = 6 + indicatorPos + indicatorSize;

    pageIndicator = lv_line_create(lv_scr_act(), NULL);
    lv_obj_set_style_local_line_width(pageIndicator, LV_LINE_PART_MAIN, LV_STATE_DEFAULT, 6);
    lv_obj_set_style_local_line_color(pageIndicator, LV_LINE_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_CYAN);
    lv_obj_set_style_local_line_rounded(pageIndicator, LV_LINE_PART_MAIN, LV_STATE_DEFAULT, true);
    lv_line_set_points(pageIndicator, pageIndicatorPoints, 2);
  }

  for(int i = 0; i < 4; i++) {
    apps[i] = applications[i].application;
    if ( applications[i].application != Apps::None) {
      iconsApps[i] = lv_imgbtn_create(lv_scr_act(), NULL);
      lv_imgbtn_set_src(iconsApps[i], LV_BTN_STATE_RELEASED, applications[i].icon);
      lv_imgbtn_set_src(iconsApps[i], LV_BTN_STATE_PRESSED, applications[i].icon);
      lv_obj_align(iconsApps[i], NULL, LV_ALIGN_CENTER, iconPos[i][0], iconPos[i][1]);
      iconsApps[i]->user_data = this;
      lv_obj_set_event_cb(iconsApps[i], event_handler);

      iconsAppsLabel[i] = lv_label_create(lv_scr_act(), NULL);
      lv_label_set_long_mode(iconsAppsLabel[i], LV_LABEL_LONG_SROLL);
      lv_obj_set_width(iconsAppsLabel[i], 105);      
      lv_label_set_text(iconsAppsLabel[i], applications[i].name);
      lv_label_set_align(iconsAppsLabel[i], LV_LABEL_ALIGN_CENTER );
      lv_obj_align(iconsAppsLabel[i], iconsApps[i], LV_ALIGN_IN_BOTTOM_MID, 0, 25);
    }
  }

  lv_obj_t * backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text_static(backgroundLabel, "");

  taskUpdate = lv_task_create(lv_update_task, 500000, LV_TASK_PRIO_MID, this);

}

Tile::~Tile() {
  lv_task_del(taskUpdate);
  lv_obj_clean(lv_scr_act());
}


void Tile::UpdateScreen() {

  hours = dateTimeController.Hours();
  minutes = dateTimeController.Minutes();
  
  if(oldHours != hours || oldMinutes != minutes) {
    lv_label_set_text_fmt(label_time,  "%02i:%02i", hours, minutes);
    oldHours = hours;
    oldMinutes = minutes;
  }

  batteryPercent = batteryController.PercentRemaining();
  lv_label_set_text(batteryIcon, BatteryIcon::GetBatteryIcon(batteryPercent));
}

bool Tile::Refresh() {

  return running;
}

void Tile::OnObjectEvent(lv_obj_t *obj, lv_event_t event) {
  if ( event == LV_EVENT_PRESSED ) {
    for(int i = 0; i < 4; i++) {
      if ( apps[i] != Apps::None && obj == iconsApps[i] ) {
        app->StartApp(apps[i], DisplayApp::FullRefreshDirections::Down);
        running = false;
        return;
      }
    }
  }
}


