#pragma once

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <chrono>

namespace Pinetime {
  namespace Controllers {
    class DateTime {
      public:
        enum class Days : uint8_t {Unknown, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday};
        enum class Months : uint8_t {Unknown, January, February, March, April, May, June, July, August, September, October, November, December};

        void SetTime(uint16_t year, uint8_t month, uint8_t day, uint8_t dayOfWeek, uint8_t hour, uint8_t minute, uint8_t second, uint32_t systickCounter);
        void UpdateTime(uint32_t systickCounter);
        uint16_t Year() const { return year; }
        Months Month() const { return month; }
        uint8_t Day() const { return day; }
        Days DayOfWeek() const { return dayOfWeek; }

        uint8_t Hours() const { return hour; }
        uint8_t Hours12();
        
        const char *HourAMPM( bool nl );
        
        const char *GetTimeStr( bool is12 ) {
          char *timestr = new char[10];
          if ( is12 ) {
            sprintf(timestr, "%i:%02i %s", Hours12(), Minutes(), HourAMPM(false));
          } else {
            sprintf(timestr, "%02i:%02i", Hours(), Minutes());
          }
          return timestr;
        }

        uint8_t Minutes() const { return minute; }
        uint8_t Seconds() const { return second; }

        const char *MonthShortToString();
        const char *MonthShortToStringLow();
        const char *MonthToStringLow();
        const char *DayOfWeekToString();
        const char *DayOfWeekShortToString();
        const char *DayOfWeekToStringLow();
        const char *DayOfWeekShortToStringLow();

        std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> CurrentDateTime() const { return currentDateTime; }
        std::chrono::seconds Uptime() const { return uptime; }

        static char const *DaysString[];
        static char const *DaysStringShort[];
        static char const *DaysStringLow[];
        static char const *DaysStringShortLow[];
        static char const *MonthsString[];
        static char const *MonthsStringLow[];
        static char const *MonthsLow[];

      private:
        uint16_t year = 2021;
        Months month = Months::January;
        uint8_t day = 0;
        Days dayOfWeek = Days::Monday;
        uint8_t hour = 0;
        uint8_t minute = 0;
        uint8_t second = 0;
          
        uint32_t previousSystickCounter = 0;
        std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> currentDateTime;
        std::chrono::seconds uptime {0};
        
    };
  }
}