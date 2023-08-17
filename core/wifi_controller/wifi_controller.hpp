#pragma once

#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

#include <string>
#include <sys/param.h>
#include <sys/socket.h>

#include "netdb.h"
#include "esp_wifi.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "nvs_flash.h"

#define TAGW "WIFI_STATION"
#define CONNECTED true
#define DISCONNECTED false

constexpr int MAX_RETRY             = 100;
constexpr int MAX_BUFFER_SIZE       = 1024;
constexpr int WIFI_CONNECTED_BIT    = BIT0;
constexpr int WIFI_FAIL_BIT         = BIT1;

namespace wifi {
    class Station
    {
    public:
        // Hace que solo se puede crear una instancia de la clase Station
        static Station& instance(const char* AP_SSID, const char* AP_PASSWORD)
        {
            static Station _instance(AP_SSID, AP_PASSWORD);
            return _instance;
        };

        void init();
        void connect_to_tcp(std::string TCP_SERVER_IP, int TCP_SERVER_PORT);
        void send_data(std::string data);

        int speed_m;
        int turnrate_m;

    private:
        Station(const char* AP_SSID, const char* AP_PASSWORD)
            : _AP_SSID{AP_SSID}, _AP_PASSWORD{AP_PASSWORD}
        {   
            esp_err_t ret = nvs_flash_init();
            if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
                ESP_ERROR_CHECK(nvs_flash_erase());
                ret = nvs_flash_init();
            }
            ESP_ERROR_CHECK(ret);
        };

        ~Station()
        {
            close(_socket_id);
            ESP_ERROR_CHECK(esp_wifi_disconnect());
            ESP_ERROR_CHECK(esp_wifi_stop());
            ESP_ERROR_CHECK(esp_wifi_deinit());
        };

        static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data);
        static EventGroupHandle_t _station_wifi_event_group;

        static int _station_retry_num;
        static int _tcp_retry_num;

        std::string _AP_SSID;
        std::string _AP_PASSWORD;
        bool _AP_CONNECTED;

        void tcp_connection_task();
        std::string _tcp_server_ip;
        int _tcp_server_port;
        int _socket_id;
        char _buffer[MAX_BUFFER_SIZE];
        bool _is_tcp_connected;

        Station(const Station&) = delete;               // Deshabilita el constructor por copia
        Station& operator=(const Station&) = delete;    // Deshabilita el operador de asignación
    };
} // wifi