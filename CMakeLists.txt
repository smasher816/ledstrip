cmake_minimum_required(VERSION 3.4)
set(CMAKE_SYSTEM_Name Generic)

project("Ledstrip")

set(MCU "atmega328p")
set(F_CPU "16000000UL")
set(PROGRAMMER "buspirate")
set(PROGRAMMER_PORT "/dev/buspirate")
set(PROGRAMMER_BAUD "115200")
set(SERIAL_PORT "/dev/usbserial")
set(SERIAL_BAUD "115200")

set(COMPILE_FLAGS "-Os -Wall -Wextra -Werror -fshort-enums")
set(PROGRAMMER_FLAGS "-V" "-xspifreq=7")
include_directories("FastLED")

set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_C_FLAGS "-mmcu=${MCU} -DF_CPU=${F_CPU} -DBAUD=${SERIAL_BAUD} ${COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "-mmcu=${MCU} -DF_CPU=${F_CPU} -DBAUD=${SERIAL_BAUD} ${COMPILE_FLAGS}")
#set(CMAKE_EXE_LINKER_FLAGS "-Wl,-u,vfprintf -lprintf_min")
set(AVRDUDE_FLAGS "-p${MCU}" "-c${PROGRAMMER}" "-P${PROGRAMMER_PORT}" "-b${PROGRAMMER_BAUD}" "${PROGRAMMER_FLAGS}")

file(GLOB_RECURSE SRC_FILES "src/*.c" "src/*.cpp")
add_executable("ledstrip" ${SRC_FILES} "FastLED/hsv2rgb.cpp")
add_custom_command(TARGET "ledstrip" POST_BUILD
	COMMAND avr-size --format=avr --mcu=${MCU} ledstrip
)

add_custom_target(upload
	COMMAND avr-objcopy -O ihex -R .eeprom ledstrip ledstrip.hex
	COMMAND avrdude ${AVRDUDE_FLAGS} -U flash:w:ledstrip.hex:i
	DEPENDS ledstrip
)
add_custom_target(eeprom
	COMMAND avr-objcopy -O ihex -j .eeprom --change-section-lma .eeprom=0 ledstrip ledstrip.eep
	COMMAND avrdude ${AVRDUDE_FLAGS} -U eeprom:w:ledstrip.eep:i
	DEPENDS ledstrip
)
add_custom_target(fuses
	COMMAND avrdude ${AVRDUDE_FLAGS} -U efuse:w:0x05:m -U hfuse:w:0xD6:m -U lfuse:w:0xFF:m
)
add_custom_target(term
	COMMAND screen ${SERIAL_PORT} ${SERIAL_BAUD}
)
