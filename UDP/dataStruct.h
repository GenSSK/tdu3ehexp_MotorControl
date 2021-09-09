//
// Created by Genki Sasaki on 2020/11/25.
//

#ifndef MBED_DEV_DATASTRUCT_H
#define MBED_DEV_DATASTRUCT_H

struct toMbed{
    float ch1 = 0.0;
    float ch2 = 0.0;
    float ch3 = 0.0;
    float ch4 = 0.0;
    float led1 = 0.0;
    float led2 = 0.0;
    float led3 = 0.0;
    float led4 = 0.0;
    float led5 = 0.0;
    float led6 = 0.0;
    float led7 = 0.0;
    bool servo1 = false;
    bool servo2 = false;
    int checkCount = 0;
    int returnCount = 0;
};

struct toPC{
    int tim1_pulse = 0;
    int tim3_pulse = 0;
    int tim4_pulse = 0;
    int tim8_pulse = 0;
    int checkCount = 0;
    int returnCount = 0;
    int mbedCheck = 0;
};
#endif //MBED_DEV_DATASTRUCT_H