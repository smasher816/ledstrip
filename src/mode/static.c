#include <stdint.h>
#include "main.h"
#include "settings.h"
#include "led.h"

void mode_static(Config *config) {
	setHSV(config->hsv.hue, config->hsv.saturation, config->hsv.value);
}
