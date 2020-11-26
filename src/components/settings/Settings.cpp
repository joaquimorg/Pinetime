#include "Settings.h"

using namespace Pinetime::Controllers;

void Settings::Init() {
    // default Clock face
    clockFace = 1;
}

void Settings::SetClockFace( uint8_t face ) {
    clockFace = face;
}
uint8_t Settings::GetClockFace() {
    return clockFace;
}

void Settings::SetAppMenu( uint8_t menu ) {
    appMenu = menu;
}

uint8_t Settings::GetAppMenu() {
    return appMenu;
}