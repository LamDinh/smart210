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

//Returns -1024 to 1024!
int qSin(unsigned int ph)
{
	int xsqr;

	ph %= 360;

	if(ph < 180) {
		xsqr = (8100-(ph-90)*(ph-90));
	}
	else {
		ph -= 180;
		xsqr = ((ph-90) * (ph-90) - 8100);
	}
	xsqr = (xsqr*517) >> 12;

	return xsqr;
}

void lcd_draw_eclipse(unsigned int x0, unsigned int y0, unsigned int x_r0, unsigned int y_r0, unsigned int color)
{
	unsigned int res = 360;
	unsigned int i;
	unsigned int x, y;

	for(i = 0; i < res; i++){
		x = x0 + ((x_r0 * qSin(i+90)) >> 10);
		y = y0 + ((y_r0 * qSin(i)) >> 10);
		lcd_draw_pixel(x, y, color);
	}
}

//draw a circle with radius r0 and center x0,y0
//isFill = 0: no fill, isFill=1: fill with color
void lcd_draw_circle(unsigned int x0, unsigned int y0, unsigned int r0, unsigned int color)
{
	lcd_draw_eclipse(x0, y0, r0, r0, color);
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

//void lcd_display_string(void)
//{
//	unsigned int x0=100, y0=100, i, j;
//	unsigned char tmp;
//	unsigned char font[] = {
//			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x0F, 0x00, 0xFE, 0xFF, 0xFF, 0x0F, 0x00, 0xFE, 0xFF, 0xFF, 0x0F, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x01, 0x00, 0x00, 0x00, 0xFC, 0x0F, 0x00, 0x00, 0x00, 0xE0, 0x7F, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x03, 0x00, 0x00, 0x00, 0xF8, 0x1F, 0x00, 0x00, 0x00, 0xC0, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x07, 0x00, 0x00, 0x00, 0xF0, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0xE0, 0x0F, 0x00, 0x00, 0x00, 0xFE, 0x07, 0x00, 0x00, 0xC0, 0xFF, 0x00, 0x00, 0x00, 0xF8, 0x1F, 0x00, 0x00, 0x00, 0xFF, 0x03, 0x00, 0x00, 0xE0, 0x7F, 0x00, 0x00, 0x00, 0xFC, 0x0F, 0x00, 0x00, 0x00, 0xFE, 0x01, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x0F, 0x00, 0xFE, 0xFF, 0xFF, 0x0F, 0x00, 0xFE, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
//	};
//
//	for (i=0;i<37;i++){
//		for (j=0;j<36;j++) {
//			tmp = font[5*i+(j/8)];
//			//printf("\ni = %d; j = %d; font[%d] = tmp = 0x%02x\n", i, j, 5*i+(j/8), tmp);
//			if (tmp & (0x80>>(j/8)))
//			{
//				lcd_draw_pixel(x0+i, y0+j, RED_COLOR);
//			}
//			else {
//				//lcd_draw_pixel(x0+i, y0+j, WHITE_COLOR);
//			}
//		}
//	}
//}


int lcd_display_character(unsigned char ch, unsigned int x0, unsigned int y0, unsigned int text_color, unsigned int background_color)
{
	unsigned int i;
	unsigned int x, y;
	unsigned char byte_color;
	unsigned char *char_buf = NULL;

	char_buf = malloc(FONT_BYTE + 1);
	if (char_buf == NULL) {
		fprintf(stderr, "%s: Can not malloc buffer char_buf\n", __FUNCTION__);
		return -1;
	}

	memset(char_buf, 0, sizeof(char_buf));
	memcpy(char_buf, font_Arial37x36 + FONT_BYTE*(ch-32), FONT_BYTE);

	for (x = 0; x < FONT_WIDTH; x++){ //display a character
		for (y = 0; y < FONT_HEIGHT; y++) {
			byte_color = char_buf[(FONT_BYTE_PER_COLUMN*x)+(y/8)];
			if (byte_color & (0x80>>(y/8)))
			{
				lcd_draw_pixel(x+x0, y+y0, text_color);
			}
			else {
				if (background_color != CURRENT_COLOR) {
					lcd_draw_pixel(x+x0, y+y0, background_color);
				}
			}
		}
	}

	if (char_buf != NULL) {
		free(char_buf);
	}

	return 0;
}

void lcd_display_string(unsigned char *str, unsigned int x0, unsigned int y0, unsigned int text_color, unsigned int background_color)
{
	unsigned int i;
	unsigned int x;

	x = x0;

	for (i = 0; i < strlen(str); i++) {
		lcd_display_character(str[i], x, y0, text_color, background_color);
		x += FONT_WIDTH;
	}
}
