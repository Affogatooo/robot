file(GLOB CPP "src/*.cpp")

set(SOURCES ${CPP})

idf_component_register(SRCS "${SOURCES}"
                       INCLUDE_DIRS "include"
                       )