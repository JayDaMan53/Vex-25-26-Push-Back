#include "main.h"
#include <string.h>
#include <sys/_intsup.h>
#include <cstdio>
#include <string>
#include "EZ-Template/sdcard.hpp"
#include "autons.hpp"
#include "liblvgl/core/lv_disp.h"
#include "liblvgl/core/lv_obj.h"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/optical.hpp"

// #include "AutoSelect.cpp"
#include "ui/ui.h"

#include "AutoSelect.hpp"

/////
// For installation, upgrading, documentations, and tutorials, check out our website!
// https://ez-robotics.github.io/EZ-Template/
/////

pros::Controller master (CONTROLLER_MASTER);

pros::Motor intakeMotor(7, pros::v5::MotorGears::blue); // intake motor on port 7
pros::Motor ScoreMotor(-8, pros::v5::MotorGears::red); // score motor on port 8

pros::adi::DigitalOut ScoreOuttakePiston('a');
pros::adi::DigitalOut ParkPiston('c');
pros::adi::DigitalOut TonguePiston('b');
pros::adi::DigitalOut HoodHook('d');

// Chassis constructor
ez::Drive chassis(
    // These are your drive motors, the first motor is used for sensing!
    {-4,5,-6},     // Left Chassis Ports (negative port will reverse it!)
    {1,-2,3},  // Right Chassis Ports (negative port will reverse it!)

    20,      // IMU Port
    3.25,  // Wheel Diameter (Remember, 4" wheels without screw holes are actually 4.125!)
    450);   // Wheel RPM = cartridge * (motor gear / wheel gear)

// Uncomment the trackers you're using here!
// - `8` and `9` are smart ports (making these negative will reverse the sensor)
//  - you should get positive values on the encoders going FORWARD and RIGHT
// - `2.75` is the wheel diameter
// - `4.0` is the distance from the center of the wheel to the center of the robot
ez::tracking_wheel horiz_tracker(18, 2, -2);  // This tracking wheel is perpendicular to the drive wheels
ez::tracking_wheel vert_tracker(19, 2, 0);   // This tracking wheel is parallel to the drive wheels

struct AutonList {
  std::string name;
  std::function<void()> func;
};
std::vector<AutonList> AutonSelect;
std::string selectedAuton;

bool runningScore = false;
bool doublePark = false;
bool tongueOut = false;
bool scoreStateUp = false;
bool LockMovement = false;
bool HoodHookState = false;


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  // Print our branding over your terminal :D
  ez::ez_template_print();

  pros::delay(500);  // Stop the user from doing anything while legacy ports configure

  // Look at your horizontal tracking wheel and decide if it's in front of the midline of your robot or behind it
  //  - change `back` to `front` if the tracking wheel is in front of the midline
  //  - ignore this if you aren't using a horizontal tracker
  // chassis.odom_tracker_back_set(&horiz_tracker);
  // Look at your vertical tracking wheel and decide if it's to the left or right of the center of the robot
  //  - change `left` to `right` if the tracking wheel is to the right of the centerline
  //  - ignore this if you aren't using a vertical tracker
  // chassis.odom_tracker_left_set(&vert_tracker);

  // Configure your chassis controls
  chassis.opcontrol_curve_buttons_toggle(true);   // Enables modifying the controller curve with buttons on the joysticks
  chassis.opcontrol_drive_activebrake_set(0.0);   // Sets the active brake kP. We recommend ~2.  0 will disable.
  chassis.opcontrol_curve_default_set(0.0, 0.0);  // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)

  // Set the drive to your own constants from autons.cpp!
  default_constants();

  // These are already defaulted to these buttons, but you can change the left/right curve buttons here!
  // chassis.opcontrol_curve_buttons_left_set(pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT);  // If using tank, only the left side is used.
  // chassis.opcontrol_curve_buttons_right_set(pros::E_CONTROLLER_DIGITAL_Y, pros::E_CONTROLLER_DIGITAL_A);

  // Initialize chassis and auton selector
  chassis.initialize();
  // ez::as::initialize();
  init_AutoSelect();

  master.rumble(chassis.drive_imu_calibrated() ? "." : "---");

  chassis.pid_tuner_disable();

  ParkPiston.set_value(true);

  // ColorSensor.set_led_pwm(100);
  // ColorSensor2.set_led_pwm(100);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
  // . . .
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
  IsAMatch = true;
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
  chassis.pid_targets_reset();                // Resets PID targets to 0
  chassis.drive_imu_reset();                  // Reset gyro position to 0
  chassis.drive_sensor_reset();               // Reset drive sensors to 0
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);    // Set the current position, you can start at a specific position with this
  chassis.drive_brake_set(MOTOR_BRAKE_HOLD);  // Set motors to hold.  This helps autonomous consistency

  /*
  Odometry and Pure Pursuit are not magic

  It is possible to get perfectly consistent results without tracking wheels,
  but it is also possible to have extremely inconsistent results without tracking wheels.
  When you don't use tracking wheels, you need to:
   - avoid wheel slip
   - avoid wheelies
   - avoid throwing momentum around (super harsh turns, like in the example below)
  You can do cool curved motions, but you have to give your robot the best chance
  to be consistent
  */

  // if (globalGif != nullptr) {
  //   renderGif();
  // }

  //ez::as::auton_selector.selected_auton_call();  // Calls selected auton from autonomous selector

  // AutoSelect_tick();

  // currentScreen = -1 - 1;
  // create_screen_main();
  // lv_obj_t *screen = objects.main;
  // printf("Object status: %s\n", (screen == NULL) ? "NULL" : "Not NULL");

  // lv_disp_load_scr(screen);
  // lv_scr_load_anim(screen, LV_SCR_LOAD_ANIM_FADE_IN, 200, 0, false);

  // RunSelected();
  test();
  // RedLeft();

  // RedLeft(false);
  // PureTest();

  // for (int i = 0; i < AutonSelect.size(); i++) {
  //   if (AutonSelect[i].name == selectedAuton) {
  //     AutonSelect[i].func();
  //   }
  // }
}

