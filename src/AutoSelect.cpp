#include <string.h>
#include <array>
#include <cstddef>
#include <cstdio>
#include <string>
#include <type_traits>

#include "autons.hpp"
#include "liblvgl/core/lv_obj.h"
#include "liblvgl/widgets/lv_label.h"
#include "main.h"
#include "ui/screens.h"


// auto selector
extern "C" {
  #include "ui/ui.h"
  #include "ui/images.h"
}

// Auton Selector Vars

bool IsAMatch = false;
bool RunningAuto = false;

int TeamColor = 1; // 1 = Red, 2 = Blue, 0 = Skills
int TeamSide = 0; // 0 = Left, 1 = Right
int AutonType = 0; // 0 = 7, 1 = 4, 2 = 0, 3 = 27, 4 = SAWP

int autoIndex = 10; 

struct AutonOption {
  std::string name;
  std::function<void()> func;
  std::optional<lv_obj_t*> ImgID;
};

AutonOption AutonOptions[] = {
  // 7 Block Autons
  {"Red Left - 7 Block", littlesaintjamesMirror, objects.ssgw2}, // 1 0 0
  {"Red Right - 7 Block", littlesaintjames, objects.ssgw2}, // 1 1 0
  {"Blue Left - 7 Block", littlesaintjamesMirror, objects.ssgw2}, // 2 0 0
  {"Blue Right - 7 Block", littlesaintjames, objects.ssgw2}, // 2 1 0

  // 4 Block Autons
  {"Red Left - 4 Block", OblockMirror, objects.ssgw}, // 1 0 1
  {"Red Right - 4 Block", Oblock, objects.ssgw}, // 1 1 1
  {"Blue Left - 4 Block", OblockMirror, objects.ssgw}, // 2 0 1
  {"Blue Right - 4 Block", Oblock, objects.ssgw}, // 2 1 1

  // 2 Goal 7 Block Auton
  {"2 Goal 7 Block", diddy, objects.dc}, // any any 3

  // SAWP
  {"Solo Auto Win Point", thuckuna, objects.sawp}, // any any 4

  // // Do nothing Auton
  {"Do Nothing - 0 Block", NULL}, // any any 2

  // // Skills Auton
  {"Skills Auto", israel, objects.skills_auto} // 0 skip skip
};

void RunningTick() {
  lv_label_set_text_fmt(objects.robot_battery_value, "%d%%", (int)pros::battery::get_capacity());
  lv_label_set_text(objects.robot_imu_value, chassis.drive_imu_calibrated() ? "True" : "False");
  lv_obj_set_style_text_color(objects.robot_imu_value, chassis.drive_imu_calibrated() ? lv_color_hex(0x00FF00) : lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(objects.robot_battery_value, (int)pros::battery::get_capacity() >= 75 ? lv_color_hex(0x00FF00) : (int)pros::battery::get_capacity() >= 25 ? lv_color_hex(0xFFFF00) : lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
}

// Auton Functions
void GetSelected() {
  AutoSelect_loadScreen(SCREEN_ID_RUNNING);

  // Skills overrides everything
  if (TeamColor == 0) {
    autoIndex = 11; // "Skills Auto"
  } else {
    switch (AutonType) {
      case 0: { // 7 Block
        // Red: 0-1, Blue: 2-3, add +1 if Right
        int base = (TeamColor == 1) ? 0 : 2;
        autoIndex = base + (TeamSide == 1 ? 1 : 0);
        break;
      }

      case 1: { // 4 Block
        // Red: 4-5, Blue: 6-7, add +1 if Right
        int base = (TeamColor == 1) ? 4 : 6;
        autoIndex = base + (TeamSide == 1 ? 1 : 0);
        break;
      }

      case 2: // 0 Block (Do Nothing)
        autoIndex = 10;
        break;

      case 3: // 2 Goal 7 Block (any color/side)
        autoIndex = 8;
        break;

      case 4: // SAWP (any color/side)
        autoIndex = 9;
        break;

      default:
        autoIndex = 10;
        break;
    }
  }

  lv_label_set_text(objects.running_label, ("Running: \n" + AutonOptions[autoIndex].name).c_str());

  // showObject(*AutonOptions[autoIndex].ImgID);

  if (*AutonOptions[autoIndex].ImgID) {
    showObject(*AutonOptions[autoIndex].ImgID);
  }

  if (AutonOptions[autoIndex].func == NULL) {
    // change_color_theme(THEME_ID_SKILLS);
    // AutoSelect_loadScreen(SCREEN_ID_TEAM_COLOR);
    lv_label_set_text(objects.running_label, ("Auto Not Found: \n" + AutonOptions[autoIndex].name).c_str());
  }
}

void RunSelected() {
  RunningAuto = true;
  if (AutonOptions[autoIndex].func != NULL) {
    AutonOptions[autoIndex].func();
  } else {
    lv_label_set_text(objects.running_label, ("Auto Not Found: \n" + AutonOptions[autoIndex].name).c_str());
  }
}

void RunMatch() {
  lv_label_set_text(objects.running_label, "Running: \nMatch Control");
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
    GetSelected();
  } else {
    AutoSelect_loadScreen(SCREEN_ID_FEILD_SIDE);
  }
}

extern "C" void action_change_feild_side(lv_event_t * e) {
  TeamSide = (int)(intptr_t)lv_event_get_user_data(e);
  AutoSelect_loadScreen(SCREEN_ID_AUTO_TYPE);
}

extern "C" void action_start_auton(lv_event_t * e) {
  // RunSelected();
}

extern "C" void action_change_auto_type(lv_event_t * e) {
  AutonType = (int)(intptr_t)lv_event_get_user_data(e);
  // AutoSelect_loadScreen(SCREEN_ID_WAITING);
  GetSelected();
}
