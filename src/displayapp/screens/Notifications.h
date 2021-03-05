#pragma once

#include <lvgl/lvgl.h>
#include <cstdint>
#include <memory>
#include "Screen.h"
#include "components/ble/NotificationManager.h"

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class Notifications : public Screen {
        public:

          explicit Notifications(DisplayApp* app, Pinetime::Controllers::NotificationManager& notificationManager);
          ~Notifications() override;

          bool Refresh() override;
          
          bool OnTouchEvent(Pinetime::Applications::TouchEvents event) override;

        private:

          class NotificationItem {
            public:
              NotificationItem(const char* title, Controllers::NotificationManager::Notification& msg, uint8_t notifNr, uint8_t notifNb);
              ~NotificationItem();
              bool Refresh() {return false;}

            private:
              Controllers::NotificationManager::Notification& msg;
              uint8_t notifNr = 0;
              uint8_t notifNb = 0;
              
          };

          struct NotificationData {
            const char* title;
            const char* text;
          };

          Pinetime::Controllers::NotificationManager& notificationManager;
          
          std::unique_ptr<NotificationItem> currentItem;
          Controllers::NotificationManager::Notification::Id currentId;
          bool validDisplay = false;

          static char const *CategoriesString[];
          static void const *CategoriesIcon[];
          static lv_color_t const CategoriesColor[];
          const char* CategoryToString( Controllers::NotificationManager::Categories category );
      };
    }
  }
}
