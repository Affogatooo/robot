#pragma once
#include "driver/mcpwm_prelude.h"

#pragma GCC diagnostic ignored "-Wmissing-field-initializers" 

namespace mcpwm_controller {
    struct pwm_config
    {
        mcpwm_timer_config_t timer_config;
        mcpwm_timer_handle_t timer;
        mcpwm_operator_config_t operator_config;
        mcpwm_oper_handle_t oper;
        mcpwm_comparator_config_t comparator_config;
        mcpwm_cmpr_handle_t comparator;
        mcpwm_generator_config_t generator_config;
        mcpwm_gen_handle_t generator;
    };
    
    class Motor
    {
    public:
        Motor(u_int64_t pin, int speed);
        ~Motor();
        
        void setMaxSpeed(int MAX_SPEED);
        void stop();
        void start();
        void start(int speed);

    private:
        int calculateSpeed(int speed);
        
        // TODO: Implementar dirección
        u_int64_t _pin;
        int _speed = 0;
        pwm_config _pwm;
        int _MAX_SPEED = 100;
    };
} // mcpwm_controller