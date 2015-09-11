#ifndef __LCD_DRIVER_H__
#define __LCD_DRIVER_H__

//Define Colors
#define RED_COLOR		0x00FF0000
#define GREEN_COLOR		0x0000FF00
#define BLUE_COLOR		0x000000FF
#define YELLOW_COLOR	0x00FFFF00
#define PINK_COLOR		0x00FF00FF
#define BLACK_COLOR		0x00000000
#define WHITE_COLOR		0x00FFFFFF

void lcd_draw_pixel(unsigned int x, unsigned int y, unsigned int color);
void lcd_fill(unsigned int x0, unsigned int y0, unsigned int dx, unsigned int dy, unsigned int color);
void lcd_clear(unsigned int color);
void lcd_display_picture(const unsigned char *picture, unsigned int x0, unsigned int y0);

#endif
