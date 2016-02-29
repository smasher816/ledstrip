#include "led_sysdefs.h"
#include "lib8tion.h"
#include "pixeltypes.h"
#include "colorutils.h"
#include "colorpalettes.h"
#include "controller.h"


extern "C" {
	#include "led.h"
}

static CRGB scalingAdjust(255,255,255);
static uint8_t brightness = 255;

void led_init() {
	//set leds to output
	DDRB |= _BV(DDB1) | _BV(DDB2) | _BV(DDB3);

	//Non inverting, Mode 3 FAST PWM. No prescaling: OC1A, OC1B, OC2A
	TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11) | _BV(WGM10);
	TCCR1B = _BV(CS10);
	TCCR2A = _BV(COM2A1) | _BV(WGM21) | _BV(WGM20);
	TCCR2B = _BV(CS20);
}

void setRGB(const CRGB& color) {
	CRGB correctedColor = color;
	correctedColor.nscale8_video(brightness).scale8(scalingAdjust);
	OCR1A = correctedColor.r;
	OCR1B = correctedColor.g;
	OCR2A = correctedColor.b;
}

void setRGB(uint8_t r, uint8_t g, uint8_t b) {
	CRGB color(r,g,b);
	setRGB(color);
}

void setHSV(uint8_t hue, uint8_t sat, uint8_t val) {
	CRGB color(CHSV(hue, sat, val));
	setRGB(color);
}

void setAdjustment(uint8_t brightness, rgb_t* correction, rgb_t* temperature) {
	scalingAdjust = CLEDController::computeAdjustment(brightness, *((CRGB*)correction), *((CRGB*)temperature));
}

void setBrightness(uint8_t b) {
	brightness = b;
}
