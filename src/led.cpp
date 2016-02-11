#include "led_sysdefs.h"
#include "lib8tion.h"
#include "pixeltypes.h"
#include "colorutils.h"
#include "colorpalettes.h"

extern "C" {
	#include "led.h"
}

void led_init() {
	//Non inverting, Mode 3 FAST PWM. No prescaling: OC1A, OC1B, OC2A
	TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11) | _BV(WGM10);
	TCCR1B = _BV(CS10);
	TCCR2A = _BV(COM2A1) | _BV(WGM21) | _BV(WGM20);
	TCCR2B = _BV(CS20);
}

void setRGB(uint8_t r, uint8_t g, uint8_t b) {
	OCR1A = r;
	OCR1B = g;
	OCR2A = b;
}

void setHue(uint8_t hue) {
	CRGB color(CHSV(hue, 255, 255));
	setRGB(color.r, color.g, color.b);
}
