#include <future>
#include "EZ-Template/util.hpp"
#include "main.h"
#include "pros/imu.h"
#include "pros/motors.hpp"
#include "subsystems.hpp"

/////
// For installation, upgrading, documentations, and tutorials, check out our website!
// https://ez-robotics.github.io/EZ-Template/
/////

// These are out of 127
const int DRIVE_SPEED = 110; // 110
const int TURN_SPEED = 90;
const int SWING_SPEED = 110;

extern pros::Controller master;

extern pros::adi::DigitalOut tongue;
extern pros::adi::DigitalOut HoodHook;
extern pros::adi::DigitalOut TonguePiston;

extern pros::Optical ColorSensor;
extern pros::Optical ColorSensor2;

extern pros::Motor intakeMotor;
extern pros::Motor ScoreMotor;

extern bool HoodHookState;

///
// Constants
///
void default_constants() {
  // P, I, D, and Start I

  chassis.pid_drive_constants_set(10.0, 0.05, 120.0, 30.0);         // Fwd/rev constants, used for odom and non odom motions
  chassis.pid_heading_constants_set(10.0, 0.0, 100.0);        // Holds the robot straight while going forward without odom
  chassis.pid_turn_constants_set(9.0, 0.0, 80.0, 0.0);     // Turn in place constants
  chassis.pid_swing_constants_set(9.0, 0.0, 85.0);           // Swing constants
  chassis.pid_odom_angular_constants_set(6.5, 0.0, 52.5);    // Angular control for odom motions
  chassis.pid_odom_boomerang_constants_set(5.8, 0.0, 32.5);  // Angular control for boomerang motions

  // chassis.pid_drive_constants_set(6.0, 0.05, 25.0);         // Fwd/rev constants, used for odom and non odom motions
  // chassis.pid_heading_constants_set(8.5, 0.0, 40.0);        // Holds the robot straight while going forward without odom
  // chassis.pid_turn_constants_set(4.0, 0.00, 100.0, 20.0);     // Turn in place constants
  // chassis.pid_swing_constants_set(6.0, 0.0, 65.0);           // Swing constants
  // chassis.pid_odom_angular_constants_set(5.8, 0.0, 32.5);    // Angular control for odom motions
  // chassis.pid_odom_boomerang_constants_set(5.8, 0.0, 32.5);  // Angular control for boomerang motions

  // Exit conditions
  chassis.pid_turn_exit_condition_set(90_ms, 1_deg, 250_ms, 7_deg, 500_ms, 500_ms);
  chassis.pid_swing_exit_condition_set(90_ms, 3_deg, 250_ms, 7_deg, 500_ms, 500_ms);
  chassis.pid_drive_exit_condition_set(90_ms, 1_in, 250_ms, 3_in, 500_ms, 500_ms);
  chassis.pid_odom_turn_exit_condition_set(90_ms, 1_deg, 250_ms, 7_deg, 500_ms, 750_ms);
  chassis.pid_odom_drive_exit_condition_set(90_ms, 1_in, 250_ms, 3_in, 500_ms, 750_ms);
  chassis.pid_turn_chain_constant_set(1_deg);
  chassis.pid_swing_chain_constant_set(5_deg);
  chassis.pid_drive_chain_constant_set(3_in);

  // Slew constants
  chassis.slew_turn_constants_set(3_deg, 70);
  chassis.slew_drive_constants_set(3_in, 70);
  chassis.slew_swing_constants_set(3_in, 80);

  // The amount that turns are prioritized over driving in odom motions
  // - if you have tracking wheels, you can run this higher.  1.0 is the max
  chassis.odom_turn_bias_set(0.9);

  chassis.odom_look_ahead_set(7_in);           // This is how far ahead in the path the robot looks at
  chassis.odom_boomerang_distance_set(16_in);  // This sets the maximum distance away from target that the carrot point can be
  chassis.odom_boomerang_dlead_set(0.625);     // This handles how aggressive the end of boomerang motions are

  chassis.pid_angle_behavior_set(ez::shortest);  // Changes the default behavior for turning, this defaults it to the shortest path there
}

///
// Drive Example
///
void drive_example() {
  // The first parameter is target inches
  // The second parameter is max speed the robot will drive at
  // The third parameter is a boolean (true or false) for enabling/disabling a slew at the start of drive motions
  // for slew, only enable it when the drive distance is greater than the slew distance + a few inches

  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_drive_set(-12_in, DRIVE_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-12_in, DRIVE_SPEED);
  chassis.pid_wait();
}

///
// Turn Example
///
void turn_example() {
  // The first parameter is the target in degrees
  // The second parameter is max speed the robot will drive at

  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();
}

