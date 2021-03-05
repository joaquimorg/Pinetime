#include "Settings.h"

using namespace Pinetime::Controllers;


// TODO (team): 
// Read and write the settings to Flash
//

void Settings::Init() {
    // default Clock face
    clockFace = 3;

    stepsGoal = 5000;

    clockType = ClockType::H24;
}

