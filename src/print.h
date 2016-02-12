#pragma once
#include <avr/pgmspace.h>
#include <stdio.h>

#define print(x) fputs_P(PSTR(x), stdout)
#define println(x) puts_P(PSTR(x))

#define VERBOSE(x) puts("\e[0;34m" x "\e[0m") //blue
#define DEBUG(x)   puts("\e[0;32m" x "\e[0m") //green
#define INFO(x)    puts("\e[0;37m" x "\e[0m") //white
#define WARN(x)    puts("\e[0;33m" x "\e[0m") //yellow
#define ERR(x)     puts("\e[0;31m" x "\e[0m") //red
#define CRIT(x)    puts("\e[1;31m" x "\e[0m") //bright red
