#include <stdint.h>
#include "main.h"
#include "settings.h"
#include "led.h"

void mode_static() {
	setHSV(preset.config.hsv.hue, preset.config.hsv.saturation, preset.config.hsv.value);
}
