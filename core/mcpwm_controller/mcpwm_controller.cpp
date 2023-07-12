#include "mcpwm_controller.hpp"

namespace mcpwm_controller
{
    Motor::Motor(u_int64_t pin, int speed)
        : _PIN{pin}, _speed{speed} 
    {
        _pwm.timer_config = {
            0,
            MCPWM_TIMER_CLK_SRC_DEFAULT,
            10'000'000,
            MCPWM_TIMER_COUNT_MODE_UP,
            10'975, // random number tbh, works with speed*100
        };
        ESP_ERROR_CHECK(mcpwm_new_timer(&_pwm.timer_config, &_pwm.timer));

        _pwm.operator_config = {
            0,
        };
        ESP_ERROR_CHECK(mcpwm_new_operator(&_pwm.operator_config, &_pwm.oper));
        ESP_ERROR_CHECK(mcpwm_operator_connect_timer(_pwm.oper, _pwm.timer));

        _pwm.comparator_config = {
            .flags{.update_cmp_on_tez = true},
        };
        ESP_ERROR_CHECK(mcpwm_new_comparator(_pwm.oper, &_pwm.comparator_config, &_pwm.comparator));

        _pwm.generator_config = {
            static_cast<int>(_PIN), // ¿Por qué le hago casting a esto?
            {0},               // No tengo idea, me dio flojera cambiar el tipo
        };
        ESP_ERROR_CHECK(mcpwm_new_generator(_pwm.oper, &_pwm.generator_config, &_pwm.generator));

        // Define cuándo la señal cambia de estado
        // HIGH
        ESP_ERROR_CHECK(mcpwm_generator_set_actions_on_timer_event(_pwm.generator,
                        MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH),
                        MCPWM_GEN_TIMER_EVENT_ACTION_END()));
        // LOW
        ESP_ERROR_CHECK(mcpwm_generator_set_actions_on_compare_event(_pwm.generator,
                        MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, _pwm.comparator, MCPWM_GEN_ACTION_LOW),
                        MCPWM_GEN_COMPARE_EVENT_ACTION_END()));

        // Iniciar Timer, y el Generador en Low
        ESP_ERROR_CHECK(mcpwm_timer_enable(_pwm.timer));
        ESP_ERROR_CHECK(mcpwm_timer_start_stop(_pwm.timer, MCPWM_TIMER_START_NO_STOP));

        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(_pwm.comparator, 0));
    };

    Motor::~Motor()
    {
        // Destruye los objetos del PWM, no creo que sea necesario
        mcpwm_del_generator(_pwm.generator);
        mcpwm_del_comparator(_pwm.comparator);
        mcpwm_del_operator(_pwm.oper);
        mcpwm_del_timer(_pwm.timer);
    };

    void Motor::setMaxSpeed(int MAX_SPEED)
    {
        _MAX_SPEED = MAX_SPEED;
    };

    void Motor::stop()
    {
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(_pwm.comparator, 0));
    };

    void Motor::start()
    {
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(_pwm.comparator, calculateSpeed(_speed)));
    };

    void Motor::start(int speed)
    {
        _speed = speed;
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(_pwm.comparator, calculateSpeed(_speed)));
    };

    int Motor::calculateSpeed(int speed)
    {
        // Para cualquier número mayor a 100, se considera 100
        if (speed < 100) {
            return (speed * _MAX_SPEED);
        } else{
            return (100 * _MAX_SPEED);
        }
    };
} // mcpwm_controller