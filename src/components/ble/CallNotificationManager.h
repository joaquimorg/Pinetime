#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>

namespace Pinetime {
  namespace Controllers {
    class CallNotificationManager {
      public:
        enum class Categories {Unknown, IncomingCall, EndCall};
        struct Notification {
          std::array<char, 5> year;
          std::array<char, 3> month;
          std::array<char, 3> day;
          std::array<char, 3> hour;
          std::array<char, 3> minute;
          std::array<char, 3> second;
          std::array<char, 30> number;
          std::array<char, 30> callername;
          Categories category = Categories::Unknown;
        };

        void Set(Notification notif) { callNotification = notif; inCall = true; };
        Notification Get() { return callNotification; };
        void TerminateCall() { inCall = false; }
        bool IsInCall() { return inCall; }

      private:

        Notification callNotification;

        bool inCall = false;
        bool isVibration = false;

    };
  }
}