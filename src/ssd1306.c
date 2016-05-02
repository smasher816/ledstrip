/*

[!] SSD1306 Display Memory 101
Buffer is 8kbit (1kB) addressed with two auto-incrementable pointers as 8 pages * (128 columns * 8 bits)

         Display Memory        Column
     Cols 0    ->   127
		+---------------+       +---+
  PAGE0 | XCCCC...CCCCC |       | 0 |
R PAGE1 | CCCCC...CCCCC |   C =>|...|  * 127 => PAGE
o  ...  | ............. |       | 7 |
w PAGE6 | CCCCC...CCCCC |       +---+
s PAGE7 | CCCCC...CCCCC |         ^--- 8 1-bit pixels per column byte, arranged vertically regardless of addressing mode
        +---------------+
		 X => Pointer at PAGE 0, COL 0

[!] Fuck yeah tile graphics
For the purposes of oled_move(), oled_home(), oled_puts(), oled_putc() and most everything else,
the display is a 16x8 array of 8x8 character cells. Functions expect horizontal addressing mode, other modes
will make them act wanky. Pixels aren't really addressable; the tiny1634 doesn't have enough RAM for a local framebuffer,
and the SPI link to the display is one way so there's no read-modify-write using the display frame buffer.
Given the memory layout I'm reasonably certain the SSD1306 was intended to be driven, perhaps primarily, as a tile graphic display.

[!] Blast from the past
Character generator table is stored in progmem as "font", and contains 128 cells in
ASCII layout.
Characters are rotated 90 degrees clockwise (so we don't have to waste AVR cycles flipping tiles).
The pointy part of an 'A' should point that way -> if you're doing it right.
It's irritating to go alone, take this: http://www.min.at/prinz/o/software/pixelfont/

[!] Fast as a (LOGO) Vturtle
Much much faster if moving the pointers and updating incrementally (like a terminal!). Add graphic tiles and make the next-gen POPStation.

[!] Cute lil feller
Basic character display functions are less than 3k compiled, 1k of which is the 128-cell chargen data.
Chargen can be trimmed to 288ish bytes if only the bare minimum alphanumerics are required.

[!] Zero to Pixels
> Set pin/port defs in ssd1306-config.h
> oled_init()
> oled_clear() // buffer is full of entropy on boot
> oled_move(0,0)  // pointers should end up here, but let's make sure
> oled_puts("POOP") // POOP

[!] We built this city in AVR studio 6 using avr-gcc
Very little AVR-specific operations outside of the headers and PROGMEM macros, porting to PIC/ARM/HP9000/M88k/etc should be trivial.

[!] See Also
Ladyada's framebuffer-based arduinolib (c++) SSD1306 implementation: https://github.com/adafruit/Adafruit_SSD1306
RTFM: https://www.adafruit.com/datasheets/SSD1306.pdf

[!] Legal Fictions
Original work released under terms of the BSD license.
Included chargen/font data generated from c64_lower.64c font and used without permission.
(Chargen data is assumed to be of negligible economic value and public domain/abandoned. Lawyer at me if you got beef.)

-------------------------------------------------------------------------------
Inspired by tile-based arcade gfx, hacked together by kmm/smallscaleresearch
April 2013
Contact: kmm CHR(0x40) rmlabs.net
-------------------------------------------------------------------------------
*/

#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_PAGES 8

#define CHAR_WIDTH 5U
#define CHAR_HEIGHT 8
#define OLED_ROWS (OLED_WIDTH/CHAR_WIDTH)
#define OLED_COLS (OLED_HEIGHT/CHAR_HEIGHT)

#define SSD1306_DC_CMD 0
#define SSD1306_DC_DATA 1

#define SSD1306_MODE_HORIZ 0
#define SSD1306_MODE_VERT 1
#define SSD1306_MODE_PAGE 2

//Fundamental Commands
#define SSD1306_CONTRAST 0x81
#define SSD1306_ALLON 0xA4 //
#define SSD1306_ALLON_FORCE 0xA5 //
#define SSD1306_DISPLAY_NORMAL 0xA6
#define SSD1306_DISPLAY_INVERSE 0xA7
#define SSD1306_DISPLAY_OFF 0xAE
#define SSD1306_DISPLAY_ON 0xAF

