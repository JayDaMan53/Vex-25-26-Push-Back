#include <array>
#include <cstddef>
#include <type_traits>

#include "liblvgl/widgets/lv_label.h"
#include "main.h"
#include "ui/screens.h"


// auto selector
extern "C" {
  #include "ui/ui.h"
}

// Auton Selector Vars

bool IsAMatch = false;
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
  {"Red Left - Normal", RedLeft},
  {"Red Right - Normal", RedRight},
  {"Blue Left - Normal", BlueLeft},
  {"Blue Right - Normal", BlueRight},

  // // No-Interference Autons
  // {"Red Left - No-Interference", RedLeft_Alt},
  // {"Red Right - No-Interference", RedRight_Alt},
  // {"Blue Left - No-Interference", BlueLeft_Alt},
  // {"Blue Right - No-Interference", BlueRight_Alt},

  // // Do nothing Auton
  // {"Do Nothing", DoNothing},

  // // Skills Auton
  {"Skills Auton", Skills}
};

// Auton Functions
void RunSelected() {
  RunningAuto = true;
  AutoSelect_loadScreen(SCREEN_ID_RUNNING);

  int autoIndex = 0;

  if (TeamColor == 1) { // Red Team
    if (TeamSide == 0) { // Left Side
      if (AutonType == 0) { // Normal
        autoIndex = 0;
      } else if (AutonType == 1) { // No-Interference
        autoIndex = 4;
      } else if (AutonType == 2) {
        autoIndex = 8;
      }
    } else if (TeamSide == 1) { // Right Side
      if (AutonType == 0) { // Normal
        autoIndex = 1;
      } else if (AutonType == 1) { // No-Interference
        autoIndex = 5;
      } else if (AutonType == 2) {
        autoIndex = 8;
      }
    }
  } else if (TeamColor == 2) { // Blue Team
    if (TeamSide == 0) { // Left Side
      if (AutonType == 0) { // Normal
        autoIndex = 2;
      } else if (AutonType == 1) { // No-Interference
        autoIndex = 6;
      } else if (AutonType == 2) {
        autoIndex = 8;
      }
    } else if (TeamSide == 1) { // Right Side
      if (AutonType == 0) { // Normal
        autoIndex = 3;
      } else if (AutonType == 1) { // No-Interference
        autoIndex = 7;
      } else if (AutonType == 2) {
        autoIndex = 8;
      }
    }
  } else if (TeamColor == 0) { // Skills
    autoIndex = 9;
  }

  lv_label_set_text(objects.running_label, ("Running: " + AutonOptions[autoIndex].name).c_str());
  if (AutonOptions[autoIndex].func != NULL) {
    AutonOptions[autoIndex].func();
  } else {
    change_color_theme(THEME_ID_SKILLS);
    AutoSelect_loadScreen(SCREEN_ID_TEAM_COLOR);
  }
}

void RunMatch() {
  lv_label_set_text(objects.running_label, "Running: Match Control");
  AutoSelect_loadScreen(SCREEN_ID_RUNNING);
}

// Auton Selector Functions

extern "C" void action_cancel_auton(lv_event_t * e) {
  change_color_theme(THEME_ID_SKILLS);
  AutoSelect_loadScreen(SCREEN_ID_TEAM_COLOR);
}

extern "C" void action_change_team_color(lv_event_t * e) {
  TeamColor = (int)(intptr_t)lv_event_get_user_data(e);
  change_color_theme(TeamColor);
  if (TeamColor == 0) {
    AutoSelect_loadScreen(SCREEN_ID_WAITING);
  } else {
    AutoSelect_loadScreen(SCREEN_ID_FEILD_SIDE);
  }
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
  AutoSelect_loadScreen(SCREEN_ID_WAITING);
}