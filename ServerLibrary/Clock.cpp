#include "stdafx.h"
#include "Clock.h"

Clock::Clock()
{
	serverStartTick_ = this->sysrtemTick();
}

Clock::~Clock()
{
}

tick_t Clock::strToTick(wstr_t str, WCHAR *fmt)
{
	int year		= 0;
	int month		= 0;
	int day			= 0;
	int hour		= 0;
	int minute		= 0;
	int second		= 0;

	swscanf_s(str.c_str(), fmt, &year, &month, &day, &hour, &minute, &second);

	tm time = {second, minute, hour, day, month -1, year - 1900};

	return mktime(&time);
}

tick_t Clock::serverStartTick()
{
	return serverStartTick_;
}
tick_t Clock::sysrtemTick()
{
	return system_clock::to_time_t(system_clock::now());
}

wstr_t Clock::tickToStr(tick_t tick, WCHAR *fmt)
{
	array<WCHAR, SIZE_128> timeStr;

	tm time;
	localtime_s(&time, &tick);
	wcsftime(timeStr.data(), timeStr.size(), fmt, &time);

	return timeStr.data();


}