///
// Combining Turn + Drive
///
void drive_and_turn() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Wait Until and Changing Max Speed
///
void wait_until_change_speed() {
  // pid_wait_until will wait until the robot gets to a desired position

  // When the robot gets to 6 inches slowly, the robot will travel the remaining distance at full speed
  chassis.pid_drive_set(24_in, 30, true);
  chassis.pid_wait_until(6_in);
  chassis.pid_speed_max_set(DRIVE_SPEED);  // After driving 6 inches at 30 speed, the robot will go the remaining distance at DRIVE_SPEED
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  // When the robot gets to -6 inches slowly, the robot will travel the remaining distance at full speed
  chassis.pid_drive_set(-24_in, 30, true);
  chassis.pid_wait_until(-6_in);
  chassis.pid_speed_max_set(DRIVE_SPEED);  // After driving 6 inches at 30 speed, the robot will go the remaining distance at DRIVE_SPEED
  chassis.pid_wait();
}

///
// Swing Example
///
void swing_example() {
  // The first parameter is ez::LEFT_SWING or ez::RIGHT_SWING
  // The second parameter is the target in degrees
  // The third parameter is the speed of the moving side of the drive
  // The fourth parameter is the speed of the still side of the drive, this allows for wider arcs

  chassis.pid_swing_set(ez::LEFT_SWING, 45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, 0_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, 45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::LEFT_SWING, 0_deg, SWING_SPEED, 45);
  chassis.pid_wait();
}

///
// Motion Chaining
///
void motion_chaining() {
  // Motion chaining is where motions all try to blend together instead of individual movements.
  // This works by exiting while the robot is still moving a little bit.
  // To use this, replace pid_wait with pid_wait_quick_chain.
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait_quick_chain();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait_quick_chain();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  // Your final motion should still be a normal pid_wait
  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Auto that tests everything
///
void combining_movements() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, -45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Interference example
///
void tug(int attempts) {
  for (int i = 0; i < attempts - 1; i++) {
    // Attempt to drive backward
    printf("i - %i", i);
    chassis.pid_drive_set(-12_in, 127);
    chassis.pid_wait();

    // If failsafed...
    if (chassis.interfered) {
      chassis.drive_sensor_reset();
      chassis.pid_drive_set(-2_in, 20);
      pros::delay(1000);
    }
    // If the robot successfully drove back, return
    else {
      return;
    }
  }
}

// If there is no interference, the robot will drive forward and turn 90 degrees.
// If interfered, the robot will drive forward and then attempt to drive backward.
void interfered_example() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  if (chassis.interfered) {
    tug(3);
    return;
  }

  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();
}

///
// Odom Drive PID
///
void odom_drive_example() {
  // This works the same as pid_drive_set, but it uses odom instead!
  // You can replace pid_drive_set with pid_odom_set and your robot will
  // have better error correction.

  chassis.pid_odom_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_odom_set(-12_in, DRIVE_SPEED);
  chassis.pid_wait();

  chassis.pid_odom_set(-12_in, DRIVE_SPEED);
  chassis.pid_wait();
}

///
// Odom Pure Pursuit
///
void odom_pure_pursuit_example() {
  // Drive to 0, 30 and pass through 6, 10 and 0, 20 on the way, with slew
  chassis.pid_odom_set({{{6_in, 10_in}, fwd, DRIVE_SPEED},
                        {{0_in, 20_in}, fwd, DRIVE_SPEED},
                        {{0_in, 30_in}, fwd, DRIVE_SPEED}},
                       true);
  chassis.pid_wait();

  // Drive to 0, 0 backwards
  chassis.pid_odom_set({{0_in, 0_in}, rev, DRIVE_SPEED},
                       true);
  chassis.pid_wait();
}

///
// Odom Pure Pursuit Wait Until
///
void odom_pure_pursuit_wait_until_example() {
  chassis.pid_odom_set({{{0_in, 24_in}, fwd, DRIVE_SPEED},
                        {{12_in, 24_in}, fwd, DRIVE_SPEED},
                        {{24_in, 24_in}, fwd, DRIVE_SPEED}},
                       true);
  chassis.pid_wait_until_index(1);  // Waits until the robot passes 12, 24
  // Intake.move(127);  // Set your intake to start moving once it passes through the second point in the index
  chassis.pid_wait();
  // Intake.move(0);  // Turn the intake off
}

///
// Odom Boomerang
///
void odom_boomerang_example() {
  chassis.pid_odom_set({{0_in, 24_in, 45_deg}, fwd, DRIVE_SPEED},
                       true);
  chassis.pid_wait();

  chassis.pid_odom_set({{0_in, 0_in, 0_deg}, rev, DRIVE_SPEED},
                       true);
  chassis.pid_wait();
}

///
// Odom Boomerang Injected Pure Pursuit
///
void odom_boomerang_injected_pure_pursuit_example() {
  chassis.pid_odom_set({{{0_in, 24_in, 45_deg}, fwd, DRIVE_SPEED},
                        {{12_in, 24_in}, fwd, DRIVE_SPEED},
                        {{24_in, 24_in}, fwd, DRIVE_SPEED}},
                       true);
  chassis.pid_wait();

  chassis.pid_odom_set({{0_in, 0_in, 0_deg}, rev, DRIVE_SPEED},
                       true);
  chassis.pid_wait();
}

