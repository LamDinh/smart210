#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>

#include "./includes/font28.h"
#include "./includes/font36.h"
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

//Display a picture from raw data in an data array: created by Img2Lcd software
void lcd_display_picture(const unsigned char *picture, unsigned int x0, unsigned int y0)
{
	unsigned int x, y;
	unsigned int dx, dy;
	long int location = 0;

	dx = (unsigned int)((picture[2]<<8) | picture[3]);
	dy = (unsigned int)((picture[4]<<8) | picture[5]);

	//fprintf(stderr, "%s: size of pict: %dx%d\n", __FUNCTION__, dx, dy);

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

//Display a bmp picture from file system
//This function just supports 24 bits per pixel file
//file_name should be the full path of file
int lcd_display_bmp_picture(char *file_name, unsigned int x0, unsigned int y0)
{
	FILE *fp_bmp;
	unsigned char *BGR_raw_data = NULL, *BGR_tmp = NULL, *BGR = NULL;
	unsigned int x, y, i, j;
	long int location = 0;
	unsigned short bit_per_pixel, width, height;
	unsigned int SizeOfImage;
	unsigned char remainder;

	fp_bmp = fopen(file_name, "r");

	if (!fp_bmp) {
		fprintf(stderr, "Cann't open file: %s \n", file_name);
		return -1;
	}

	fseek(fp_bmp, 0x1CL, SEEK_SET);
	fread(&bit_per_pixel, 2 , 1, fp_bmp);

//	fprintf(stderr, "bit_per_pixel = %d = 0x%x\n", bit_per_pixel, bit_per_pixel);

	if (bit_per_pixel == 0x18) {		//24 bits per pixel
		fseek(fp_bmp, 0x12L, SEEK_SET);
		fread(&width, 2, 1, fp_bmp);  // Read width size image
		fseek(fp_bmp, 0x16L, SEEK_SET);
		fread(&height, 2, 1, fp_bmp);  // Read height size image
		fseek(fp_bmp, 0x22L, SEEK_SET);
		fread(&SizeOfImage, 4, 1, fp_bmp); //read size of image: number of bytes of raw data

//		fprintf(stderr, "width = %d = 0x%x (pixels)\n", width, width);
//		fprintf(stderr, "height = %d = 0x%x (pixels)\n", height, height);
//		fprintf(stderr, "SizeOfImage = %d = 0x%x (bytes)\n", SizeOfImage, SizeOfImage);

		fseek(fp_bmp, 0x36L, SEEK_SET); //jump to start address of raw data

		BGR_raw_data = (unsigned char *)calloc(SizeOfImage, 1);

		if (BGR_raw_data == NULL) {
			fprintf(stderr, "Cann't malloc memory for BGR_raw_data\n");
			return -1;
		}

		BGR_tmp = (unsigned char *)calloc(SizeOfImage, 1);

		if (BGR_tmp == NULL) {
			fprintf(stderr, "Cann't malloc memory for BGR_tmp\n");
			return -1;
		}

		BGR = (unsigned char *)calloc(SizeOfImage, 1);

		if (BGR == NULL) {
			fprintf(stderr, "Cann't malloc memory for BGR\n");
			return -1;
		}

		fread(BGR_raw_data, SizeOfImage, 1, fp_bmp);

		remainder = width % 4;

		for (i = 0; i < height; i++) {
			for (j = 0; j < width*3; j++) {
				BGR_tmp[i*width*3 + j] = BGR_raw_data[i * width * 3 + j + i * remainder];
			}
		}

		for (i = 0; i < height; i++) {
			for (j = 0;j < width*3;j++) {
				BGR[i*width*3+j] = BGR_tmp[(height-1-i)*width*3+j];
			}
		}
	}
	else {
		fprintf(stderr, "File %s is not 24 bits per pixel format: unsupported, please re-format into correct type\n", file_name);
		fclose(fp_bmp);
		return -1;
	}

	for(y = y0; y < (y0+height); y++) {
		for(x = x0; x < (x0+width); x++) {
			//Figure out where in memory to put the pixel
			location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * finfo.line_length;

			*(fbp + location) = BGR[3*(width*(y-y0) + (x-x0))];			//Blue
			*(fbp + location + 1) = BGR[3*(width*(y-y0) + (x-x0)) + 1];	//Green
			*(fbp + location + 2) = BGR[3*(width*(y-y0) + (x-x0)) + 2];	//Red
			*(fbp + location + 3) = 0;											//No transparency
		}
	}

	if (BGR_raw_data != NULL) {
		free(BGR_raw_data);
	}

	if (BGR_tmp != NULL) {
		free(BGR_tmp);
	}

	if (BGR != NULL) {
		free(BGR);
	}
	fclose(fp_bmp);

	return 0;
}

void lcd_display_character(unsigned char ch, unsigned char font, unsigned int x0, unsigned int y0, unsigned int text_color, unsigned int background_color)
{
	unsigned int x, y;
	unsigned int dx, dy;
	long int location = 0;
	unsigned int color_tmp[3];

	if (font == FONT28) {
		dx = (unsigned int)((font_arial_28[ch-32][2]<<8) | font_arial_28[ch-32][3]);
		dy = (unsigned int)((font_arial_28[ch-32][4]<<8) | font_arial_28[ch-32][5]);
	}
	else if (font == FONT36) {
		dx = (unsigned int)((font_arial_36[ch-32][2]<<8) | font_arial_36[ch-32][3]);
		dy = (unsigned int)((font_arial_36[ch-32][4]<<8) | font_arial_36[ch-32][5]);
	}

	for(y = y0; y < (y0+dy); y++) {
		for(x = x0; x < (x0+dx); x++) {
			//Figure out where in memory to put the pixel
			location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * finfo.line_length;
			//Save current color
			color_tmp[0] = *(fbp + location);
			color_tmp[1] = *(fbp + location + 1);
			color_tmp[2] = *(fbp + location + 2);

			if (font == FONT28) {
				//Coloring follow the font data
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
						*(fbp + location + 2) = (background_color>>16)&0xFF;//Red
					}
				}

				//Re-color unnecessary area into previous color
				if (x >= (x0+font28_width[ch-32])) {
					*(fbp + location) = color_tmp[0];		//Blue
					*(fbp + location + 1) = color_tmp[1];	//Green
					*(fbp + location + 2) = color_tmp[2];	//Red
				}
			}
			else if (font == FONT36) {
				//Coloring follow the font data
				if ((font_arial_36[ch-32][3*(dx*(y-y0) + (x-x0)) + 10] == 0x00)
					&& (font_arial_36[ch-32][3*(dx*(y-y0) + (x-x0)) + 9] == 0x00)
					&& (font_arial_36[ch-32][3*(dx*(y-y0) + (x-x0)) + 10] == 0x00)){
					*(fbp + location) = (text_color>>0)&0xFF;		//Blue
					*(fbp + location + 1) = (text_color>>8)&0xFF;	//Green
					*(fbp + location + 2) = (text_color>>16)&0xFF;	//Red
				}
				else {
					if (background_color != CURRENT_COLOR) {
						*(fbp + location) = (background_color>>0)&0xFF;		//Blue
						*(fbp + location + 1) = (background_color>>8)&0xFF;	//Green
						*(fbp + location + 2) = (background_color>>16)&0xFF;//Red
					}
				}
				//Re-color unnecessary area into previous color
				if (x >= (x0+font36_width[ch-32])) {
					*(fbp + location) = color_tmp[0];		//Blue
					*(fbp + location + 1) = color_tmp[1];	//Green
					*(fbp + location + 2) = color_tmp[2];	//Red
				}
			}

			*(fbp + location + 3) = 0;	//No transparency
		}
	}
}

void lcd_display_string(unsigned char *str, unsigned char font, unsigned int x0, unsigned int y0, unsigned int text_color, unsigned int background_color)
{
	unsigned int i;
	unsigned int x;

	x = x0;

	for (i = 0; i < strlen(str); i++) {
		lcd_display_character(str[i], font, x, y0, text_color, background_color);
		if (font == FONT28) {
			x += font28_width[str[i] - 32];

		} else if (font == FONT36) {
			x += font36_width[str[i] - 32];
		}

	}
}