/**
 * Simplifies printing tracker values to the brain screen
 */
void screen_print_tracker(ez::tracking_wheel *tracker, std::string name, int line) {
  std::string tracker_value = "", tracker_width = "";
  // Check if the tracker exists
  if (tracker != nullptr) {
    tracker_value = name + " tracker: " + util::to_string_with_precision(tracker->get());             // Make text for the tracker value
    tracker_width = "  width: " + util::to_string_with_precision(tracker->distance_to_center_get());  // Make text for the distance to center
  }
  ez::screen_print(tracker_value + tracker_width, line);  // Print final tracker text
}

/**
 * Ez screen task
 * Adding new pages here will let you view them during user control or autonomous
 * and will help you debug problems you're having
 */
void ez_screen_task() {
  while (true) {
    // Only run this when not connected to a competition switch
    if (!pros::competition::is_connected()) {
      // Blank page for odom debugging
      if (chassis.odom_enabled() && !chassis.pid_tuner_enabled()) {
        // If we're on the first blank page...
        if (ez::as::page_blank_is_on(0)) {
          // Display X, Y, and Theta
          ez::screen_print("x: " + util::to_string_with_precision(chassis.odom_x_get()) +
                               "\ny: " + util::to_string_with_precision(chassis.odom_y_get()) +
                               "\na: " + util::to_string_with_precision(chassis.odom_theta_get()),
                           1);  // Don't override the top Page line

          // Display all trackers that are being used
          screen_print_tracker(chassis.odom_tracker_left, "l", 4);
          screen_print_tracker(chassis.odom_tracker_right, "r", 5);
          screen_print_tracker(chassis.odom_tracker_back, "b", 6);
          screen_print_tracker(chassis.odom_tracker_front, "f", 7);
        }
      }
    }

    // Remove all blank pages when connected to a comp switch
    else {
      if (ez::as::page_blank_amount() > 0)
        ez::as::page_blank_remove_all();
    }

    pros::delay(ez::util::DELAY_TIME);
  }
}
pros::Task ezScreenTask(ez_screen_task);

/**
 * Gives you some extras to run in your opcontrol:
 * - run your autonomous routine in opcontrol by pressing DOWN and B
 *   - to prevent this from accidentally happening at a competition, this
 *     is only enabled when you're not connected to competition control.
 * - gives you a GUI to change your PID values live by pressing X
 */
void ez_template_extras() {
  // Only run this when not connected to a competition switch
  if (!pros::competition::is_connected()) {
    // PID Tuner
    // - after you find values that you're happy with, you'll have to set them in auton.cpp

    // Enable / Disable PID Tuner
    //  When enabled:
    //  * use A and Y to increment / decrement the constants
    //  * use the arrow keys to navigate the constants
    if (master.get_digital_new_press(DIGITAL_X))
      chassis.pid_tuner_toggle();

    // Trigger the selected autonomous routine
    if (master.get_digital(DIGITAL_B) && master.get_digital(DIGITAL_DOWN)) {
      pros::motor_brake_mode_e_t preference = chassis.drive_brake_get();
      autonomous();
      chassis.drive_brake_set(preference);
    }

    // Allow PID Tuner to iterate
    chassis.pid_tuner_iterate();
  }

  // Disable PID Tuner when connected to a comp switch
  else {
    if (chassis.pid_tuner_enabled())
      chassis.pid_tuner_disable();
    }
}

