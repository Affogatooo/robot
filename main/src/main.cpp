#include "main.h"
#include "mcpwm_controller.hpp"
#include "wifi_controller.hpp"
#include "motor.h"

int calc(int speed){
    if (speed >= 100) {
        speed = 100;
    }
    else if (speed <= 0) {
        speed = 0;
    }
    return speed;
}

extern "C" void app_main() 
{
    wifi::Station& station = wifi::Station::instance("INFINITUMAC7C_2.4", "0030012013");
    station.init();
    station.connect_to_tcp("192.168.1.67", 12345);

    mcpwm_controller::Motor leftMotor(18, 0);
    mcpwm_controller::Motor rightMotor(19, 0);

    drive wheels;
    wheels.attachMotors(leftMotor, rightMotor);
    
    while (true) {
        wheels.listen(station);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
};