//Scrolling Commands
#define SSD1306_SCROLL_RIGHT 0x26
#define SSD1306_SCROLL_LEFT 0x27
#define SSD1306_SCROLL_VERT_RIGHT 0x29
#define SSD1306_SCROLL_VERT_LEFT 0x2A
#define SSD1306_SCROLL_STOP 0x2E
#define SSD1306_SCROLL_START 0x2F
#define SSD1306_SCROLL_AREA 0xA3

//Address Setting Commands
#define SSD1306_COL_L 0x00 //OR
#define SSD1306_COL_H 0x10 //OR
#define SSD1306_ADDRESS_MODE 0x20
#define SSD1306_ADDRESS_HORIZ 0
#define SSD1306_ADDRESS_VERT 1
#define SSD1306_ADDRESS_PAGE 2
#define SSD1306_SET_COL 0x21
#define SSD1306_SET_PAGE 0x22
#define SSD1306_PAGE 0xB0 //OR

//Hardware Configuration
#define SSD1306_START_LINE 0x40
#define SSD1306_SEG_NORM 0xA0
#define SSD1306_SEG_INV 0xA1
#define SSD1306_MUX_RATIO 0xA8
#define SSD1306_COM_NORM 0xC0
#define SSD1306_COM_INV 0xC8
#define SSD1306_OFFSET 0xD3
#define SSD1306_COM_HW 0xDA

//Timing & Driving Commands
#define SSD1306_CLK_DIV 0xD5
#define SSD1306_PRECHRG 0xD9
#define SSD1306_DESELECT 0xDB
#define SSD1306_NOP 0xE3

//Charge Pump Commands
#define SSD1306_CHARGE_PUMP 0x8D
#define SSD1306_CHARGE_PUMP_OFF 0x10
#define SSD1306_CHARGE_PUMP_ON 0x14

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include "ssd1306.h"
#include "font.h"

int contrast = 127;
int _row = 0;
int _col = 0;

// dc arg specifies data or control buffer, 0 -> control, 1 -> data
void oled_write(uint8_t data, uint8_t dc) {
	// add delays to stretch bit times if needed
	// SSD1306 needs something like 15ns hold...probably not needed unless you port this to an ARM or AVR32.
	if(dc) { H(PORTB, SS1306_OLED_DC); }
	else { L(PORTB, SS1306_OLED_DC); }

	L(PORTB, SS1306_OLED_CS);
	SPDR = data;
	loop_until_bit_is_set(SPSR, SPIF);
	H(PORTB, SS1306_OLED_CS);
}

void oled_cmd(uint8_t cmd) {
	oled_write(cmd, SSD1306_DC_CMD);
}

void oled_data(uint8_t data) {
	oled_write(data, SSD1306_DC_DATA);
}

void oled_init() {
	DDRB |= _BV(SS1306_OLED_RST) | _BV(SS1306_OLED_CLK) | _BV(SS1306_OLED_DAT) | _BV(SS1306_OLED_DC) | _BV(SS1306_OLED_CS);
	SPCR = _BV(SPE) | _BV(MSTR);
	SPSR |= SPI2X;

	L(PORTB, SS1306_OLED_RST);
	_delay_ms(10);
	H(PORTB, SS1306_OLED_RST);
	_delay_ms(10);

	oled_cmd(SSD1306_MUX_RATIO);
	oled_cmd(OLED_HEIGHT-1);

	oled_cmd(SSD1306_OFFSET);
	oled_cmd(0x00);

	oled_cmd(SSD1306_COL_L | 0x0);
	oled_cmd(SSD1306_START_LINE | 0x0);
	oled_cmd(SSD1306_SEG_INV);
	oled_cmd(SSD1306_COM_INV);

	oled_cmd(SSD1306_COM_HW);
	oled_cmd(0x12);

	oled_cmd(SSD1306_ADDRESS_MODE);
	oled_cmd(SSD1306_ADDRESS_HORIZ);

	oled_cmd(SSD1306_CONTRAST);
	oled_cmd(contrast);

	//oled_cmd(SSD1306_ALLON);
	//oled_cmd(SSD1306_DISPLAY_NORMAL);

	oled_cmd(SSD1306_CLK_DIV);
	oled_cmd(0x80);

	oled_cmd(SSD1306_CHARGE_PUMP);
	oled_cmd(SSD1306_CHARGE_PUMP_ON);

	oled_cmd(SSD1306_DISPLAY_ON);

	/*
	oled_write(SSD1306_PRECHRG); // precharge
	oled_write(0, 0x11); // rtfm

	oled_write(SSD1306_DESELECT); // vcomh deselect level
	oled_write(0, 0x20); // rtfm
	*/
}


