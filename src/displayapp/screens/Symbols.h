#pragma once

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      namespace Symbols {
        static constexpr const char* none = "";

        // font sys 20
        //         
        //  lv_font_conv --font icons_sys.ttf -r 0xe920 -r 0xe924 -r 0xe921 -r 0xe922 -r 0xe923 -r 0xe906 -r 0xe907 -r 0xe901 -r 0xe903 -r 0xe905 -r 0xe904 -r 0xe902 -r 0xe900 -r 0xe908 -r 0xe909 -r 0xe90a -r 0xe90b -r 0xe90c --size 20 --format lvgl --bpp 2 --no-compress -o lv_font_sys_20.c
        static constexpr const char* batteryFull          = "\xEE\xA4\xA0"; //e920
        static constexpr const char* batteryEmpty         = "\xEE\xA4\xA4"; //e924
        static constexpr const char* batteryThreeQuarter  = "\xEE\xA4\xA1"; //e921
        static constexpr const char* batteryHalf          = "\xEE\xA4\xA2"; //e922
        static constexpr const char* batteryOneQuarter    = "\xEE\xA4\xA3"; //e923
        static constexpr const char* batteryError         = "\xEE\xA4\x86"; //e906
        static constexpr const char* batteryUnknown       = "\xEE\xA4\x87"; //e907
        static constexpr const char* heartBeat            = "\xEE\xA4\x81"; //e901
        static constexpr const char* bluetooth            = "\xEE\xA4\x83"; //e903
        static constexpr const char* plug                 = "\xEE\xA4\x85"; //e905
        static constexpr const char* shoe                 = "\xEE\xA4\x84"; //e904
        static constexpr const char* dot                  = "\xEE\xA4\x82"; //e902
        static constexpr const char* info                 = "\xEE\xA4\x80"; //e900
        static constexpr const char* phone                = "\xEE\xA4\x88"; //e908
        static constexpr const char* clock                = "\xEE\xA4\x89"; //e909
        static constexpr const char* backLight            = "\xEE\xA4\x8A"; //e90a
        static constexpr const char* firmware             = "\xEE\xA4\x8B"; //e90b
        static constexpr const char* about                = "\xEE\xA4\x8C"; //e90c

        // font sys 48
        // 
        static constexpr const char* tennisRacket1        = "\xEE\xA4\x81";//e901
        static constexpr const char* settings             = "\xEE\xA4\x82";//e902
        static constexpr const char* tennisRacket         = "\xEE\xA4\x80";//e900
        static constexpr const char* ringVolume           = "\xEE\xA4\x83";//e903
        static constexpr const char* brightnessHigh       = "\xEE\xA4\x84";//e904
        static constexpr const char* brightnessLow        = "\xEE\xA4\x85";//e905
        static constexpr const char* brightnessMedium     = "\xEE\xA4\x86";//e906
        static constexpr const char* highlight            = "\xEE\xA4\x87";//e907
        static constexpr const char* wbSunny              = "\xEE\xA4\x88";//e908
        static constexpr const char* phoneInTalk          = "\xEE\xA4\x89";//e909
        static constexpr const char* notificationsOff     = "\xEE\xA4\x8B";//e90b
        static constexpr const char* notificationsOn      = "\xEE\xA4\x8C";//e90c
        static constexpr const char* settingsPower        = "\xEE\xA4\x8E";//e90e
        static constexpr const char* phoneDisabled        = "\xEE\xA4\x90";//e910
        static constexpr const char* toggleOff            = "\xEE\xA4\x91";//e911
        static constexpr const char* toggleOn             = "\xEE\xA4\x92";//e912
        static constexpr const char* pestControl          = "\xEE\xA4\x93";//e913
        static constexpr const char* qrCode               = "\xEE\xA4\x94";//e914
      }
    }
  }
}

