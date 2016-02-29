#include <stdint.h>
#include "main.h"
#include "settings.h"
#include "led.h"

#define FPS2MS(x) (1000/x)

void mode_music(Config *config) {
	static unsigned long lastMillis = 0;
	if (millis>lastMillis+FPS2MS(config->music.fps)) {
		msgeq7_read();

		uint16_t vol =f[config->music.frequency][CHANNEL_LEFT];
		if (vol <= settings.msgeq7_min)
			vol = 0;

		vol = config->music.min_brightness + vol*(config->music.sensitivity/100.0);
		if (vol > 255)
			vol = 255;

		setBrightness(vol);
		lastMillis = millis;
	}

}
