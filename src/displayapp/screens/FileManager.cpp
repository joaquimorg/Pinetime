#include "FileManager.h"
#include "../DisplayApp.h"

#include "littlefs/lfs.h"

using namespace Pinetime::Applications::Screens;

static void event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
        //printf("Clicked: %s\n", lv_list_get_btn_text(obj));
    }
}


FileManager::FileManager(Pinetime::Applications::DisplayApp *app) : Screen(app) {
      
    lv_style_init(&style_halo);
    
    lv_obj_t * btn2 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_align(btn2, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(btn2, LV_BTN_PART_MAIN, &style_halo);
    lv_obj_set_style_local_value_str(btn2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "Pinetime");
}

FileManager::~FileManager() {
  lv_style_reset(&style_halo);
  lv_obj_clean(lv_scr_act());
}

bool FileManager::Refresh() {
  return running;
}

bool FileManager::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  return false;
}

bool FileManager::OnButtonPushed() {
  running = false;
  return true;
}