///
// Calculate the offsets of your tracking wheels
///
void measure_offsets() {
  // Number of times to test
  int iterations = 10;

  // Our final offsets
  double l_offset = 0.0, r_offset = 0.0, b_offset = 0.0, f_offset = 0.0;

  // Reset all trackers if they exist
  if (chassis.odom_tracker_left != nullptr) chassis.odom_tracker_left->reset();
  if (chassis.odom_tracker_right != nullptr) chassis.odom_tracker_right->reset();
  if (chassis.odom_tracker_back != nullptr) chassis.odom_tracker_back->reset();
  if (chassis.odom_tracker_front != nullptr) chassis.odom_tracker_front->reset();
  
  for (int i = 0; i < iterations; i++) {
    // Reset pid targets and get ready for running an auton
    chassis.pid_targets_reset();
    chassis.drive_imu_reset();
    chassis.drive_sensor_reset();
    chassis.drive_brake_set(pros::E_MOTOR_BRAKE_HOLD);
    chassis.odom_xyt_set(0_in, 0_in, 0_deg);
    double imu_start = chassis.odom_theta_get();
    double target = i % 2 == 0 ? 90 : 270;  // Switch the turn target every run from 270 to 90

    // Turn to target at half power
    chassis.pid_turn_set(target, 63, ez::raw);
    chassis.pid_wait();
    pros::delay(250);

    // Calculate delta in angle
    double t_delta = util::to_rad(fabs(util::wrap_angle(chassis.odom_theta_get() - imu_start)));

    // Calculate delta in sensor values that exist
    double l_delta = chassis.odom_tracker_left != nullptr ? chassis.odom_tracker_left->get() : 0.0;
    double r_delta = chassis.odom_tracker_right != nullptr ? chassis.odom_tracker_right->get() : 0.0;
    double b_delta = chassis.odom_tracker_back != nullptr ? chassis.odom_tracker_back->get() : 0.0;
    double f_delta = chassis.odom_tracker_front != nullptr ? chassis.odom_tracker_front->get() : 0.0;

    // Calculate the radius that the robot traveled
    l_offset += l_delta / t_delta;
    r_offset += r_delta / t_delta;
    b_offset += b_delta / t_delta;
    f_offset += f_delta / t_delta;
  }

  // Average all offsets
  l_offset /= iterations;
  r_offset /= iterations;
  b_offset /= iterations;
  f_offset /= iterations;

  // Set new offsets to trackers that exist
  if (chassis.odom_tracker_left != nullptr) chassis.odom_tracker_left->distance_to_center_set(l_offset);
  if (chassis.odom_tracker_right != nullptr) chassis.odom_tracker_right->distance_to_center_set(r_offset);
  if (chassis.odom_tracker_back != nullptr) chassis.odom_tracker_back->distance_to_center_set(b_offset);
  if (chassis.odom_tracker_front != nullptr) chassis.odom_tracker_front->distance_to_center_set(f_offset);
}

extern void ChangeScoreState(bool State);
extern void Score(void* State);

// MARK: bad auto
void test() {
  chassis.odom_y_flip(true);
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);

  intakeMotor.move(127);

  chassis.pid_odom_set({{{10_in, 24_in}, rev, 127},}, true);
  chassis.pid_wait();

  // chassis.pid_swing_set(ez::LEFT_SWING, -90_deg, 80);
  // chassis.pid_wait();

  chassis.pid_odom_set({{{-13_in, 24_in}, rev, 100},}, true);
  chassis.pid_wait();

  pros::delay(1000);

  // chassis.pid_odom_set({{{-10_in, 26_in}, rev, 100},}, true);
  // chassis.pid_wait();

  // chassis.pid_odom_set({{{-28_in, 44_in}, rev, 100},}, true);
  // chassis.pid_wait();

  // TonguePiston.set_value(true);
  // pros::delay(1000);

  // chassis.pid_odom_set({{{-5_in, 20_in}, fwd, 110},}, true);
  // chassis.pid_wait();

  ChangeScoreState(true);

  chassis.pid_odom_set({{{-33_in, 0_in}, rev, 127},}, true);
  chassis.pid_wait();

  chassis.pid_turn_set(180_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_odom_set({{{-33_in, 25_in}, fwd, 127},}, true);
  chassis.pid_wait();

  chassis.odom_xyt_set(-33_in, 25_in, 180_deg);

  TonguePiston.set_value(true);

  pros::Task ScoreThread(Score, (void*)true);
  pros::delay(500);

  chassis.pid_odom_set({{{-33_in, -5_in}, rev, 80},}, true);
  chassis.pid_wait();

  pros::delay(1000);

  chassis.odom_xyt_set(-33_in, -5_in, 180_deg);

  // chassis.pid_odom_set({{{-31_in, 22_in}, fwd, 80},}, true);
  // chassis.pid_odom_set({{{-33_in, 25_in, 180_deg}, fwd, 127},}, true);
  // chassis.pid_wait();

  chassis.pid_drive_set(30_in, 127, true, true);
  chassis.pid_wait();

  TonguePiston.set_value(false);

  intakeMotor.move(0);
  pros::Task ScoreThreadAgain(Score, (void*)true);
  pros::delay(500);
}

void RedLeft() {
  chassis.odom_x_flip(true);
  test();
}

void RedRight() {
  chassis.odom_theta_flip();
  test();
}

