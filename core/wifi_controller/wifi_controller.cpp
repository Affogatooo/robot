#include "wifi_controller.hpp"

namespace wifi {
    // Inicializa las variables estáticas
    EventGroupHandle_t Station::station_wifi_event_group = xEventGroupCreate();
    int Station::station_retry_num = 0;

    void Station::event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
    {
        if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
            esp_wifi_connect();
        } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
            if (station_retry_num < MAX_RETRY) {
                esp_wifi_connect();
                station_retry_num++;
                ESP_LOGI(TAG, "Reintentando conexion al ACCESS POINT");
            } else {
                xEventGroupSetBits(station_wifi_event_group, WIFI_FAIL_BIT);
            }
            ESP_LOGI(TAG,"Fallo la conexion al ACCESS POINT");
        } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
            ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
            ESP_LOGI(TAG, "Se consiguio una IP:" IPSTR, IP2STR(&event->ip_info.ip));
            station_retry_num = 0;
            xEventGroupSetBits(station_wifi_event_group, WIFI_CONNECTED_BIT);
        }
    };

    void Station::init()
    {

        ESP_ERROR_CHECK(esp_netif_init());

        ESP_ERROR_CHECK(esp_event_loop_create_default());
        esp_netif_create_default_wifi_sta();

        wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK(esp_wifi_init(&config));

        esp_event_handler_instance_t instance_any_id;
        esp_event_handler_instance_t instance_got_ip;
        ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                            ESP_EVENT_ANY_ID,
                                                            &event_handler,
                                                            NULL,
                                                            &instance_any_id));
        ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                            IP_EVENT_STA_GOT_IP,
                                                            &event_handler,
                                                            NULL,
                                                            &instance_got_ip));

        wifi_config_t wifi_config = {
            .sta = {
                .ssid = {0},
                .password = {0},
                .threshold = {.authmode = WIFI_AUTH_WPA2_PSK},
            },
        };
        std::copy(_AP_SSID.begin(), _AP_SSID.end(), reinterpret_cast<char*>(wifi_config.sta.ssid));
        std::copy(_AP_PASSWORD.begin(), _AP_PASSWORD.end(), reinterpret_cast<char*>(wifi_config.sta.password));

        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
        ESP_ERROR_CHECK(esp_wifi_start());

        EventBits_t bits = xEventGroupWaitBits(station_wifi_event_group,
                WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                pdFALSE,
                pdFALSE,
                portMAX_DELAY);

        if (bits & WIFI_CONNECTED_BIT) {
            ESP_LOGI(TAG, "Conectado al ACCESS POINT SSID:%s password:%s",
                    _AP_SSID.c_str(), _AP_PASSWORD.c_str());
        } else if (bits & WIFI_FAIL_BIT) {
            ESP_LOGI(TAG, "Fallo la conexión al ACCESS POINT SSID:%s, password:%s",
                    _AP_SSID.c_str(), _AP_PASSWORD.c_str());
        } else {
            ESP_LOGE(TAG, "UNEXPECTED EVENT");
        }

        vEventGroupDelete(station_wifi_event_group);
    };

    void Station::connect_to_tcp(std::string TCP_SERVER_IP, int TCP_SERVER_PORT)
    {        
        _socket_id = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
        // Crea el socket
        if (_socket_id < 0) {
            ESP_LOGE("TCP", "Fallo la creacion del socket. Error code: %d", errno);
            return;
        }

        // Configura la dirección del servidor
        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(TCP_SERVER_PORT);
        if (inet_pton(AF_INET, TCP_SERVER_IP.c_str(), &server_addr.sin_addr) <= 0) {
            ESP_LOGE("TCP", "Direccion del servidor inválida. Error code: %d", errno);
            close(_socket_id);
            return;
        }

        // Conecta al socket
        if (connect(_socket_id, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            ESP_LOGE("TCP", "Fallo la conexion al servidor. Error code: %d", errno);
            close(_socket_id);
            return;
        }

        // Conexión exitosa
        ESP_LOGI("TCP", "Conectado al servidor TCP");

        // Esuchar al servidor
        const char *message = "Hola desde el ESP32!";
        if (send(_socket_id, message, strlen(message), 0) < 0) {
            ESP_LOGE("TCP", "No se pudo enviar el mensaje al servidor. Error code: %d", errno);
            close(_socket_id);
            return;
        }
    };

    void Station::send_data(std::string data)
    {
        if (send(_socket_id, data.c_str(), data.length(), 0) < 0) {
            ESP_LOGE("TCP", "No se pudo enviar el mensaje al servidor. Error code: %d", errno);
            close(_socket_id);
            return;
        }
    };

    void Station::receive_data()
    { 
        int received_bytes = recv(_socket_id, _buffer, sizeof(_buffer) - 1, 0);
        if (received_bytes <= 0) {
            return;
        }

        _buffer[received_bytes] = '\0';

        ESP_LOGI("TCP", "Server: %s", _buffer);
    };

    void Station::print_buffer()
    {
        ESP_LOGW("BUFFER", "DATA WRITTEN INTO THE BUFFER: %s", _buffer);
    };
} // wifi