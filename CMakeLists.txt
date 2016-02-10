cmake_minimum_required(VERSION 3.4)
set(CMAKE_SYSTEM_Name Generic)

project("Ledstrip")

set(MCU "atmega328p")
set(F_CPU "16000000UL")

set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_C_FLAGS "-mmcu=${MCU} -DF_CPU=${F_CPU} -Os")
set(CMAKE_CXX_FLAGS "-mmcu=${MCU} -DF_CPU=${F_CPU} -Os")

add_executable("ledstrip" "test.c")
