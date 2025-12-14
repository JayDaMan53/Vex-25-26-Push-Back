#include "pros/motors.hpp"
#include "pros/adi.hpp"
#include "lemlib/api.hpp"
#include <charconv>
#include <cstdio>
#include <map>

// bool GetKey(pros::controller_digital_e_t key, bool NewPress = false, bool Released = false) {
//     printf("Getting key %d\n", key);
//     printf("ispressed: %d\n", controller.get_digital(key));
//     if (NewPress) {
//         return controller.get_digital_new_press(key);
//     } else if (Released) {
//         return controller.get_digital_new_release(key);
//     } else {
//         return controller.get_digital(key);
//     }
// }

// void ChangeScoreState(bool State) {
//     ScoreOuttakePiston.set_value(State);
// }



// void Score(void* State) {
//     runningScore = true;
//     ChangeScoreState(State);
//     ScoreMotor.move_relative(1080, 100);
//     while (!((ScoreMotor.get_position() < 1075) && (ScoreMotor.get_position() > 1085))) {
// 	    pros::delay(2);
//     }
//     ScoreMotor.move_relative(-1080, 100);
//     while (!((ScoreMotor.get_position() < -5) && (ScoreMotor.get_position() > 5))) {
// 	    pros::delay(2);
//     }
//     runningScore = false;
// }