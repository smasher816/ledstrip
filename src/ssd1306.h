/*
 * ssd1306_bitbang.h
 *
 * Created: 4/21/2013 2:16:29 PM
 *  Author: kmm
 */

#pragma once
#define SS1306_OLED_RST PB0 //8
#define SS1306_OLED_DC  PB1 //9
#define SS1306_OLED_CS  PB2 //10
#define SS1306_OLED_DAT PB3 //11
#define SS1306_OLED_CLK PB5 //13

#define H(port, pin) port |= _BV((pin))
#define L(port, pin) port &= ~(_BV((pin)))

void oled_pbar(uint8_t val, uint8_t len);
void oled_clear_row(uint8_t row);
void oled_puti(uint8_t val, uint8_t len);
void oled_putc2(char c);
void oled_puts2(char *str);

void oled_write(uint8_t dc, uint8_t data);
void oled_init();
void oled_set_contrast(uint8_t val);
void oled_set_addr_mode(uint8_t mode);
void oled_put_tile(uint8_t *tile, uint8_t limit);
void oled_putc(char c);
void oled_puts(char *str);
void oled_move_raw(uint8_t row, uint8_t col);
void oled_move(uint8_t row, uint8_t col);
void oled_clear();
/*void oled_fill(uint8_t row, uint8_t col, uint8_t count, uint8_t max, uint32_t pattern, int8_t pshift);
void oled_putxy(uint8_t left_pxl, uint8_t top_pxl, uint8_t *tile);
void oled_bigdigit(uint8_t top, uint8_t left, uint8_t num);
void oled_box(uint8_t top, uint8_t left, uint8_t width, uint8_t height);*/
