#ifndef PACKET_H
#define PACKET_H

struct Command {
    union {
        char Bytes[8];
        struct {
            unsigned short motor_H_G;
            unsigned short motor_H_D;
            unsigned short motor_B_G;
            unsigned short motor_B_D;
        };
    };
};

bool is_equal(Command &left, Command &right);

/*
class Command {
public:
    unsigned short motor_H_G;
    unsigned short motor_H_D;
    unsigned short motor_B_G;
    unsigned short motor_B_D;

    char bytes[8];

    Command() {
        motor_H_G = 0;
        motor_H_D = 0;
        motor_B_G = 0;
        motor_B_D = 0;
    }

    char* data() {
        bytes[0] = motor_H_G & 0xff;
        bytes[1] = (motor_H_G >> 8) & 0xff;

        bytes[2] = motor_H_D & 0xff;
        bytes[3] = (motor_H_D >> 8) & 0xff;

        bytes[4] = motor_B_G & 0xff;
        bytes[5] = (motor_B_G >> 8) & 0xff;

        bytes[6] = motor_B_D & 0xff;
        bytes[7] = (motor_B_D >> 8) & 0xff;


        return bytes;
    }

    inline bool operator==(Command &other) {
        return motor_H_G==other.motor_H_G && motor_H_D==other.motor_H_D &&
                motor_B_G==other.motor_B_G && motor_B_D==other.motor_B_D;
    }
};
*/

#endif // PACKET_H
