#pragma once

// Ignorar warnings de inicialización de structs
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

#include <string>

#include "esp_wifi.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "nvs_flash.h"

#define TAG "WIFI_STATION"
#define MAX_RETRY 3

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

namespace wifi {
    class station
    {
    public:
        /** 
         * @brief Constructor de la clase station
         * @param AP_SSID nombre de la red a la que se conectará
         * @param AP_PASSWORD contraseña de la red
        **/
        station(const char* AP_SSID, const char* AP_PASSWORD)
            : _AP_SSID{AP_SSID}, _AP_PASSWORD{AP_PASSWORD}
        {   
            esp_err_t ret = nvs_flash_init();
            if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
                ESP_ERROR_CHECK(nvs_flash_erase());
                ret = nvs_flash_init();
            }
            ESP_ERROR_CHECK(ret);
        };

        void wifi_init();

    private:
        static EventGroupHandle_t station_wifi_event_group;
        static int station_retry_num;
        static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data);

        std::string _AP_SSID;
        std::string _AP_PASSWORD;
    };
} // wifi