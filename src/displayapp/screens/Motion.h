#pragma once

#include <cstdint>
#include <chrono>
#include <lvgl/lvgl.h>
#include "Screen.h"

namespace Pinetime {

    namespace Controllers {
        class Accelerometer;
    }
    namespace Applications {
        namespace Screens {

            class Motion : public Screen{
            public:
                Motion(DisplayApp* app, Pinetime::Controllers::Accelerometer& accelerometer);
                ~Motion() override;

                bool Refresh() override;

            private:
                Pinetime::Controllers::Accelerometer& accelerometer;

                lv_obj_t * chart;
                lv_chart_series_t * ser1;
                lv_chart_series_t * ser2;
                lv_chart_series_t * ser3;
 
            };
        }
    }
}