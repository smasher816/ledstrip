rm -rf build 2>/dev/null
mkdir build
cd build
cmake ..
make fuses
make eeprom
make upload
