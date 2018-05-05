#include "packet.h"

bool is_equal(Command &left, Command &right) {
    return left.motor_H_G==right.motor_H_G && left.motor_H_D==right.motor_H_D &&
            left.motor_B_G==right.motor_B_G && left.motor_B_D==right.motor_B_D;
}
