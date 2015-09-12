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
#define CURRENT_COLOR	0xFFFFFFFF

#define LCD_WIDTH		800 //in pixels
#define LCD_HEIGHT		480 //in pixels

void lcd_draw_pixel(unsigned int x, unsigned int y, unsigned int color);
void lcd_fill(unsigned int x0, unsigned int y0, unsigned int dx, unsigned int dy, unsigned int color);
void lcd_clear(unsigned int color);
void lcd_draw_hline(unsigned int x, unsigned int y0, unsigned int y1, unsigned int color);
void lcd_draw_vline(unsigned int x0, unsigned int x1, unsigned int y, unsigned int color);
void lcd_draw_rectangle(unsigned int x0, unsigned int x1, unsigned int y0, unsigned int y1, unsigned int color, int isFill);
void lcd_display_picture(const unsigned char *picture, unsigned int x0, unsigned int y0);
void lcd_display_character(unsigned char ch, unsigned int x0, unsigned int y0, unsigned int text_color, unsigned int background_color);
void lcd_display_string(unsigned char *str, unsigned int x0, unsigned int y0, unsigned int text_color, unsigned int background_color);

#endif
