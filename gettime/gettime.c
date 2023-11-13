#include "gettime.h"

char* getSysTime() {
	time_t time_stamp;					//存放时间戳
	static char time_data[20] = { 0 };	//2023-07-13 17:29:26

	time(&time_stamp);	//初始化

	struct tm* p = localtime(&time_stamp);

	sprintf(time_data, "%04d-%02d-%02d %02d:%02d:%02d" \
		, p->tm_year + 1900, p->tm_mon + 1, p->tm_mday \
		, p->tm_hour, p->tm_min, p->tm_sec);
	return time_data;
}



