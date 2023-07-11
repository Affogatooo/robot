#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "wifi_controller.hpp"

extern "C" void app_main() 
{
    wifi::station station("ssid", "n&79008M");
    station.wifi_init();
};