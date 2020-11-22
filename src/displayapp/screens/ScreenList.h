#pragma once

#include <array>
#include <functional>
#include <memory>
#include "Screen.h"
#include "../DisplayApp.h"

namespace Pinetime {
  namespace Applications {
    namespace Screens {

      enum class ScreenListModes {UpDown, RightLeft, LongPress};
      template <size_t N>
      class ScreenList : public Screen {
        public:

          ScreenList(DisplayApp* app, std::array<std::function<std::unique_ptr<Screen>()>, N>&& screens, ScreenListModes mode, uint8_t initScreen)
          : Screen(app), screens{std::move(screens)}, current{this->screens[initScreen]()}, mode{mode} {}
          ~ScreenList() override {

          }

          bool Refresh() override {
            running = current->Refresh();
            return running;
          }

          bool OnButtonPushed() override {
            running = false;
            return true;
          }

          bool OnTouchEvent(TouchEvents event) override {

            if ( mode == ScreenListModes::UpDown) {
              switch (event) {
                case TouchEvents::SwipeDown:
                  if (screenIndex > 0) {
                    current.reset(nullptr);
                    app->SetFullRefresh(DisplayApp::FullRefreshDirections::Down);
                    screenIndex--;
                    current = screens[screenIndex]();
                    return true;
                  } else {
                    return false;
                  }
                  
                case TouchEvents::SwipeUp:
                  if (screenIndex < screens.size() - 1) {
                    current.reset(nullptr);
                    app->SetFullRefresh(DisplayApp::FullRefreshDirections::Up);
                    screenIndex++;
                    current = screens[screenIndex]();                
                  }
                  return true;
                default:
                  return false;
              }
            } else if ( mode == ScreenListModes::RightLeft) {
              switch (event) {
                case TouchEvents::SwipeRight:
                  if (screenIndex > 0) {
                    current.reset(nullptr);
                    app->SetFullRefresh(DisplayApp::FullRefreshDirections::None);
                    screenIndex--;
                    current = screens[screenIndex]();
                    return true;
                  } else {
                    return false;
                  }
                  
                case TouchEvents::SwipeLeft:
                  if (screenIndex < screens.size() - 1) {
                    current.reset(nullptr);
                    app->SetFullRefresh(DisplayApp::FullRefreshDirections::None);
                    screenIndex++;
                    current = screens[screenIndex]();                
                  }
                  return true;
                default:
                  return false;
              }
            } else if ( event == TouchEvents::LongTap ) {
              if (screenIndex < screens.size() - 1) {
                screenIndex++;
              } else {
                screenIndex = 0;
              }              
              current.reset(nullptr);
              app->SetFullRefresh(DisplayApp::FullRefreshDirections::None);
              current = screens[screenIndex]();
              return true;
            }

            return false;
          }

        private:
          bool running = true;
          ScreenListModes mode = ScreenListModes::UpDown;
          uint8_t screenIndex = 0;
          std::array<std::function<std::unique_ptr<Screen>()>, N> screens;
          std::unique_ptr<Screen> current;
      };
    }
  }
}