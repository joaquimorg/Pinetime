#include "Settings.h"

using namespace Pinetime::Controllers;


// TODO (team): 
// Read and write the settings to Flash
//

void Settings::Init() {
    // default Clock face
    clockFace = 0;

    stepsGoal = 5000;

    clockType = ClockType::H24;
}



void Settings::SetHistorySteps( Accelerometer steps, DateTime date ) {
    history_step_struct currStepHistory = stepHistory[stepHistoryPos];

    if ( date.Day() == currStepHistory.day && date.Month() == currStepHistory.month && date.Year() == currStepHistory.year ) {
        stepHistory[stepHistoryPos].steps = steps.GetSteps();
    } else {
        
        steps.ResetSteps();

        currStepHistory.day = date.Day();
        currStepHistory.month = date.Month();
        currStepHistory.year = date.Year();

        stepHistoryPos++;
        if ( stepHistoryPos > 8 ) stepHistoryPos = 0;
        stepHistory[stepHistoryPos] = currStepHistory;
    }
}