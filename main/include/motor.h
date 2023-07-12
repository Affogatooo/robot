#pragma once

#include <vector>
#include <bitset>
#include <map>
#include <functional>

#include "mcpwm_controller.hpp"

class drive
{
public:
    drive();
    ~drive();
    /* void forward();
    void backward(); // TODO: implementar el H bridge
    void left();
    void right();
    void stop();
    */
    void listen();

    // TODO: Benchmark esta función
    void attachMotors(int MOTOR_PIN_1, int MOTOR_PIN_2);
private:
    void forward();
    void backward(); // TODO: implementar el H bridge
    void left();
    void right();
    void stop();

    std::vector<mcpwm_controller::Motor> _motor;
    std::bitset<3> _flags;
};