#include "shide/sh_year_month_day.h"
#include "shide/tzdb.h"
#include "shide/make.h"
#include <iostream>
#include <iomanip>

int main()
{
	using std::chrono::hours;
	using std::chrono::minutes;
	using std::chrono::seconds;
	const std::string tz_name{"Iran"};
	sh_year_month_day ymd{ date::year(1401), date::month(12), date::day(1) };
	struct sh_fields fds{};
	fds.ymd = ymd;
	fds.tod = {hours(12), minutes(12), seconds(12)};
	const date::time_zone* tz{};
	tzdb::locate_zone(tz_name, tz);
	date::local_info info;
	auto tp = make_jdatetime(fds, tz, info);
	auto ld = date::local_days{ ymd }; 
	std::cout << std::fixed << tp << std::endl;
	std::cin.get();
	return 0;
}
