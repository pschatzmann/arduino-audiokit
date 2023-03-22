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


# define source files
file(GLOB_RECURSE SRC_LIST_CPP CONFIGURE_DEPENDS  "${PROJECT_SOURCE_DIR}/src/*.cpp" )
file(GLOB_RECURSE SRC_LIST_C CONFIGURE_DEPENDS  "${PROJECT_SOURCE_DIR}/src/*.c" )

# define libraries
add_library (arduino-audiokit STATIC ${SRC_LIST_CPP} ${SRC_LIST_C})

# define location for header files
target_include_directories(arduino-audiokit PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src )
target_compile_definitions(arduino-audiokit PUBLIC -DUSE_CMAKE -DAUDIOKIT_BOARD=${BOARD_SELECTION})
