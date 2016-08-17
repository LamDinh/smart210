//File: date_time.c
//Author: lamdinh
//Email: lamdinh179@gmail.com

#include <stdio.h>
#include <time.h>
#include <fcntl.h>

#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include "./includes/date_time.h"

extern int rtc_dev;
void get_date_time (char *date_str, char *time_str)
{
//	time_t t = time(NULL);
//	struct tm tm = *localtime(&t);
//	char *wday[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//	char *week_day;
//
//	week_day = wday[tm.tm_wday];
//
//	sprintf(date_str, "%s, %02d/%02d/%04d ", week_day, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
//	sprintf(time_str, "%02d:%02d:%02d ", tm.tm_hour, tm.tm_min, tm.tm_sec);

	struct rtc_time {
		int	tm_sec; 	 
		int	tm_min; 	 
		int	tm_hour; 	 
		int	tm_mday; 	 
		int	tm_mon; 	 
		int	tm_year; 	 
		int	tm_wday; /* unused */
		int	tm_yday; /* unused */
		int	tm_isdst;/* unused */
	};

	struct rtc_time rt;
	char *wday[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
	char *week_day;

	//http://linux.die.net/man/4/rtc
	ioctl(rtc_dev, RTC_RD_TIME, &rt);

	fprintf(stderr, "Current RTC date/time is %d-%d-%d, %02d:%02d:%02d.\n",
		rt.tm_mday, rt.tm_mon + 1, rt.tm_year + 1900,
		rt.tm_hour, rt.tm_min, rt.tm_sec);

	week_day = wday[rt.tm_wday];
	sprintf(date_str, "%s, %02d/%02d/%04d ", week_day, rt.tm_mday, rt.tm_mon + 1, rt.tm_year + 1900);
	sprintf(time_str, "%02d:%02d:%02d ", rt.tm_hour, rt.tm_min, rt.tm_sec);

}
