cmake_minimum_required(VERSION 3.0)
project(tdlist)

set(SRC src/main.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

if(NOT EXISTS "/usr/local/include/termcolor.hpp")
    file(DOWNLOAD "https://raw.githubusercontent.com/ikalnytskyi/termcolor/master/include/termcolor/termcolor.hpp" "/usr/local/include/termcolor.hpp")
endif()

include_directories(include /usr/local/include/)
add_executable(tdlist ${SRC})

install(TARGETS tdlist DESTINATION bin)
