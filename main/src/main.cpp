#include "main.h"

extern "C" void app_main() 
{
    wifi::Station& station = wifi::Station::instance("ssid", "n&79008M");
    station.init();
    station.connect_to_tcp("192.168.0.31", 1234);

    while (true)
    {
        station.receive_data();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
};