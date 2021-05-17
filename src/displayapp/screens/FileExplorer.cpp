#include "FileExplorer.h"
#include <lvgl/lvgl.h>
#include "../DisplayApp.h"
#include <littlefs/lfs.h>

using namespace Pinetime::Applications::Screens;


FileExplorer::FileExplorer(
  Pinetime::Applications::DisplayApp *app, Controllers::FS &fs) : 
  Screen(app),
  fs{fs}
{

  app->SetTouchMode(DisplayApp::TouchModes::Polling);

  lv_obj_t * list1 = lv_list_create(lv_scr_act(), nullptr);
  lv_obj_set_size(list1, 240, 240);
  lv_obj_align(list1, nullptr, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t * list_btn;

  lfs_info entryInfo;

  lfs_dir_open(&fs.mLfs, &mLfs_dir, "/");
  while (lfs_dir_read(&fs.mLfs, &mLfs_dir, &entryInfo)) {
    list_btn = lv_list_add_btn(list1, nullptr, entryInfo.name);
  }
  lfs_dir_close(&fs.mLfs, &mLfs_dir);  


  lv_obj_t * backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text_static(backgroundLabel, "");

  lv_obj_t * info = lv_label_create(lv_scr_act(), NULL);                     /*Enable re-coloring by commands in the text*/
  lv_label_set_text_fmt(info, "boot %d", fs.boot_count); 
  lv_obj_set_style_local_text_color(info, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);
  lv_label_set_align(info, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(info, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

}

FileExplorer::~FileExplorer() {
  app->SetTouchMode(DisplayApp::TouchModes::Gestures);
  lv_obj_clean(lv_scr_act());
}

bool FileExplorer::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  return true;
}

bool FileExplorer::Refresh() {
  return running;
}