#pragma once

typedef struct {
	uint8_t r,g,b;
} rgb_t;

void led_init();
void setRGB(uint8_t, uint8_t, uint8_t);
void setHSV(uint8_t hue, uint8_t sat, uint8_t val);
void setAdjustment(uint8_t brightness, rgb_t* correction, rgb_t* temperature);
void setBrightness(uint8_t b);
