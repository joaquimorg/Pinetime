#pragma once

#include <cstdint>
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
        uint8_t Minutes() const { return minute; }
        uint8_t Seconds() const { return second; }

        const char* MonthToString();
        const char* DayOfWeekToString();
        const char* DayOfWeekShortToString();

        const char* MonthToStringLow();
        const char* DayOfWeekToStringLow();
        const char* DayOfWeekShortToStringLow();
        const char* MonthsGetLow();

        std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> CurrentDateTime() const { return currentDateTime; }
        std::chrono::seconds Uptime() const { return uptime; }
      private:
        uint16_t year = 2020;
        Months month = Months::December;
        uint8_t day = 7;
        Days dayOfWeek = Days::Monday;
        uint8_t hour = 21;
        uint8_t minute = 17;
        uint8_t second = 31;
          
        uint32_t previousSystickCounter = 0;
        std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> currentDateTime;
        std::chrono::seconds uptime {0};

        static char const *DaysString[];
        static char const *DaysStringShort[];
        static char const *MonthsString[];

        static char const *DaysStringLow[];
        static char const *DaysStringShortLow[];
        static char const *MonthsStringLow[];
        static char const *MonthsLow[];
    };
  }
}