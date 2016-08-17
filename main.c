//File: main.c
//Author: lamdinh
//Email: lamdinh179@gmail.com

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <time.h>

#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>

#include "./images/book_heart.h"
#include "./images/robot.h"
#include "./includes/lcd_driver.h"


extern const unsigned char font_width[96];

char *fbp = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
int rtc_dev;

int main()
{
	int fbfd = 0;
	long int screensize = 0;
	int x = 0, y = 0;
	long int location = 0;
	char date_str[22], time_str[10], date_str_tmp[22], time_str_tmp[10];
	
	rtc_dev = 0;
	/* Open the file for reading and writing */
	rtc_dev  = open("/dev/rtc", O_RDWR);
	if (rtc_dev < 0)
	{
		fprintf(stderr, "- Error: Can not open RTC device, rtc_dev = %d\n", rtc_dev);
		return -1;
	}

	fprintf(stderr, "RTC device was opened successfully.\n");

	/* Open the file for reading and writing */
	fbfd = open("/dev/fb0", O_RDWR);
	if (!fbfd) {
		fprintf(stderr, "Error: cannot open framebuffer device.\n");
		return -1;
	}
	fprintf(stderr, "The framebuffer device was opened successfully.\n");

	//Get fixed screen information
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
		fprintf(stderr, "Error reading fixed information.\n");
		return -1;
	}

	//Get variable screen information
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
		fprintf(stderr, "Error reading variable information.\n");
		return -1;
	}

	//Figure out the size of the screen in bytes
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

	//Map the device to memory
	fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if ((int)fbp == -1) {
		fprintf(stderr, "Error: failed to map frame buffer device to memory.\n");
		return -1;
	}
	fprintf(stderr, "The frame buffer device was mapped to memory successfully.\n");

