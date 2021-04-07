#include <lvgl/lvgl.h>
#include "Motion.h"
#include "../DisplayApp.h"
#include "components/accelerometer/Accelerometer.h"

using namespace Pinetime::Applications::Screens;

Motion::Motion(Pinetime::Applications::DisplayApp *app, Pinetime::Controllers::Accelerometer& accelerometer) : Screen(app), accelerometer{accelerometer} {
    chart = lv_chart_create(lv_scr_act(), NULL);
    lv_obj_set_size(chart, 240, 240);
    lv_obj_align(chart, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);   /*Show lines and points too*/
    //lv_chart_set_series_opa(chart, LV_OPA_70);                            /*Opacity of the data series*/
    //lv_chart_set_series_width(chart, 4);                                  /*Line width and point radious*/

    lv_chart_set_range(chart, -1100, 1100);
    lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);
    lv_chart_set_point_count(chart, 10);

    /*Add 3 data series*/
    ser1 = lv_chart_add_series(chart, LV_COLOR_RED);
    ser2 = lv_chart_add_series(chart, LV_COLOR_GREEN);
    ser3 = lv_chart_add_series(chart, LV_COLOR_YELLOW);

    lv_chart_init_points(chart, ser1, 0);
    lv_chart_init_points(chart, ser2, 0);
    lv_chart_init_points(chart, ser3, 0);
    lv_chart_refresh(chart); /*Required after direct set*/

}

Motion::~Motion() {
    lv_obj_clean(lv_scr_act());
}

bool Motion::Refresh() {
    accelerometer.UpdateAccel();
    accl_data_struct accl = accelerometer.GetAccel();
    lv_chart_set_next(chart, ser1, accl.x);
    lv_chart_set_next(chart, ser2, accl.y);
    lv_chart_set_next(chart, ser3, accl.z);

  return running;
}

