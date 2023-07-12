#include "main.h"

extern "C" void app_main() 
{
    wifi::Station& station = wifi::Station::instance("Redmi 9C", "123456789");
    station.init();
    station.connect_to_tcp("192.168.43.87", 1234);

    while (true)
    {
        station.receive_data();
        station.print_buffer();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
};