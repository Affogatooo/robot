file(GLOB CPP "./**/*.cpp")

set(SOURCES ${CPP})

idf_component_register(SRCS ${SOURCES}
                       INCLUDE_DIRS mcpwm_controller wifi_controller
                       REQUIRES driver esp_wifi nvs_flash
                       )