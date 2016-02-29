#include <stdint.h>
#include "main.h"
#include "settings.h"
#include "led.h"

void mode_cycle(Config *config) {
	static uint8_t hue = 0;
	static unsigned long lastMillis = 0;

	if (millis>lastMillis+config->cycle.delay) {
		setHSV(hue++, config->cycle.saturation, config->cycle.brightness);
		lastMillis = millis;
	}
}