void BlueLeft() {
  chassis.odom_x_flip(true);
  test();
}

void BlueRight() {
  chassis.odom_theta_flip();
  test();
}

void Skills() {
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);
  chassis.pid_drive_set(45_in, 127, true);
  chassis.pid_wait();
  chassis.pid_drive_set(-5_in, 80, true);
  chassis.pid_wait();
}

// MARK: Diddy
void diddy() {
  //All movements use skewing for smoother acceleration and deceleration

  //Reset the navigation sensors to 0,0,0 at the start of auton
  chassis.odom_y_flip(true);
  chassis.odom_x_flip(true);
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);

  //Start spinning the intake and intake preload (if used)
  intakeMotor.move(127);

  //Drive to the match loader at 110/127 speed
  chassis.pid_drive_set(35_in, 110, true);
  chassis.pid_wait();

  //Activate the tongue and lift storage to the high goal position
  TonguePiston.set_value(1);
  ChangeScoreState(true);

  //Turn towards the match loader
  chassis.pid_turn_set(-88_deg, TURN_SPEED);
  chassis.pid_wait();

  //Drive into the match loader at full speed
  chassis.pid_drive_set(-13.25_in, 80, true);
  pros::delay(1000);
  
  //Reverse out of the match loader and into the goal
  //Moves slightly "past" the goal to ensure the aligner works properly
  chassis.pid_drive_set(30_in, 60, true);
  chassis.pid_wait_until(15_in);
  //Put the tongue up after moving 15 inches (halfway)
  TonguePiston.set_value(0);
  chassis.pid_wait();

  //Score in the high goal
  //Open the hood and lift the wing
  HoodHook.set_value(false);
  //Activate lever scoring mechanism
  ScoreMotor.move(127);
  pros::delay(500);
  //Reset the scoring lever position
  ScoreMotor.move(-20);

  //Reset sensors to start fresh with new alignment
  //Avoids cumulative inertial error
  //New relative bearing for more coherent coding
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);
  
  //Drive backwards out of the goal and start spinning the intake
  chassis.pid_drive_set(-16_in, 50, true);
  chassis.pid_wait();
  intakeMotor.move(127);

  //Reset sensors to start fresh with new alignment
  //Sets new theta position to -90 degrees
  chassis.odom_xyt_set(0_in, 0_in, -90_deg);

  //Turn towards the collection of center blocks
  //Based on the heading from the new alignment above
  chassis.pid_turn_set(40_deg, 100);
  chassis.pid_wait();

  //Drive forwards to the center of the field to collect blocks
  chassis.pid_drive_set(-34_in, 80, true);
  //After 28 inches, put down the tongue to trap and collect unintook blocks
  chassis.pid_wait_until(-28_in);
  TonguePiston.set_value(1);
  chassis.pid_wait();

  //Drive forwards into the lower goal
  chassis.pid_drive_set(-15_in, 127, true);
  //Put the tongue up to let the blocks outtake freely
  TonguePiston.set_value(0);
  chassis.pid_wait();
  //Outtake to score collected blocks
  intakeMotor.move(-127);
  //Shake the bot to outtake stuck blocks
  chassis.pid_drive_set(1_in, 127, true);
  chassis.pid_wait();
  chassis.pid_drive_set(-2_in, 127, true);

}

// MARK: Diddy Mirror
void diddyMirror() {
  //All movements use skewing for smoother acceleration and deceleration

  //Reset the navigation sensors to 0,0,0 at the start of auton
  chassis.odom_y_flip(true);
  chassis.odom_x_flip(true);
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);

  //Start spinning the intake and intake preload (if used)
  intakeMotor.move(127);

  //Drive to the match loader at 110/127 speed
  chassis.pid_drive_set(35_in, 110, true);
  chassis.pid_wait();

  //Activate the tongue and lift storage to the high goal position
  TonguePiston.set_value(1);
  ChangeScoreState(true);

  //Turn towards the match loader
  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();

  //Drive into the match loader at full speed
  chassis.pid_drive_set(-13.25_in, 80, true);
  pros::delay(1000);
  
  //Reverse out of the match loader and into the goal
  //Moves slightly "past" the goal to ensure the aligner works properly
  chassis.pid_drive_set(30_in, 60, true);
  //Put the tongue up after moving 15 inches (halfway)
  chassis.pid_wait_until(15_in);
  TonguePiston.set_value(0);
  chassis.pid_wait();

  //Score in the high goal
  //Open the hood and lift the wing
  HoodHook.set_value(false);
  //Activate lever scoring mechanism
  ScoreMotor.move(127);
  pros::delay(500);
  //Reset the scoring lever position
  ScoreMotor.move(-50);

  //Reset sensors to start fresh with new alignment
  //Avoids cumulative inertial error
  //New relative bearing for more coherent coding
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);

  //Turn towards the blocks using a swing function at 100/127 speed
  chassis.pid_swing_set(ez::LEFT_SWING, -135_deg, 100);
  chassis.pid_wait();
  ChangeScoreState(false);

  chassis.odom_xyt_set(0_in, 0_in, 0_deg);
  
  //Drive to the center field blocks
  chassis.pid_drive_set(-16_in, 127, true);
  //Activate the tongue after moving 5 inches to trap any unintook blocks
  chassis.pid_wait_until(-5_in);
  TonguePiston.set_value(1);
  chassis.pid_wait();

  //turn to the center goal
  chassis.pid_turn_set(175_deg, 100);
  chassis.pid_wait();

  //Drive to the center goal
  chassis.pid_drive_set(18_in, 80, true);
  chassis.pid_wait();

  //Activate the scoring mechanism at a reduced speed
  //Prevents blocks from overshooting the goal and falling out
  HoodHook.set_value(false);
  ScoreMotor.move(50);
  pros::delay(1000); 
  //Reset scoring lever position
  ScoreMotor.move(-50);

}

