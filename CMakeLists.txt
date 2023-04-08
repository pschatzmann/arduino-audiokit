
option(ESP_COMPONENT "Is IDF Component" ON)

if(ESP_COMPONENT)
    # Logic for IDF Component
    set (SRC_DIR_C 
        "src/audio_driver/es8388" 
        "src/audio_driver/ac101" 
        "src/audio_hal" 
        "src/audio_board/ai_thinker_ac101"  
        "src/audio_board/ai_thinker_es8388_2957"  
        "src/audio_board/ai_thinker_es8388_3478"  
        "src/audio_board/esp32_s2_kaluga_1_v1_2"  
        "src/audio_board/generic_es8311"  
        "src/audio_board/generic_es8388"  
        "src/audio_board/lyrat_mini_v1_1"  
        "src/audio_board/lyrat_v4_2"  
        "src/audio_board/lyrat_v4_3"
    )
    
    idf_component_register(
        SRC_DIRS ${SRC_DIR_C} 
        INCLUDE_DIRS "src"
        REQUIRES driver
    )

    target_compile_definitions(${COMPONENT_LIB} PUBLIC 
        -DAUDIOKIT_BOARD=5
    )

else()
    cmake_minimum_required(VERSION 3.16)

    # set the project name
    project(arduino-audiokit)
    set(CMAKE_CXX_STANDARD 17)

    set(BOARD_SELECTION "5" CACHE STRING "Board Selection")
    set_property(
      CACHE BOARD_SELECTION
      PROPERTY STRINGS
      "1" "2" "3" "4" "5" "6" "7" "10"
    )
    option(AUDIOKIT_ESP32_I2S "Acivate I2S" OFF)
    option(AUDIOKIT_SETUP_SD "Acivate SD" OFF)
    option(AUDIOKIT_FREE_RTOS "Acivate RTOS" OFF)

    # define source files
    file(GLOB_RECURSE SRC_LIST_CPP CONFIGURE_DEPENDS  "${PROJECT_SOURCE_DIR}/src/*.cpp" )
    file(GLOB_RECURSE SRC_LIST_C CONFIGURE_DEPENDS  "${PROJECT_SOURCE_DIR}/src/*.c" )

    # define libraries
    add_library (arduino-audiokit STATIC ${SRC_LIST_CPP} ${SRC_LIST_C})

    # define location for header files
    target_include_directories(arduino-audiokit PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src )
    target_compile_definitions(arduino-audiokit PUBLIC -DUSE_CMAKE -DAUDIOKIT_BOARD=${BOARD_SELECTION} -DAUDIOKIT_ESP32_I2S=${AUDIOKIT_ESP32_I2S} -DAUDIOKIT_SETUP_SD=${AUDIOKIT_SETUP_SD} -DAUDIOKIT_FREE_RTOS=${AUDIOKIT_FREE_RTOS})
endif()