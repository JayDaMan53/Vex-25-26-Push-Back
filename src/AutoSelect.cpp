#include <type_traits>
#include "main.h"
#include "ui/actions.h"
#include "ui/screens.h"

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

// Auton Functions
void RunSelected() {
  RunningAuto = true;
  loadScreen(SCREEN_ID_RUNNING);
}

// Auton Selector Functions

extern "C" void action_cancel_auton(lv_event_t * e) {
  change_color_theme(THEME_ID_SKILLS);
  loadScreen(SCREEN_ID_TEAM_COLOR);
}

extern "C" void action_change_team_color(lv_event_t * e) {
  TeamColor = (int)(intptr_t)lv_event_get_user_data(e);
  loadScreen(SCREEN_ID_FEILD_SIDE);
  change_color_theme(TeamColor);
}

extern "C" void action_change_feild_side(lv_event_t * e) {
  TeamSide = (int)(intptr_t)lv_event_get_user_data(e);
  loadScreen(SCREEN_ID_AUTO_TYPE);
}

extern "C" void action_start_auton(lv_event_t * e) {
  RunSelected();
}

extern "C" void action_change_auto_type(lv_event_t * e) {
  AutonType = (int)(intptr_t)lv_event_get_user_data(e);
  if (IsAMatch) {
    loadScreen(SCREEN_ID_WAITING);
  } else {
    RunSelected();
  }
}