// MARK: Diddy Skills
void diddyskills() {
  //All movements use skewing for smoother acceleration and deceleration

  //Reset the navigation sensors to 0,0,0 at the start of auton
  chassis.odom_y_flip(true);
  chassis.odom_x_flip(true);
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);

  //Start spinning the intake
  intakeMotor.move(127);

  //Drive to the match loader at 80/127 speed
  chassis.pid_drive_set(34.5_in, 80, true);
  chassis.pid_wait();

  //Activate the tongue and lift storage to the high goal position
  TonguePiston.set_value(true);
  ChangeScoreState(true);

  //Turn towards the match loader
  chassis.pid_turn_set(-90_deg, TURN_SPEED);
  chassis.pid_wait();

  //Drive into the match loader at 50/127 speed
  chassis.pid_drive_set(-12.375_in, 50, true);
  chassis.pid_wait();
  pros::delay(2000); 
  
  //Drive backwards out of the match loader
  chassis.pid_drive_set(10_in, 127, true);
  chassis.pid_wait();
  //Lift the tongue mechanism once out
  TonguePiston.set_value(false);
  pros::delay(100);

  //Turn to face perpendicular to the perimeter
  chassis.pid_turn_set(0_deg, 70);
  chassis.pid_wait();

  //Drive into the perimeter wall for accurate alignment
  chassis.pid_drive_set(30, 127, true);
  chassis.pid_wait();

  //Reset sensors to start fresh with new alignment
  //Avoids cumulative error and inertial error from hitting the wall
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);

  //Move forward off the wall a small amount for unobstructed turning
  chassis.pid_drive_set(-1, 127, true);
  chassis.pid_wait();

  //Turn parallel to the perimeter
  chassis.pid_turn_set(-90_deg, 70);
  chassis.pid_wait();

  //Drive past goal to the other side
  chassis.pid_drive_set(90, 127, true);
  chassis.pid_wait();

  //Turn perpendicular to the perimeter
  //0 degree refers to facing opposite the perimeter wall as a result of the earlier bearing reset
  chassis.pid_turn_set(0_deg, 70);
  chassis.pid_wait();
  
  //Align along the perimeter by reversing into the wall again
  chassis.pid_drive_set(30, 127, true);
  chassis.pid_wait();

  //Reset sensors to start fresh with new alignment
  //Avoids cumulative odom error and inertial error from hitting the wall
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);

  //Move forward off the wall to align with the goal
  chassis.pid_drive_set(-15, 127, true);
  chassis.pid_wait();

  //Turn to face the goal
  chassis.pid_turn_set(90_deg, 70);
  chassis.pid_wait();

  //Drive into the goal
  //Moves slightly past the goal to ensure the aligner works properly
  chassis.pid_drive_set(15, 127, true);
  chassis.pid_wait();
  
  //Activate lever scoring mechanism
  HoodHook.set_value(false);
  ScoreMotor.move(127);
  pros::delay(1000);
  //Return lever and hood to initial position with a series of delays for sequencing
  ScoreMotor.move(-127);
  pros::delay(2000);  
  HoodHook.set_value(true);

  //Put down the tongue to prepare for match loading
  TonguePiston.set_value(true);
  //Turn to face the match loader
  chassis.pid_turn_set(86_deg, TURN_SPEED);
  chassis.pid_wait();
  //Drive to the match loader
  //Small drive adjustments tuned over time for consistency
  chassis.pid_drive_set(-4, 50, true);
  chassis.pid_wait();
  chassis.pid_drive_set(-25.5, 50, true);
  chassis.pid_wait();
  //Wait for game pieces to be loaded
  pros::delay(2000); 
  
  //Reverse out of the match loader and into the goal
  chassis.pid_drive_set(4, 50, true);
  chassis.pid_wait();
  //Put the  tongue up after moving out
  TonguePiston.set_value(false);
  chassis.pid_drive_set(24, 50, true);
  chassis.pid_wait();

  //Activate lever scoring mechanism and hood
  HoodHook.set_value(false);
  ScoreMotor.move(127);
  //Wait, then return lever and hood to initial position
  pros::delay(5000);
  HoodHook.set_value(true);
  ScoreMotor.move(0);

  //Move forwards out of the goal
  chassis.pid_drive_set(-10, 50, true);
  chassis.pid_wait();

  //Turn perpendicular to the perimeter wall
  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  //Drive into the wall for accurate alignment
  chassis.pid_drive_set(30, 127, true);
  chassis.pid_wait();

  //Reset sensors to start fresh with new alignment
  //Avoids cumulative odom error and inertial error from hitting the wall
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);

  //Move forward off the wall a small amount for unobstructed turning
  chassis.pid_drive_set(-5, 50, true);
  chassis.pid_wait();

  //Turn to be parallel to the long goal
  chassis.pid_turn_set(-90_deg, TURN_SPEED);
  chassis.pid_wait();

  //Drive past the long goal to the other side
  chassis.pid_drive_set(-90_in, 127, true);
  chassis.pid_wait();

  //Turn to point towards the rear corner of the park zone
  chassis.pid_turn_set(160_deg, TURN_SPEED);
  chassis.pid_wait();

  //Drive into the perimeter wall just before the park zone
  chassis.pid_drive_set(50_in, 127, true);
  chassis.pid_wait();

  //Turn along the wall to face the park zone
  chassis.pid_turn_set(180_deg, TURN_SPEED);
  chassis.pid_wait();

  //First drive attempt into the park zone at full force
  //Also serves to clear the game pieces out of the park zone for unobstructed parking
  chassis.pid_drive_set(20_in, 127, true);
  chassis.pid_wait();
  //Reverse for second attempt
  chassis.pid_drive_set(-20_in, 127, true);
  chassis.pid_wait();
  //Final drive into the park zone at full force
  chassis.pid_drive_set(48_in, 127, true);
  chassis.pid_wait();

}

