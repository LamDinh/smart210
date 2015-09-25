#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>

#include "./images/book_heart.h"
#include "./images/robot.h"
//#include "./images/filmframe.h"
#include "./includes/lcd_driver.h"


extern const unsigned char font_width[96];

char *fbp = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;

int main()
{
	int fbfd = 0;
	long int screensize = 0;
	int x = 0, y = 0;
	long int location = 0;
	char date_str[19], time_str[13], date_str_tmp[19], time_str_tmp[13];

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

	lcd_clear(YELLOW_COLOR);
	memset(date_str, 0, sizeof(date_str));
	memset(time_str, 0, sizeof(time_str));
	memset(date_str_tmp, 0, sizeof(date_str_tmp));
	memset(time_str_tmp, 0, sizeof(time_str_tmp));
	//lcd_display_picture(gImage_book_heart,0,0);
	lcd_display_bmp_picture("/root/LAM/photo.bmp", 0, 0);
	while(1) {
		get_date_time(date_str, time_str);
		if (strcmp(date_str, date_str_tmp) != 0) {
			lcd_display_string(date_str, 50, 200, BLUE_COLOR, RED_COLOR);
		}
		if (strcmp(time_str, time_str_tmp) != 0) {
			lcd_display_string(time_str, 50, 260, BLUE_COLOR, RED_COLOR);
		}
		strcpy(date_str_tmp, date_str);
		strcpy(time_str_tmp, time_str);
		memset(date_str, 0, sizeof(date_str));
		memset(time_str, 0, sizeof(time_str));
		sleep(1);
	}

	munmap(fbp, screensize);
	close(fbfd);
	return 0;
}
