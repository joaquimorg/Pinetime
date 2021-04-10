#pragma once

#include <cstdint>
#include <lvgl/lvgl.h>
#include "Screen.h"
#include "systemtask/SystemTask.h"
#include "components/ble/CallNotificationManager.h"
#include "components/ble/AlertNotificationService.h"

namespace Pinetime {

  namespace Applications {
    namespace Screens {

      class IncomingCall : public Screen{
        public:
          IncomingCall(DisplayApp* app, 
            Pinetime::Controllers::CallNotificationManager& callNotificationManager, 
            Pinetime::Controllers::AlertNotificationService& alertNotificationService,
            System::SystemTask &systemTask);

          ~IncomingCall() override;

          bool Refresh() override;
          void OnButtonEvent(lv_obj_t *object, lv_event_t event);
         
        private:          

          Pinetime::Controllers::CallNotificationManager& callNotificationManager;
          Pinetime::Controllers::AlertNotificationService& alertNotificationService;
          Pinetime::System::SystemTask& systemTask;

          lv_obj_t * btn1;
          lv_obj_t * btn2;
          lv_obj_t * btn3;

      };
    }
  }
}