bool GetKey(pros::controller_digital_e_t key, bool NewPress = false) {
    if (NewPress) {
      return master.get_digital_new_press(key);
    } else {
      return master.get_digital(key);
    }
}

void ChangeScoreState(bool State) {
    scoreStateUp = State;
    ScoreOuttakePiston.set_value(State);
}

void Score(void* State) {
    runningScore = true;
    ChangeScoreState(State);
    ScoreMotor.move_relative(1180, State ? 100 : 50);
    // printf("Score Motor Pos: %f\n", ScoreMotor.get_actual_velocity());
    pros::delay(200);
    while (ScoreMotor.get_actual_velocity() != 0) {
        // printf("Score Motor Pos: %f\n", ScoreMotor.get_actual_velocity());
	    pros::delay(2);
    }
    pros::delay(500);
    ScoreMotor.move_relative(-1180, 100);
    // printf("Score Motor Pos: %f\n", ScoreMotor.get_actual_velocity());
    pros::delay(200);
    while (ScoreMotor.get_actual_velocity() != 0) {
        // printf("Score Motor Pos: %f\n", ScoreMotor.get_actual_velocity());
	    pros::delay(2);
    }
    printf("Score Motor Pos: %f\n", ScoreMotor.get_actual_velocity());
    runningScore = false;
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
  // This is preference to what you like to drive on
  chassis.drive_brake_set(MOTOR_BRAKE_COAST);

  // if (IsAMatch) {
  //   RunMatch();
  // }

  std::map<std::string, pros::controller_digital_e_t> Keybinds = {
        {"Intake", pros::E_CONTROLLER_DIGITAL_R1},
        {"Outtake", pros::E_CONTROLLER_DIGITAL_R2},
        {"ScoreUp", pros::E_CONTROLLER_DIGITAL_L1},
        {"ScoreDown", pros::E_CONTROLLER_DIGITAL_L2},
        {"DoublePark", pros::E_CONTROLLER_DIGITAL_X},
        {"LockMovement+ChangeHeight", pros::E_CONTROLLER_DIGITAL_RIGHT},
        {"CancelDoublePark", pros::E_CONTROLLER_DIGITAL_B},
        {"Tongue", pros::E_CONTROLLER_DIGITAL_Y},
        {"HoodHook", pros::E_CONTROLLER_DIGITAL_UP}
  };

  while (true) {
    // enableColorSort = false;

    // Gives you some extras to make EZ-Template ezier
    // ez_template_extras();

    if (!LockMovement) {
      chassis.opcontrol_tank();  // Tank control
    }

    // chassis.opcontrol_arcade_standard(ez::SPLIT);   // Standard split arcade
    // chassis.opcontrol_arcade_standard(ez::SINGLE);  // Standard single arcade
    // chassis.opcontrol_arcade_flipped(ez::SPLIT);    // Flipped split arcade
    // chassis.opcontrol_arcade_flipped(ez::SINGLE);   // Flipped single arcade

    // Scoring
    if (GetKey(Keybinds["ScoreUp"], true) || GetKey(Keybinds["ScoreDown"], true)) {
        if (!runningScore) {
            if (GetKey(Keybinds["ScoreUp"], false)) {
                scoreStateUp = true;
            } else if (GetKey(Keybinds["ScoreDown"], false)) {
                scoreStateUp = false;
            }
            
            pros::Task ScoreThread(Score, (void*)scoreStateUp);
        }
    }


    if (!runningScore) { // constantly move down
        ScoreMotor.move(-25);
    }

    // Intaking / Outtaking out front
    if (GetKey(Keybinds["Intake"])) {
        intakeMotor.move(127);
    } else if (GetKey(Keybinds["Outtake"])) {
        intakeMotor.move(-127);
    } else {
        intakeMotor.move(0);
    }

    // Double Park
    if (GetKey(Keybinds["DoublePark"], true)) {
        doublePark = true;
        ParkPiston.set_value(doublePark);
    }

    bool LMCH = GetKey(Keybinds["LockMovement+ChangeHeight"], true);
    
    if (LMCH && doublePark) {
        LockMovement = true;
    } else if (GetKey(Keybinds["CancelDoublePark"], true) && doublePark) {
        doublePark = false;
        LockMovement = false;
        ParkPiston.set_value(doublePark);
    } else if (LMCH && !doublePark) {
        scoreStateUp = !scoreStateUp;
        ChangeScoreState(scoreStateUp);
    }

    // Tongue
    if (GetKey(Keybinds["Tongue"], true)) {
        tongueOut = !tongueOut;
        TonguePiston.set_value(tongueOut);
    }

    if (GetKey(Keybinds["HoodHook"], true)) {
        HoodHookState = !HoodHookState;
        HoodHook.set_value(HoodHookState);
    }

    pros::delay(ez::util::DELAY_TIME);  // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
  }
}