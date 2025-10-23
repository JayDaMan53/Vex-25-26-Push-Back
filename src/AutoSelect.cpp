#include <array>
#include <cstddef>
#include <type_traits>

// #include "AutoSelect.hpp"
#include "main.h"


// auto selector
extern "C" {
  #include "ui/ui.h"
}

// Auton Selector Vars

bool IsAMatch = true;
bool RunningAuto = false;

int TeamColor = 1; // 1 = Red, 2 = Blue, 0 = Skills
int TeamSide = 0; // 0 = Left, 1 = Right
int AutonType = 0; // 0 = Normal, 1 = no-interference, 2 = Skills

struct AutonOption {
  std::string name;
  std::function<void()> func;
};

AutonOption AutonOptions[] = {
  // Normal Autons
  {"Red Left - Normal", NULL},
  {"Red Right - Normal", NULL},
  {"Blue Left - Normal", NULL},
  {"Blue Right - Normal", NULL},

  // No-Interference Autons
  {"Red Left - No-Interference", NULL},
  {"Red Right - No-Interference", NULL},
  {"Blue Left - No-Interference", NULL},
  {"Blue Right - No-Interference", NULL},

  // Do nothing Auton
  {"Do Nothing", NULL},

  // Skills Auton
  {"Skills Auton", NULL}
};

// Auton Functions
void RunSelected() {
  RunningAuto = true;
  AutoSelect_loadScreen(SCREEN_ID_RUNNING);

  if (TeamColor == 1) { // Red Team
    if (TeamSide == 0) { // Left Side
      if (AutonType == 0) { // Normal
        AutonOptions[0].func();
      } else if (AutonType == 1) { // No-Interference
        AutonOptions[4].func();
      } else if (AutonType == 2) {
        AutonOptions[8].func();
      }
    } else if (TeamSide == 1) { // Right Side
      if (AutonType == 0) { // Normal
        AutonOptions[1].func();
      } else if (AutonType == 1) { // No-Interference
        AutonOptions[5].func();
      } else if (AutonType == 2) {
        AutonOptions[8].func();
      }
    }
  } else if (TeamColor == 2) { // Blue Team
    if (TeamSide == 0) { // Left Side
      if (AutonType == 0) { // Normal
        AutonOptions[2].func();
      } else if (AutonType == 1) { // No-Interference
        AutonOptions[6].func();
      } else if (AutonType == 2) {
        AutonOptions[8].func();
      }
    } else if (TeamSide == 1) { // Right Side
      if (AutonType == 0) { // Normal
        AutonOptions[3].func();
      } else if (AutonType == 1) { // No-Interference
        AutonOptions[7].func();
      } else if (AutonType == 2) {
        AutonOptions[8].func();
      }
    }
  } else if (TeamColor == 0) { // Skills
    AutonOptions[9].func();
  }
}

// Auton Selector Functions

extern "C" void action_cancel_auton(lv_event_t * e) {
  change_color_theme(THEME_ID_SKILLS);
  AutoSelect_loadScreen(SCREEN_ID_TEAM_COLOR);
}

extern "C" void action_change_team_color(lv_event_t * e) {
  TeamColor = (int)(intptr_t)lv_event_get_user_data(e);
  AutoSelect_loadScreen(SCREEN_ID_FEILD_SIDE);
  change_color_theme(TeamColor);
}

extern "C" void action_change_feild_side(lv_event_t * e) {
  TeamSide = (int)(intptr_t)lv_event_get_user_data(e);
  AutoSelect_loadScreen(SCREEN_ID_AUTO_TYPE);
}

extern "C" void action_start_auton(lv_event_t * e) {
  RunSelected();
}

extern "C" void action_change_auto_type(lv_event_t * e) {
  AutonType = (int)(intptr_t)lv_event_get_user_data(e);
  if (IsAMatch) {
    AutoSelect_loadScreen(SCREEN_ID_WAITING);
  } else {
    RunSelected();
  }
}