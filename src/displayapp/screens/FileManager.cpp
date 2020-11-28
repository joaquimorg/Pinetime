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
  
  /*uint32_t count = fs.read();

  lv_obj_t * label2 = lv_label_create(lv_scr_act(), NULL);        
  
  lv_label_set_text_fmt(label2,  "%02i", count);
  lv_obj_align(label2, NULL, LV_ALIGN_CENTER, 0, -90);*/

  /*Create a list*/
  lv_obj_t * list1 = lv_list_create(lv_scr_act(), NULL);
  lv_obj_set_size(list1, 160, 200);
  lv_obj_align(list1, NULL, LV_ALIGN_CENTER, 0, 0); 


  lv_obj_t * list_btn;
  int size = 0;
  
  struct lfs_info info = {};
  lfs_dir_t dir = {};

  //fs.mkdir("/config");

  //fs.dir_open(&dir, "/");
  //list_btn = lv_list_add_btn(list1, NULL , "/");  

  do {
    //size = fs.dir_read(&dir, &info);
    list_btn = lv_list_add_btn(list1, NULL , info.name);
  } while ( size > 0);  

  /*
  list_btn = lv_list_add_btn(list1, NULL , "Open");
  list_btn = lv_list_add_btn(list1, NULL , "Delete");
  list_btn = lv_list_add_btn(list1, NULL , "Edit");
  list_btn = lv_list_add_btn(list1, NULL , "Save");
  */

  //fs.dir_close(&dir);
}

FileManager::~FileManager() {


  lv_obj_clean(lv_scr_act());
}

bool FileManager::Refresh() {
  return running;
}

bool FileManager::OnButtonPushed() {
  running = false;
  return true;
}
