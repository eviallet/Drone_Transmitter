#include "packet.h"

bool is_equal(Command &left, Command &right) {
    return left.speed==right.speed && left.yaw==right.yaw &&
            left.pitch==right.pitch && left.roll==right.roll;
}
