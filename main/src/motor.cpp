#include "motor.h"
#include <algorithm>

void drive::attachMotors(mcpwm_controller::Motor leftMotor, mcpwm_controller::Motor rightMotor) 
{
    _motor.push_back(leftMotor);  
    _motor.push_back(rightMotor);  
};

// Escucha los paquetes TCP y responde con un movimiento
void drive::listen(wifi::Station &station)
{
    drive::start(station.speed_m, station.turnrate_m);
};

void drive::start(int speed, int turnrate)
{
    int leftMotorSpeed = std::max(0, std::min(100, speed + turnrate));
    int rightMotorSpeed = std::max(0, std::min(100, speed - turnrate));

    _motor[0].start(leftMotorSpeed);
    _motor[1].start(rightMotorSpeed);
}

// DEPRECATED
/*
template<typename T, typename... Args>
typename std::enable_if<std::is_same<T, int>::value>::type // llama a la función solo si los argumentos son de tipo entero
drive::attachMotors(T GPIO_PIN, Args... args) 
{
    // Recursivo: trata el primer argumento, después vuelve a llamarse para los demás
    _motor.push_back(mcpwm_controller::Motor(GPIO_PIN, 0));  
    attachMotors(args...);
};
*/