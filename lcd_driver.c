#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>

#include "./includes/font.h"
#include "./includes/lcd_driver.h"

extern char *fbp;
extern struct fb_var_screeninfo vinfo;
extern struct fb_fix_screeninfo finfo;

//color: 3 bytes: Red-Green-Blue: defined in lcd_driver.h
void lcd_draw_pixel(unsigned int x, unsigned int y, unsigned int color)
{
	long int location = 0;

	//Figure out where in memory to put the pixel
	location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * finfo.line_length;

	*(fbp + location) = (color>>0)&0xFF;		//Blue
	*(fbp + location + 1) = (color>>8)&0xFF;	//Green
	*(fbp + location + 2) = (color>>16)&0xFF;	//Red
	*(fbp + location + 3) = 0;					//No transparency
}

void lcd_fill(unsigned int x0, unsigned int y0, unsigned int dx, unsigned int dy, unsigned int color)
{
	unsigned int x, y;

	for(y = y0; y < (y0+dy); y++) {
		for(x = x0; x < (x0+dx); x++) {
			lcd_draw_pixel(x, y, color);
		}
	}
}

void lcd_clear(unsigned int color)
{
	lcd_fill(0, 0, LCD_WIDTH, LCD_HEIGHT, color);
}

void lcd_draw_hline(unsigned int x, unsigned int y0, unsigned int y1, unsigned int color)
{
	unsigned int i;

	for (i = y0; i <= y1; i++) {
		lcd_draw_pixel(x, i, color);
	}
}

void lcd_draw_vline(unsigned int x0, unsigned int x1, unsigned int y, unsigned int color)
{
	unsigned int i;

	for (i = x0; i <= x1; i++) {
		lcd_draw_pixel(i, y, color);
	}
}

//isFill = 0: no fill, isFill=1: fill with color
void lcd_draw_rectangle(unsigned int x0, unsigned int x1, unsigned int y0, unsigned int y1, unsigned int color, int isFill)
{
	if (isFill == 0) {
		lcd_draw_hline(x0, y0, y1, color);
		lcd_draw_hline(x1, y0, y1, color);
		lcd_draw_vline(x0, x1, y0, color);
		lcd_draw_vline(x0, x1, y1, color);
	}
	else {
		lcd_fill(x0, y0, (x1-x0), (y1-y0), color);
	}
}

void lcd_display_picture(const unsigned char *picture, unsigned int x0, unsigned int y0)
{
	unsigned int x, y;
	unsigned int dx, dy;
	long int location = 0;

	dx = (unsigned int)((picture[2]<<8) | picture[3]);
	dy = (unsigned int)((picture[4]<<8) | picture[5]);

	fprintf(stderr, "%s: size of pict: %dx%d\n", __FUNCTION__, dx, dy);

	for(y = y0; y < (y0+dy); y++) {
		for(x = x0; x < (x0+dx); x++) {
			//Figure out where in memory to put the pixel
			location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * finfo.line_length;

			*(fbp + location) = picture[3*(dx*(y-y0) + (x-x0)) + 10];		//Blue
			*(fbp + location + 1) = picture[3*(dx*(y-y0) + (x-x0)) + 9];	//Green
			*(fbp + location + 2) = picture[3*(dx*(y-y0) + (x-x0)) + 8];	//Red
			*(fbp + location + 3) = 0;										//No transparency
		}
	}
}

void lcd_display_character(unsigned char ch, unsigned int x0, unsigned int y0, unsigned int text_color, unsigned int background_color)
{
	unsigned int x, y;
	unsigned int dx, dy;
	long int location = 0;

	dx = (unsigned int)((font_arial_28[ch-32][2]<<8) | font_arial_28[ch-32][3]);
	dy = (unsigned int)((font_arial_28[ch-32][4]<<8) | font_arial_28[ch-32][5]);

	//fprintf(stderr, "%s: size of character: %dx%d\n", __FUNCTION__, dx, dy);

	for(y = y0; y < (y0+dy); y++) {
		for(x = x0; x < (x0+dx); x++) {
			//Figure out where in memory to put the pixel
			location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * finfo.line_length;

			if ((font_arial_28[ch-32][3*(dx*(y-y0) + (x-x0)) + 10] == 0x00)
				&& (font_arial_28[ch-32][3*(dx*(y-y0) + (x-x0)) + 9] == 0x00)
				&& (font_arial_28[ch-32][3*(dx*(y-y0) + (x-x0)) + 10] == 0x00)){
				*(fbp + location) = (text_color>>0)&0xFF;		//Blue
				*(fbp + location + 1) = (text_color>>8)&0xFF;	//Green
				*(fbp + location + 2) = (text_color>>16)&0xFF;	//Red
			}
			else {
				if (background_color != CURRENT_COLOR) {
					*(fbp + location) = (background_color>>0)&0xFF;		//Blue
					*(fbp + location + 1) = (background_color>>8)&0xFF;	//Green
					*(fbp + location + 2) = (background_color>>16)&0xFF;	//Red
				}
			}
			*(fbp + location + 3) = 0;	//No transparency
		}
	}
}

void lcd_display_string(unsigned char *str, unsigned int x0, unsigned int y0, unsigned int text_color, unsigned int background_color)
{
	unsigned int i;
	unsigned int x;

	x = x0;

	for (i = 0; i < strlen(str); i++) {
		lcd_display_character(str[i], x, y0, text_color, background_color);

		x += font_width[str[i] - 32];
	}
}
