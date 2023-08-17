#include "main.h"
#include "mcpwm_controller.hpp"


esp_err_t setup()
{
    /*wifi::Station& station = wifi::Station::instance("ssid", "n&79008M");
    station.init();
    station.connect_to_tcp("192.168.0.105", 1234);
    */
    return ESP_OK;

};
   

extern "C" void app_main() 
{
    mcpwm_controller::Motor motor(18, 0);
    
    while (true) {
        for (int i = 0; i == 100; ++i){
            motor.start(i);
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
};