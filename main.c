#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>

#include "./images/book_heart.h"
#include "./images/robot.h"
//#include "./images/filmframe.h"
#include "./includes/lcd_driver.h"

char *fbp = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;

int main()
{
	int fbfd = 0;
	long int screensize = 0;
	int x = 0, y = 0;
	long int location = 0;
	//char date_str[15], time_str[9];

	/* Open the file for reading and writing */
	fbfd = open("/dev/fb0", O_RDWR);
	if (!fbfd) {
		fprintf(stderr, "Error: cannot open framebuffer device.\n");
		return -1;
	}
	printf("The framebuffer device was opened successfully.\n");

	/* Get fixed screen information */
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
		fprintf(stderr, "Error reading fixed information.\n");
		return -1;
	}

	/* Get variable screen information */
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
		fprintf(stderr, "Error reading variable information.\n");
		return -1;
	}

	/* Figure out the size of the screen in bytes */
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

	/* Map the device to memory */
	fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if ((int)fbp == -1) {
		fprintf(stderr, "Error: failed to map frame buffer device to memory.\n");
		return -1;
	}
	fprintf(stderr, "The frame buffer device was mapped to memory successfully.\n");

//	lcd_display_picture(gImage_filmframe,0,0);

//	while(1) {
		lcd_clear(YELLOW_COLOR);
		lcd_draw_vline(10, 799, 15, BLUE_COLOR);
		lcd_draw_vline(10, 799, 43, BLUE_COLOR);
		lcd_display_string("ABCDEFGHIJKLMNOPQRSTUVW", 10, 10, RED_COLOR, CURRENT_COLOR);
		lcd_display_string("UVWXYZabcdefghijklmn", 10, 50, BLUE_COLOR, CURRENT_COLOR);
		lcd_display_string("opqrstuvwxyz01234567", 10, 90, GREEN_COLOR, CURRENT_COLOR);
		lcd_display_string("!#@$%^&*():;'<>,./?`", 10, 130, BLACK_COLOR, CURRENT_COLOR);
		lcd_display_string("\{}[]|/", 10, 170, PINK_COLOR, CURRENT_COLOR);
		lcd_display_string("A B C D E F G H I J", 10, 210, PINK_COLOR, CURRENT_COLOR);
//		sleep(5);
//		lcd_display_picture(gImage_robot,500,10);
//		sleep(5);
//		lcd_display_picture(gImage_book_heart,0,0);
//		sleep(5);
//		memset(date_str, 0, sizeof(date_str));
//		memset(time_str, 0, sizeof(time_str));
//		get_date_time(date_str, time_str);
//		fprintf(stderr, "DATE: %s\n", date_str);
//		fprintf(stderr, "TIME: %s\n", time_str);
//	}

	munmap(fbp, screensize);
	close(fbfd);
	return 0;
}
