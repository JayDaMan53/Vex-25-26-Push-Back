// #MARK: Includes
#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep

#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"
#include "ui/ui.h"
#include "AutoSelect.hpp"
#include <map>
#include <string>

// #include "autos.hpp"

// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// motor groups
pros::MotorGroup leftMotors({4, -5, 6}, pros::MotorGearset::blue); // left motor group - ports 4 (reversed), 5, 6 (reversed)
pros::MotorGroup rightMotors({-1, 2, -3}, pros::MotorGearset::blue); // right motor group - ports 1, 2 (reversed), 3

// Inertial Sensor on port 10
pros::Imu imu(20);

// tracking wheels
// horizontal tracking wheel encoder. Rotation sensor, port 18, not reversed
pros::Rotation horizontalEnc(18);
// vertical tracking wheel encoder. Rotation sensor, port 19, reversed
pros::Rotation verticalEnc(-19);
// horizontal tracking wheel. 2.75" diameter, 5.75" offset, back of the robot (negative)
lemlib::TrackingWheel horizontal(&horizontalEnc, lemlib::Omniwheel::NEW_2, 4);
// vertical tracking wheel. 2.75" diameter, 2.5" offset, left of the robot (negative)
lemlib::TrackingWheel vertical(&verticalEnc, lemlib::Omniwheel::NEW_2, 0);

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, // left motor group
                              &rightMotors, // right motor group
                              10, // 10 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 4" omnis
                              450, // drivetrain rpm is 360
                              2 // horizontal drift is 2. If we had traction wheels, it would have been 8
);

// lateral motion controller
lemlib::ControllerSettings linearController(10, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            3, // derivative gain (kD)
                                            3, // anti windup
                                            1, // small error range, in inches
                                            100, // small error range timeout, in milliseconds
                                            3, // large error range, in inches
                                            500, // large error range timeout, in milliseconds
                                            20 // maximum acceleration (slew)
);

// angular motion controller
lemlib::ControllerSettings angularController(2, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);


// sensors for odometry
lemlib::OdomSensors sensors(&vertical, // vertical tracking wheel
                            nullptr, // vertical tracking wheel 2, set to nullptr as we don't have a second one
                            &horizontal, // horizontal tracking wheel
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);

pros::Motor intakeMotor(7, pros::v5::MotorGears::blue); // intake motor on port 7
pros::Motor ScoreMotor(-8, pros::v5::MotorGears::red); // score motor on port 8

pros::adi::DigitalOut ScoreOuttakePiston('a');
pros::adi::DigitalOut ParkPiston('c');
pros::adi::DigitalOut TonguePiston('b');

bool runningScore = false;
bool doublePark = false;
bool tongueOut = false;
bool scoreStateUp = false;
bool LockMovement = false;

// #MARK: Initialize Function
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {


    init_AutoSelect();
    // pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors


    // the default rate is 50. however, if you need to change the rate, you
    // can do the following.
    // lemlib::bufferedStdout().setRate(...);
    // If you use bluetooth or a wired connection, you will want to have a rate of 10ms

    // for more information on how the formatting for the loggers
    // works, refer to the fmtlib docs

    // thread to for brain screen and position logging
    // pros::Task screenTask([&]() {
    //     while (true) {
    //         // print robot location to the brain screen
    //         pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
    //         pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
    //         pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
    //         // log position telemetry
    //         lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());
    //         // delay to save resources
    //         pros::delay(50);
    //     }
    // });
}

// #MARK: Disabled Function
/**
 * Runs while the robot is disabled
 */
void disabled() {}

// #MARK: Comp Initialize Function
/**
 * runs after initialize if the robot is connected to field control
 */
void competition_initialize() {}



// get a path used for pure pursuit
// this needs to be put outside a function
ASSET(example_txt); // '.' replaced with "_" to make c++ happy


// #MARK: Autonomous Function
/**
 * Runs during auto
 *
 * This is an example autonomous routine which demonstrates a lot of the features LemLib has to offer
 */