//MARK: OBLOCK

void Oblock() {
  //All movements use skewing for smoother acceleration and deceleration

  //Reset the navigation sensors to 0,0,0 at the start of auton
  chassis.odom_y_flip(true);
  chassis.odom_x_flip(true);
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);

  //Start spinning the intake and intake preload (if used)
  intakeMotor.move(127);

  //Drive to the match loader at 80/127 speed
  chassis.pid_drive_set(35, 80, true);
  chassis.pid_wait();

  //Activate the tongue and lift storage to the high goal position
  TonguePiston.set_value(true);
  ChangeScoreState(true);

  //Turn towards the match loader
  chassis.pid_turn_set(-86_deg, TURN_SPEED);
  chassis.pid_wait();

  //Persist the raised storage state for scoring high goal
  ChangeScoreState(true);

  //Drive into the match loader
  //Intake is already spinning from before
  chassis.pid_drive_set(-13_in, 80, true);
  pros::delay(1000);

  //Persist the raised storage state for scoring high goal
  ChangeScoreState(true);
  
  //Drive backwards out of the match loader and into the goal
  //Moves slightly "past" the goal to ensure the aligner works properly
  chassis.pid_drive_set(28_in, 50, true);
  chassis.pid_wait();
  //Put the tongue up after moving out of the match loader
  TonguePiston.set_value(false);

  //Persist the raised storage state for scoring high goal
  ChangeScoreState(true);

  //Activate lever scoring mechanism and open the hood
  HoodHook.set_value(false);
  ScoreMotor.move(127);
  //Reset lever position after blocks are scored
  pros::delay(1000);
  ScoreMotor.move(-20);

  //Reset sensors to start fresh with new alignment
  //Avoids cumulative inertial error
  //New relative bearing for more coherent coding
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);

  //Turn away from the goal using a swing function at 100/127 speed
  chassis.pid_swing_set(ez::RIGHT_SWING, 135_deg, 100);
  chassis.pid_wait();

  //Turn parallel to the goal, aliging wing with the high goal
  chassis.pid_turn_set(5_deg, TURN_SPEED);
  chassis.pid_wait();

  //Drive forwards into the high goal at full speed to wing the blocks in
  chassis.pid_drive_set(20_in, 127, true);
  chassis.pid_wait();
}

//MARK: OBLOCK MIRROR

void OblockMirror() {
  //All movements use skewing for smoother acceleration and deceleration

  //Reset the navigation sensors to 0,0,0 at the start of auton
  chassis.odom_y_flip(true);
  chassis.odom_x_flip(true);
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);

  //Start spinning the intake and intake preload (if used)
  intakeMotor.move(127);

  //Drive to the match loader at 80/127 speed
  chassis.pid_drive_set(35, 80, true);
  chassis.pid_wait();

   //Activate the tongue and lift storage to the high goal position
  TonguePiston.set_value(true);
  ChangeScoreState(true);

  //Turn towards the match loader
  chassis.pid_turn_set(88_deg, TURN_SPEED);
  chassis.pid_wait();

  //Persist the raised storage state for scoring high goal
  ChangeScoreState(true);

  //Drive into the match loader
  //Intake is already spinning from before
  chassis.pid_drive_set(-13_in, 80, true);
  pros::delay(1000);

  //Persist the raised storage state for scoring high goal
  ChangeScoreState(true);
  
  //Drive backwards out of the match loader and into the goal
  //Moves slightly "past" the goal to ensure the aligner works properly
  chassis.pid_drive_set(28_in, 50, true);
  chassis.pid_wait();
  //Put the tongue up after moving out of the match loader
  TonguePiston.set_value(false);

  //Persist the raised storage state for scoring high goal
  ChangeScoreState(true);

  //Activate lever scoring mechanism and open the hood
  HoodHook.set_value(false);
  ScoreMotor.move(127);
  //Reset lever position after blocks are scored
  pros::delay(1000);
  ScoreMotor.move(-20);

  //Reset sensors to start fresh with new alignment
  //Avoids cumulative inertial error
  //New relative bearing for more coherent coding
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);

  //Turn away from the goal using a swing function at 100/127 speed
  chassis.pid_swing_set(ez::RIGHT_SWING, 135_deg, 100);
  chassis.pid_wait();

  //Turn to align with the goal
  chassis.pid_turn_set(5_deg, TURN_SPEED);
  chassis.pid_wait();

  //Drive forwards to put the wing into the goal
  chassis.pid_drive_set(20_in, 127, true);
  chassis.pid_wait();
}

