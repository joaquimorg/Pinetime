#include <libs/lvgl/src/lv_core/lv_obj.h>
#include <libs/lvgl/src/lv_font/lv_font.h>
#include <libs/lvgl/lvgl.h>

#include "Tile.h"
#include "displayapp/DisplayApp.h"
#include "Symbols.h"
#include "../../Version.h"

using namespace Pinetime::Applications::Screens;

//extern lv_font_t jetbrains_mono_bold_20;

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  Tile* screen = static_cast<Tile *>(obj->user_data);
  uint32_t* eventDataPtr = (uint32_t*) lv_event_get_data();
  uint32_t eventData = *eventDataPtr;
  screen->OnObjectEvent(obj, event, eventData);
}

static std::array<std::array<lv_coord_t, 2>, 4> iconPos = {{{-55, -50}, {55, -50}, {-55, 60}, {55, 60}}};

Tile::Tile(DisplayApp* app, std::array<Applications, 4>& applications) : Screen(app) {
  /*for(int i = 0, appIndex = 0; i < 8; i++) {
    if(i == 3) btnm_map1[i] = "\n";
    else if(i == 7) btnm_map1[i] = "";
    else {
      btnm_map1[i] = applications[appIndex].icon;
      apps[appIndex] = applications[appIndex].application;
      appIndex++;
    }
  }
  //modal.reset(new Modal(app));

  btnm1 = lv_btnm_create(lv_scr_act(), nullptr);
  lv_btnm_set_map(btnm1, btnm_map1);
  lv_obj_set_size(btnm1, LV_HOR_RES, LV_VER_RES);

  btnm1->user_data = this;
  lv_obj_set_event_cb(btnm1, event_handler);*/

  for(int i = 0, appIndex = 0; i < 4; i++) {
    if ( applications[i].application == Apps::None)
      appIndex++;
    else {
      iconsApps[appIndex] = lv_imgbtn_create(lv_scr_act(), NULL);
      lv_imgbtn_set_src(iconsApps[appIndex], LV_BTN_STATE_REL, applications[i].icon);
      lv_imgbtn_set_src(iconsApps[appIndex], LV_BTN_STATE_PR, applications[i].icon);
      lv_obj_align(iconsApps[appIndex], NULL, LV_ALIGN_CENTER, iconPos[i][0], iconPos[i][1]);
      iconsApps[appIndex]->user_data = this;
      lv_obj_set_event_cb(iconsApps[appIndex], event_handler);

      apps[appIndex] = applications[i].application;
      appIndex++;
    }
  }

  backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  backgroundLabel->user_data = this;
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text(backgroundLabel, "");

}

Tile::~Tile() {
  lv_obj_clean(lv_scr_act());
}

bool Tile::Refresh() {
  return running;
}

void Tile::OnObjectEvent(lv_obj_t *obj, lv_event_t event, uint32_t buttonId) {
  if(event == LV_EVENT_VALUE_CHANGED) {
    app->StartApp(apps[buttonId]);
    running = false;
  }
}

bool Tile::OnButtonPushed() {
  app->StartApp(Apps::Clock);
  running = false;
  return true;
}


