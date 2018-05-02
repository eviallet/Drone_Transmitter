#ifndef PACKET_H
#define PACKET_H

union Command {
    struct Values {
        unsigned short motor_B_G;
        unsigned short motor_H_G;
        unsigned short motor_B_D;
        unsigned short motor_H_D;
    } Values;
    char Bytes[sizeof(Values)];
};


#endif // PACKET_H
