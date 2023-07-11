#include "motor.h"

void drive::attachMotors(int MOTOR_PIN_1, int MOTOR_PIN_2) 
{
    _motor.push_back(mcpwm_controller::Motor(MOTOR_PIN_1, 0));  
    _motor.push_back(mcpwm_controller::Motor(MOTOR_PIN_2, 0));  
};

void drive::forward()
{
    _motor[0].start();
    _motor[1].start();
};

void drive::backward()
{
    //_motor[0].start();
    //_motor[1].start();
};

void drive::stop(){
    _motor[0].stop();
    _motor[1].stop();
};

// TODO: Implementar diferencia de velocidad entre motores
void drive::left()
{
    _motor[0].start(50);
    _motor[1].start(); 
};

void drive::right()
{
    _motor[0].start();
    _motor[1].start(50);
};

// Escucha los paquetes TCP y responde con un movimiento
void drive::listen(){}; //TODO: implementar

// DEPRECATED
/*
// @Param GPIO_PIN: Pin del GPIO
// Crea los objetos motores y lo añade al vector para conducirlos
template<typename T, typename... Args>
typename std::enable_if<std::is_same<T, int>::value>::type // llama a la función solo si los argumentos son de tipo entero
drive::attachMotors(T GPIO_PIN, Args... args) 
{
    // Recursivo: trata el primer argumento, después vuelve a llamarse para los demás
    _motor.push_back(mcpwm_controller::Motor(GPIO_PIN, 0));  
    attachMotors(args...);
};
*/