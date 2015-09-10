#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>

#include "./images/book_heart.h"
#include "./images/robot.h"
#include "./includes/font.h"
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
		fprintf(stderr, "Error: failed to map framebuffer device to memory.\n");
		return -1;
	}
	fprintf(stderr, "The framebuffer device was mapped to memory successfully.\n");

	while(1) {
		lcd_clear(YELLOW_COLOR);
		sleep(5);
		lcd_fill(100,100,100,100,GREEN_COLOR);
		sleep(5);
		lcd_fill(200,200,100,100,BLUE_COLOR);
		sleep(5);
		lcd_fill(300,300,100,100,BLACK_COLOR);
		sleep(5);
		lcd_display_picture(gImage_robot,500,10);
		sleep(5);
		lcd_display_picture(gImage_book_heart,0,0);
		sleep(5);
	}

	munmap(fbp, screensize);
	close(fbfd);
	return 0;
}
