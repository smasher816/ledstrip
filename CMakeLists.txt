cmake_minimum_required(VERSION 3.4)
set(CMAKE_SYSTEM_Name Generic)

project("Ledstrip")

set(MCU "atmega328p")
set(F_CPU "16000000UL")
set(PROGRAMMER "buspirate")
set(PORT "/dev/buspirate")
set(BAUDRATE "115200")

set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_C_FLAGS "-mmcu=${MCU} -DF_CPU=${F_CPU} -Os")
set(CMAKE_CXX_FLAGS "-mmcu=${MCU} -DF_CPU=${F_CPU} -Os")

add_executable("ledstrip" "test.c")
add_custom_command(TARGET "ledstrip" POST_BUILD
	COMMAND avr-objcopy -O ihex -R .eeprom ledstrip ledstrip.hex
)

add_custom_target(upload
	COMMAND avrdude -p ${MCU} -c ${PROGRAMMER} -P ${PORT} -b ${BAUDRATE} -U flash:w:ledstrip.hex:i
	DEPENDS ledstrip
)
