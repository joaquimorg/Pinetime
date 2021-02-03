#include "Label.h"

using namespace Pinetime::Applications::Screens;

Label::Label(Pinetime::Applications::DisplayApp *app, lv_obj_t* labelText)  : 
  Screen(app), 
  labelText{labelText} {

  lv_label_set_align(labelText, LV_LABEL_ALIGN_LEFT);
  lv_obj_set_size(labelText, 240, 240);

}

Label::~Label() {
  lv_obj_clean(lv_scr_act());
}
