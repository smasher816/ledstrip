#include <stdint.h>
#include "main.h"
#include "settings.h"
#include "led.h"

#include "print.h"
#include "term.h"

#define FPS2MS(x) (1000/x)
#define SQUARE(x) ((x)*(x))

int isBeat(uint8_t freq) {
	#define BLOCK_SIZE 32
	#define BLOCK_COUNT 32
	static int sample = 0;
	static int block = 0;
	static unsigned long energy;
	static unsigned long E[FREQ_COUNT][BLOCK_COUNT] = {0};
	static uint8_t isPeak = 0;
	double C = 1.4;

	//for (int freq=0; freq<FREQ_COUNT; freq++) {
		sample++;
		if (sample<BLOCK_SIZE) {
			uint16_t vol = f[freq][CHANNEL_LEFT];
			if (vol <= (settings.msgeq7_min+10u))
				vol = 0;
			energy += SQUARE(vol); // + SQUARE(vol2);
		} else {
			sample = 0;

			block++;
			if (block>=BLOCK_COUNT) block=0;
			E[freq][block] = energy;

			unsigned long avg = 0;
			for (int b=0; b<BLOCK_COUNT; b++) {
				avg += E[freq][b];
			}
			avg /= BLOCK_COUNT;

			/*unsigned long var = 0;
			for (int b=0; b<BLOCK_COUNT; b++) {
				long diff = avg - energy;
				var += SQUARE(diff);
			}
			var /= BLOCK_COUNT;
			C = -0.0000015*var + 1.5142857;
			}*/

			putchar('\n');
			if (energy > C*avg) {
				energy = 0;
				if (!isPeak) {
					isPeak = 1;
					print("*");
					return 1;
				} else {
					print("#");
				}
			} else {
				energy = 0;
				isPeak = 0;
				print("");
			}

		}
	//}
	return 0;
}

void mode_beat(Config *config) {
	static unsigned long lastBeat = 0;
	static uint16_t bps = 0;

	static uint8_t index = 2;
	uint8_t colors[] = {0,96,160};

	setHSV(0, 255, 255);
	return;
	msgeq7_read();
	if (isBeat(0)) {
		bps++;
		lastBeat = millis;
		//setHSV(240, 255, 255);

		//hue += 64;
		//setHSV(hue, 255, 255);

		index = (index+1)%(sizeof(colors)/sizeof(colors[0]));
		//index = (index+1)%3;
		setHSV(colors[index], 255, 255);
	} else {
		if (millis > lastBeat+75) {
			//setHSV(120, 255, 255);
		}
	}

	static unsigned long lastBPM = 0;
	if (millis>lastBPM+10000) {
		puti(bps*6); bps=0;
		lastBPM = millis;
	}
}

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