void oled_set_addr_mode(uint8_t mode) {
	oled_cmd(SSD1306_ADDRESS_MODE);
	oled_cmd(mode);
}

void oled_set_contrast(uint8_t val) {
	oled_cmd(SSD1306_CONTRAST);
	oled_cmd(val);
}

void oled_pbar(uint8_t val, uint8_t len) {
	oled_data(0xFF);
	for (uint8_t i=1; i<len-1; i++) {
		oled_data(i<val ? 0xFF : 0x81);
	}
	oled_data(0xFF);
}

void oled_clear_row(uint8_t row) {
	oled_move(row, 0);
	for (uint8_t i=0; i<127; i++) {
		oled_data(0x00);
	}
	oled_cmd(SSD1306_SET_COL);
	oled_cmd(0);
	oled_cmd(OLED_WIDTH-1);
}

void oled_puti(uint8_t val, uint8_t len) {
	static char s[10] = {0};
	itoa(val, s, 10);

	uint8_t i=0.;
	for(; s[i]!='\0'; i++) {
		oled_putc2(s[i]);
	}
	for (; i<len; i++) {
		oled_putc2(' ');
	}
}

void oled_put_tile(uint8_t *tile, uint8_t limit) {
	for(uint16_t i = 0; i < limit; i++) {
		oled_data(*tile++);
	}
}

void oled_putc(char c) {
	for(uint16_t i = c*CHAR_WIDTH; i < (c+1)*CHAR_WIDTH; i++) {
		oled_data(pgm_read_byte(font + i));
	}
	oled_data(0x00);
}

void oled_putc2(char c) {
	uint8_t row = _row;
	uint8_t col = _col;
	for(uint16_t i = c*CHAR_WIDTH; i < (c+1)*CHAR_WIDTH; i++) {
		uint8_t data = pgm_read_byte(font+i);
		uint8_t l =
				((data&(1<<0)) ? (3<<0) : 0) |
				((data&(1<<1)) ? (3<<2) : 0) |
				((data&(1<<2)) ? (3<<4) : 0) |
				((data&(1<<3)) ? (3<<6) : 0);
		oled_data(l);
		oled_data(l);
	}
	oled_move_raw(row+1, col);
	for(uint16_t i = c*CHAR_WIDTH; i < (c+1)*CHAR_WIDTH; i++) {
		uint8_t data = pgm_read_byte(font+i);
		uint8_t h =
				((data&(1<<4)) ? (3<<0) : 0) |
				((data&(1<<5)) ? (3<<2) : 0) |
				((data&(1<<6)) ? (3<<4) : 0) |
				((data&(1<<7)) ? (3<<6) : 0);
		oled_data(h);
		oled_data(h);
	}
	oled_data(0x00);
	oled_move_raw(row, col+(CHAR_WIDTH+1)*2);
}

void oled_puts(char *str) {
	while(*str != 0) {
		oled_putc(*str++);
	}
}

void oled_puts2(char *str) {
	while(*str != 0) {
		oled_putc2(*str++);
	}
}

void oled_move_raw(uint8_t row, uint8_t col) {
	if(col >= OLED_WIDTH) { col = OLED_WIDTH-1; }
	if(row >= OLED_PAGES) { row = OLED_HEIGHT-1; }

	_row = row;
	_col = col;

	oled_cmd(SSD1306_SET_COL);
	oled_cmd(col);
	oled_cmd(OLED_WIDTH-1);
	oled_cmd(SSD1306_SET_PAGE);
	oled_cmd(row);
	oled_cmd(OLED_PAGES-1);
}

void oled_move(uint8_t row, uint8_t col) {
	oled_move_raw(row, col/CHAR_WIDTH);
}

void oled_clear() {
	oled_move(0,0);
	for(uint16_t i=0; i<OLED_WIDTH*OLED_PAGES; i++) {
		oled_data(0x00);
	}
}