// MARK: thuckuna
void thuckuna() {
  //All movements use skewing for smoother acceleration and deceleration

  //Reset the navigation sensors to 0,0,0 at the start of auton
  chassis.odom_y_flip(true);
  chassis.odom_x_flip(true);
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);

  //Start spinning the intake and intake preload (if used)
  intakeMotor.move(127);

  //Drive to the match loader at 110/127 speed
  chassis.pid_drive_set(36_in, 110, true);
  chassis.pid_wait();

  //Activate the tongue and prepare the scoring mechanism
  TonguePiston.set_value(1);
  ChangeScoreState(true);

  //Turn towards the match loader
  chassis.pid_turn_set(-88_deg, TURN_SPEED);
  chassis.pid_wait();

  //Drive into the match loader at 80/127 speed
  chassis.pid_drive_set(-13.5_in, 80, true);
  pros::delay(1000);
  
  //Drive to the goal with small tuning adjustments
  chassis.pid_drive_set(5_in, 127, true);
  chassis.pid_wait();
  chassis.pid_drive_set(25_in, 127, true);
  chassis.pid_wait_until(20_in);

  //After 20 inches, activate the scoring mechanism
  ScoreMotor.move(127);
  HoodHook.set_value(false);
  //stop the inktake to prevent under-lever jams
  intakeMotor.move(0);
  
  //Store the tongue, reset the scoring motor, and outtake any jams
  TonguePiston.set_value(0);
  chassis.pid_wait();
  pros::delay(500);
  ScoreMotor.move(-50);
  intakeMotor.move(-127);

  //Reset sensors to start fresh with the new alignment
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);
  pros::delay(250);
  
  //Reverse out of the goal and start spinning the intake
  chassis.pid_drive_set(-15_in, 100, true);
  chassis.pid_wait();
  intakeMotor.move(127);

  //New alignment and relative bearing for simpler navigation
  chassis.odom_xyt_set(0_in, 0_in, -90_deg);

  //Turn towards the collection of center blocks
  //Based on the heading from the new alignment above
  chassis.pid_turn_set(40_deg, 80);
  chassis.pid_wait();

  //Drive forwards to the center goal and to collect blocks
  chassis.pid_drive_set(-51_in, 80, true);
  //After 44 inches, stop intaking and score into the lower goal
  chassis.pid_wait_until(-44_in);
  intakeMotor.move(-127);
  chassis.pid_wait();
  //Wait for game pieces to score
  pros::delay(250);

  ///Reverse out of the lower goal
  chassis.pid_drive_set(16_in, 127, true);
  chassis.pid_wait();
  
  //Turn to face the second center block collection
  chassis.pid_turn_set(-2_deg, 100);
  chassis.pid_wait();

  //Drive to the blocks and intake them
  intakeMotor.move(127);
  chassis.pid_drive_set(-45_in, 127, true);
  chassis.pid_wait_until(-39_in);
  //Put down the tongue to help collect any unintook blocks
  TonguePiston.set_value(1);
  chassis.pid_wait();

  //Turn to face the middle goal and lower storage position
  ChangeScoreState(false);
  chassis.pid_turn_set(-45_deg, 100);
  chassis.pid_wait();
  
  //Drive to the middle goal and score collected blocks
  chassis.pid_drive_set(17_in, 127, true);
  HoodHook.set_value(false);
  //Scoring uses a slower speed to prevent overshooting
  ScoreMotor.move(50);
  chassis.pid_wait();
  pros::delay(250);
  //Reset scoring mechanism and outtake any jams
  ScoreMotor.move(0);
  intakeMotor.move(-127);
  ChangeScoreState(true);
}

