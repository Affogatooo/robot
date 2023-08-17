#pragma once

#include <vector>
#include <map>
#include <functional>

#include "mcpwm_controller.hpp"
#include "wifi_controller.hpp"

class drive
{
public:
    void listen(wifi::Station &station);

    // TODO: Benchmark esta función
    void attachMotors(mcpwm_controller::Motor leftMotor, mcpwm_controller::Motor rightMotor);
private:
    void start(int speed, int turnrate);
    void stop();

    std::vector<mcpwm_controller::Motor> _motor;
    int speed;
    int turnrate;
};