void autonomous() {
    // // Move to x: 20 and y: 15, and face heading 90. Timeout set to 4000 ms
    // chassis.moveToPose(20, 15, 90, 4000);
    // // Move to x: 0 and y: 0 and face heading 270, going backwards. Timeout set to 4000ms
    // chassis.moveToPose(0, 0, 270, 4000, {.forwards = false});
    // // cancel the movement after it has traveled 10 inches
    // chassis.waitUntil(10);
    // chassis.cancelMotion();
    // // Turn to face the point x:45, y:-45. Timeout set to 1000
    // // dont turn faster than 60 (out of a maximum of 127)
    // chassis.turnToPoint(45, -45, 1000, {.maxSpeed = 60});
    // // Turn to face a direction of 90º. Timeout set to 1000
    // // will always be faster than 100 (out of a maximum of 127)
    // // also force it to turn clockwise, the long way around
    // chassis.turnToHeading(90, 1000, {.direction = AngularDirection::CW_CLOCKWISE, .minSpeed = 100});
    // // Follow the path in path.txt. Lookahead at 15, Timeout set to 4000
    // // following the path with the back of the robot (forwards = false)
    // // see line 116 to see how to define a path
    // chassis.follow(example_txt, 15, 4000, false);
    // // wait until the chassis has traveled 10 inches. Otherwise the code directly after
    // // the movement will run immediately
    // // Unless its another movement, in which case it will wait
    // chassis.waitUntil(10);
    // pros::lcd::print(4, "Traveled 10 inches during pure pursuit!");
    // // wait until the movement is done
    // chassis.waitUntilDone();
    // pros::lcd::print(4, "pure pursuit finished!");

    // chassis.follow(TestPath_txt, 15, 4000, false);


    // chassis.setBrakeMode(MOTOR_BRAKE_HOLD);
    // chassis.resetLocalPosition();
    // chassis.setPose(0, 0, 0);
    
    // RunSelected();
    // set position to x:0, y:0, heading:0
    chassis.setPose(0, 0, 0);
    // turn to face heading 90 with a very long timeout
    chassis.turnToHeading(90, 100000);
    
    // while (true) {
    //     printf("rot: %.2f\n", imu.get_rotation());
    // }
}

bool GetKey(pros::controller_digital_e_t key, bool NewPress = false, bool Released = false) {
    if (NewPress) {
        return controller.get_digital_new_press(key);
    } else if (Released) {
        return controller.get_digital_new_release(key);
    } else {
        return controller.get_digital(key);
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

// #MARK: Operator Control Function
/**
 * Runs in driver control
 */
void opcontrol() {
    // Driver Functions

    std::map<std::string, pros::controller_digital_e_t> Keybinds = {
        {"Intake", pros::E_CONTROLLER_DIGITAL_R1},
        {"Outtake", pros::E_CONTROLLER_DIGITAL_R2},
        {"ScoreUp", pros::E_CONTROLLER_DIGITAL_L1},
        {"ScoreDown", pros::E_CONTROLLER_DIGITAL_L2},
        {"DoublePark", pros::E_CONTROLLER_DIGITAL_X},
        {"LockMovement+ChangeHeight", pros::E_CONTROLLER_DIGITAL_RIGHT},
        {"CancelDoublePark", pros::E_CONTROLLER_DIGITAL_B},
        {"Tongue", pros::E_CONTROLLER_DIGITAL_Y}
    };
    
    while (true) {
        // get joystick positions
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

        controller.print(0, 0, "X: %d Y: %d", chassis.getPose().x, chassis.getPose().y);

        // move the chassis with curvature drive
        // only when not dp
        if (!LockMovement) {
            chassis.tank(rightY, leftY);
        }

        // controller.print(0, 0, "X: %f", chassis.getPose().x); // x
        // controller.print(1, 0, "Y: %f", chassis.getPose().y); // y
        // controller.print(2, 0, "Theta: %f", chassis.getPose().theta); // heading

        // R1 intake
        // R2 outtake
        // L1 Up-state score
        // L2 Down-state score
        // X Double Park
        // B Cancel Double Park
        // Y Tongue

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

        pros::delay(10);
    }
}
