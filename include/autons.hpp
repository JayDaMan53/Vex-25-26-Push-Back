#pragma once

#include "autons.hpp"
void default_constants();

void drive_example();
void turn_example();
void drive_and_turn();
void wait_until_change_speed();
void swing_example();
void motion_chaining();
void combining_movements();
void interfered_example();
void odom_drive_example();
void odom_pure_pursuit_example();
void odom_pure_pursuit_wait_until_example();
void odom_boomerang_example();
void odom_boomerang_injected_pure_pursuit_example();
void measure_offsets();

// real autos //

void RedLeft_old();

void RedLeft();
void RedLeft_Alt();

void RedRight();
void RedRight_Alt();

void BlueLeft();
void BlueLeft_Alt();

void BlueRight();
void BlueRight_Alt();

void DoNothing();
void SkillsAuton();
void SkillsAuton_short();
void PureTest();