//	lcd_display_picture(gImage_filmframe,0,0);
//	lcd_clear(PINK_COLOR);

	lcd_display_bmp_picture("/LAM/LoveDream.bmp", 0, 0);
	lcd_display_string("IS FROM A", FONT36, 390, 220, BLUE_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/LoveAngel.bmp", 0, 0);
	lcd_display_string("SOMEWHERE IN THE WORLD...", FONT28, 120, 70, BLUE_COLOR, CURRENT_COLOR);
	lcd_display_string("GOD PREPARED A PARTNER FOR YOU.", FONT28, 40, 380, YELLOW_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/TreeCouple.bmp", 0, 0);
	lcd_display_string("AS COUPLE OF TREES", FONT28, 20, 50, RED_COLOR, CURRENT_COLOR);
	lcd_display_string("GROW UP TOGETHER", FONT28, 400, 100, PINK_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/CoupleOfSwan.bmp", 0, 0);
	lcd_display_string("AS COUPLE OF SWANS", FONT28, 20, 20, WHITE_COLOR, CURRENT_COLOR);
	lcd_display_string("SWIM SIDE BY SIDE", FONT28, 400, 70, RED_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/CoupleOfBirds.bmp", 0, 0);
	lcd_display_string("AS COUPLE OF BIRDS", FONT28, 20, 20, WHITE_COLOR, CURRENT_COLOR);
	lcd_display_string("WELCOME SUNRISE WITH A KISS", FONT28, 180, 70, RED_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/BookAndHeart.bmp", 0, 0);
	lcd_display_string("AND YOU ARE THE LOVE", FONT28, 20, 20, PINK_COLOR, CURRENT_COLOR);
	lcd_display_string("WHO GOD PREPARED FOR ME.", FONT28, 100, 420, BLUE_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/HandInHand.bmp", 0, 0);
	lcd_display_string("WE ARE HAND IN HAND,", FONT28, 50, 390, RED_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/LovelySky.bmp", 0, 0);
	lcd_display_string("AND WALK TOGETHER", FONT28, 50, 300, RED_COLOR, CURRENT_COLOR);
	lcd_display_string("ON THE LOVING ROAD.", FONT28, 350, 350, PINK_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/FlyingLoveMail.bmp", 0, 0);
	lcd_display_string("IT STARTED BY LOVELY MESSAGES", FONT28, 50, 50, PINK_COLOR, CURRENT_COLOR);
	lcd_display_string("EVERY DAY,", FONT36, 470, 90, RED_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/Love_Fan.bmp", 0, 0);
	lcd_display_string("AND SWEET WORDS WE SAID,", FONT28, 90, 390, RED_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/Rose.bmp", 0, 0);
	lcd_display_string("AND BEAUTIFUL ROSES.", FONT28, 50, 20, BLUE_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/HappyCouple1.bmp", 0, 0);
	lcd_display_string("BESIDE YOU, I ALWAYS FEEL HAPPY,", FONT28, 50, 20, PINK_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/HappyCouple2.bmp", 0, 0);
	lcd_display_string("AND SMILE A LOT.", FONT28, 420, 410, GREEN_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/HappyCouple3.bmp", 0, 0);
	lcd_display_string("I GO WITH YOU TO THE PLACES", FONT28, 20, 10, WHITE_COLOR, CURRENT_COLOR);
	lcd_display_string("WHERE I HAVE NEVER GONE BEFORE,", FONT28, 20, 410, RED_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/RunningBird.bmp", 0, 0);
	lcd_display_string("AND I ALSO DO", FONT28, 20, 380, GREEN_COLOR, CURRENT_COLOR);
	lcd_display_string("WHAT I HAVE NEVER DONE BEFORE,", FONT28, 90, 420, GREEN_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/GiangDien7.bmp", 0, 0);
	lcd_display_string("JUST TO SEE YOUR SMILE,", FONT28, 150, 20, PINK_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/MangoGarden.bmp", 0, 0);
	lcd_display_string("AND TO SEE YOUR HAPPINESS.", FONT28, 110, 390, RED_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/WaterFall.bmp", 0, 0);
	lcd_display_string("LOVE IS SOMETIME AS A WATERFALL", FONT28, 50, 230, RED_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/GiangDien3.bmp", 0, 0);
	lcd_display_string("IT IS RUGGED,", FONT28, 50, 380, RED_COLOR, CURRENT_COLOR);
	lcd_display_string("BUT MAKING US HAPPIER.", FONT28, 180, 420, PINK_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/GiangDien4.bmp", 0, 0);
	lcd_display_string("LOVE IS ALSO PEACE AS A FIELD,", FONT28, 80, 380, WHITE_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/Stream.bmp", 0, 0);
	lcd_display_string("WHERE WE ENJOY THE LIFE STREAM.", FONT28, 60, 420, RED_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/GiangDien5.bmp", 0, 0);
	lcd_display_string("LOVE IS ALSO AS THE FLOWER,", FONT28, 100, 20, WHITE_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/GiangDien6.bmp", 0, 0);
	lcd_display_string("MAKING OUR LIFE IS MORE BEAUTIFUL.", FONT28, 50, 80, PINK_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/SweetLove.bmp", 0, 0);
	lcd_display_string("LOVE IS SWEET,", FONT28, 260, 50, RED_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/LoveCoffeeCup.bmp", 0, 0);
	lcd_display_string("SOMETIME ASLO BE BITTER,", FONT28, 50, 20, BLACK_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/Park1.bmp", 0, 0);
	lcd_display_string("BUT YOUR SMILE MAKE ME HAPPY.", FONT28, 50, 50, RED_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/SantaCruz.bmp", 0, 0);
	lcd_display_string("WHEN I GO ANYWHERE,", FONT28, 300, 390, RED_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/ButterFly.bmp", 0, 0);
	lcd_display_string("OR JUST LEAVE YOU A SHORT TIME,", FONT28, 50, 50, BLUE_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/MissingMessage.bmp", 0, 0);
	lcd_display_string("I MISS YOU SO MUCH.", FONT28, 50, 130, RED_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/Kissing_message.bmp", 0, 0);
	lcd_display_string("MISS YOUR KISS,", FONT28, 350, 130, PINK_COLOR, CURRENT_COLOR);
	lcd_display_string("YOUR HUG,", FONT28, 100, 330, PINK_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/PrayWithBirthdayCake24.bmp", 0, 0);
	lcd_display_string("MISS YOUR INGENUITY,", FONT28, 350, 50, WHITE_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/Birthday25.bmp", 0, 0);
	lcd_display_string("MISS OUR SWEET MEMORIES,", FONT28, 120, 30, PINK_COLOR, CURRENT_COLOR);
	sleep(5);

	lcd_display_bmp_picture("/LAM/RiverSide.bmp", 0, 0);
	lcd_display_string("MISS THE BEAUTIFUL PLACES WE COME,", FONT28, 30, 30, WHITE_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/NguyenKim.bmp", 0, 0);
	lcd_display_string("LIKE THIS,", FONT28, 325, 50, WHITE_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/Park2.bmp", 0, 0);
	lcd_display_string("MISS YOUR SMILE,", FONT28, 415, 410, RED_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/Lotus1.bmp", 0, 0);
	lcd_display_string("MISS YOU A LOT. . .", FONT28, 50, 20, WHITE_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/Lotus2.bmp", 0, 0);
	lcd_display_string("A LOT. . .", FONT28, 190, 400, PINK_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/SunRise.bmp", 0, 0);
	lcd_display_string("DAY BY DAY, FROM SUNRISE,", FONT28, 150, 30, RED_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/NighAndMoon.bmp", 0, 0);
	lcd_display_string("TO MOONRISE,", FONT28, 280, 220, WHITE_COLOR, CURRENT_COLOR);
	sleep(7);

	lcd_display_bmp_picture("/LAM/ILoveU.bmp", 0, 0);
	lcd_display_string("I JUST WANNA TO SAY:", FONT28, 50, 50, BLUE_COLOR, CURRENT_COLOR);
	sleep(7);

	//lcd_display_bmp_picture("/LAM/WillYouMarryMe.bmp", 0, 0);
	//sleep(100);
	lcd_display_bmp_picture("/LAM/BrideGroom1.bmp", 0, 0);
	lcd_display_string("I'M SO HAPPY", FONT28, 90, 15, RED_COLOR, CURRENT_COLOR);
	lcd_display_string("THAT I MARRIED YOU", FONT28, 10, 55, RED_COLOR, CURRENT_COLOR);
	sleep(7);

	//lcd_display_bmp_picture("/LAM/KissingCouple.bmp", 0, 0);
	lcd_display_bmp_picture("/LAM/BrideGroom2.bmp", 0, 0);	
	lcd_display_string("KEEP THE TIME WE HAVE TOGETHER", FONT28, 40, 20, PINK_COLOR, CURRENT_COLOR);
	lcd_display_string("HAPPILY", FONT36, 310, 60, PINK_COLOR, CURRENT_COLOR);
	lcd_display_character(127, FONT36, 510, 60, RED_COLOR, CURRENT_COLOR); //heart
	lcd_display_character(127, FONT36, 270, 60, RED_COLOR, CURRENT_COLOR); //heart

	memset(date_str, 0, sizeof(date_str));
	memset(time_str, 0, sizeof(time_str));
	memset(date_str_tmp, 0, sizeof(date_str_tmp));
	memset(time_str_tmp, 0, sizeof(time_str_tmp));
	lcd_draw_rectangle(0,799,420, 479, PINK_COLOR, 1);
	while(1) {
		get_date_time(date_str, time_str);
		if (strcmp(date_str, date_str_tmp) != 0) {
			lcd_display_string(date_str, FONT36, 50, 420, BLUE_COLOR, PINK_COLOR);
		}
		if (strcmp(time_str, time_str_tmp) != 0) {
			lcd_display_string(time_str, FONT36, 600, 420, BLUE_COLOR, PINK_COLOR);
		}
		memset(date_str_tmp, 0, sizeof(date_str_tmp));
		memset(time_str_tmp, 0, sizeof(time_str_tmp));
		strcpy(date_str_tmp, date_str);
		strcpy(time_str_tmp, time_str);
		memset(date_str, 0, sizeof(date_str));
		memset(time_str, 0, sizeof(time_str));
		sleep(1);
	}

	munmap(fbp, screensize);
	close(fbfd);
	close(rtc_dev);
	return 0;
}