// MARK: little saint james
void littlesaintjames() {
  //Reset the navigation sensors to 0,0,0 at the start of the auton
  chassis.odom_y_flip(true);
  chassis.odom_x_flip(true);
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);

  //Begin intaking and drive backwards towards the match loader at 80/127 speed
  intakeMotor.move(127);
  chassis.pid_drive_set(36_in, 110, true);
  chassis.pid_wait();

  //Activate the tongue and change the scoring state to high goal
  TonguePiston.set_value(1);
  ChangeScoreState(true);

  //Turn towards the match loader
  chassis.pid_turn_set(-88_deg, TURN_SPEED);
  chassis.pid_wait();

  //Drive into the match loader at 80/127 speed
  chassis.pid_drive_set(-13.5_in, 80, true);
  pros::delay(1000);
  
  //Drive backwards into the goal
  chassis.pid_drive_set(30_in, 60, true);
  chassis.pid_wait_until(15_in);
  TonguePiston.set_value(0);
  chassis.pid_wait();

  //Activate scoring mechanism and hood
  HoodHook.set_value(false);
  ScoreMotor.move(127);
  pros::delay(500); 
  ScoreMotor.move(-50);

  //Reset the sensors to the new alignment at the goal
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);
  
  //Turn towards blocks using a swing function at 100/127 speed
  chassis.pid_swing_set(ez::RIGHT_SWING, 135_deg, 100);
  chassis.pid_wait();

  //Drive to the blocks, activating the tongue when close enough
  chassis.pid_drive_set(-15_in, 127, true);
  chassis.pid_wait_until(-5_in);
  TonguePiston.set_value(1);
  chassis.pid_wait();

  //Drive backwards away from the blocks
  chassis.pid_drive_set(13_in, 127, true);
  chassis.pid_wait();

  //Turn backwards towards the goal using a swing function at 100/127 speed
  chassis.pid_swing_set(ez::RIGHT_SWING, 0_deg, 100);
  chassis.pid_wait();

  //Ensure alignment with the goal by driving backwards
  chassis.pid_drive_set(5_in, 127, true);
  chassis.pid_wait();

  //Activate scoring mechanism and hood
  HoodHook.set_value(false);
  ScoreMotor.move(127);
  pros::delay(100);
  intakeMotor.move(-127);
  TonguePiston.set_value(0);
  pros::delay(400); 
  ScoreMotor.move(-50);

  //Turn away from the goal using a swing function at 100/127 speed
  chassis.pid_swing_set(ez::RIGHT_SWING, 135_deg, 100);
  chassis.pid_wait();

  //Turn to align with the goal
  chassis.pid_turn_set(5_deg, TURN_SPEED);
  chassis.pid_wait();

  //Drive forwards to put the wing into the goal
  chassis.pid_drive_set(20_in, 127, true);
  chassis.pid_wait();
}

// MARK: little saint james Mirror
void littlesaintjamesMirror() {
  //Reset the navigation sensors to 0,0,0 at the start of the auton
  chassis.odom_y_flip(true);
  chassis.odom_x_flip(true);
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);

  //Begin intaking and drive backwards towards the match loader at 80/127 speed
  intakeMotor.move(127);
  chassis.pid_drive_set(34.5_in, 110, true);
  chassis.pid_wait();

  //Activate the tongue and change the scoring state to high goal
  TonguePiston.set_value(1);
  ChangeScoreState(true);

  //Turn towards the match loader
  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();

  //Drive into the match loader at 80/127 speed
  chassis.pid_drive_set(-13.5_in, 80, true);
  pros::delay(1000);
  
  //Drive backwards into the goal
  chassis.pid_drive_set(30_in, 60, true);
  chassis.pid_wait_until(15_in);
  TonguePiston.set_value(0);
  chassis.pid_wait();

  //Activate scoring mechanism and hood
  HoodHook.set_value(false);
  ScoreMotor.move(127);
  pros::delay(500); 
  ScoreMotor.move(-50);

  //Reset the sensors to the new alignment at the goal
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);
  
  //Turn towards blocks using a swing function at 100/127 speed
  chassis.pid_swing_set(ez::LEFT_SWING, -135_deg, 100);
  chassis.pid_wait();

  //Drive to the blocks, activating the tongue when close enough
  chassis.pid_drive_set(-15_in, 127, true);
  chassis.pid_wait_until(-5_in);
  TonguePiston.set_value(1);
  chassis.pid_wait();

  //Drive backwards away from the blocks
  chassis.pid_drive_set(13_in, 127, true);
  chassis.pid_wait();

  //Turn backwards towards the goal using a swing function at 100/127 speed
  chassis.pid_swing_set(ez::LEFT_SWING, 0_deg, 100);
  chassis.pid_wait();

  //Ensure alignment with the goal by driving backwards
  chassis.pid_drive_set(5_in, 127, true);
  chassis.pid_wait();

  //Activate scoring mechanism and hood
  HoodHook.set_value(false);
  ScoreMotor.move(127);
  pros::delay(100);
  intakeMotor.move(-127);
  TonguePiston.set_value(0);
  pros::delay(400); 
  ScoreMotor.move(-50);

  //Turn away from the goal using a swing function at 100/127 speed
  chassis.pid_swing_set(ez::RIGHT_SWING, 135_deg, 100);
  chassis.pid_wait();

  //Turn to align with the goal
  chassis.pid_turn_set(5_deg, TURN_SPEED);
  chassis.pid_wait();

  //Drive forwards to put the wing into the goal
  chassis.pid_drive_set(20_in, 127, true);
  chassis.pid_wait();
}
