#include <stdio.h>
#include <time.h>

#include "./includes/date_time.h"

void get_date_time (char *date_str, char *time_str)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char *wday[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	char *week_day;

	week_day = wday[tm.tm_wday];

	sprintf(date_str, "%s, %02d/%02d/%04d", week_day, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
	sprintf(time_str, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
}
