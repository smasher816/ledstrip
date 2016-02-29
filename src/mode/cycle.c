#include <stdint.h>
#include "main.h"
#include "settings.h"
#include "led.h"

void mode_cycle() {
	static uint8_t hue = 0;
	static unsigned long lastMillis = 0;

	if (millis>lastMillis+preset.config.cycle.delay) {
		setHSV(hue++, preset.config.cycle.saturation, preset.config.cycle.brightness);
		lastMillis = millis;
	}
}
