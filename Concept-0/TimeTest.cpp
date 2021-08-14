#pragma once

#include <chrono>
#include <format>
#include <sstream>
#include <string>
#include <iostream>
#include <string_view>
#include "TimeHelper.h"

using namespace std;
using namespace std::chrono;

class DateAndTime {

public:
	std::chrono::year_month_day date;
	std::chrono::seconds time = 0s;
	
public:
	DateAndTime() {
	}

	DateAndTime(std::chrono::year_month_day date, std::chrono::seconds time) {
		this->date = date;
		this->time = time;

	}

	void printDateAndTime() {

		std::cout << format("Date and time: {:%d/%m/%Y}", this->date) << ' ' << hh_mm_ss(this->time);
	}

};

void CalendarTest()
{
	day d;
	month m = January;
	basic_stringstream<char> sstr{ "22" };
	basic_string<char> s{ "%d" };
	sstr >> parse(s, d);
	cout << d << "\n";

	d++;
	cout << d << "\n";

	year_month_day programmers_day{ January / 7d / 2021 };
	year_month_day spanishFormat{23d / March / 2021};
	// Hay que poner ':' antes del formato entre brackets {}
	cout << format("International Programmer's day: {:%d/%m/%Y}", programmers_day);
}

void HourTest() {

	seconds sec = 23s;
	hh_mm_ss hora1 = hh_mm_ss(23h + 40min + 23s);
	cout << "Hora hh_mm_ss: " << hora1 << "\n";
	duration d = hora1.to_duration();
	cout << "Hora en duration: " << d << "\n";

	d += 23min;
	cout << "Hora en duration + 23 mins: " << d << "\n";

	hh_mm_ss hora2 = hh_mm_ss(d);
	cout << "Hora en hh_mm_ss + 23 mins: " << hora2;
}

void DateAndTimeTest() {
	std::chrono::year_month_day spanishFormat{ 23d / March / 2021 };
	std::chrono::seconds durationSeconds = 4h+23min+43s+9h;
	DateAndTime dateAndTime = DateAndTime(spanishFormat, durationSeconds);
	dateAndTime.printDateAndTime();
}

DateAndTime getFinishTimeOld(int taskDuration, DateAndTime initialDateAndTime) {

	int hoursPerDay = 8;
	int remainingHours;
	int daysToAdvance = taskDuration / hoursPerDay;

	// If there are more hours in the duration than labour hours, we advance time that many days
	if (daysToAdvance != 0) {

		remainingHours = taskDuration - daysToAdvance * 24;
		// We advance the days directly
		std::chrono::day daysToAdvance = std::chrono::day(daysToAdvance);
		//daysToAdvance += 24d;
		initialDateAndTime.date = { daysToAdvance / initialDateAndTime.date.month() / initialDateAndTime.date.year() };
		//initialDateAndTime.date += std::chrono::months(taskDays);
	}

}

tm getFinishTimeNew(int taskDuration, tm startTime) {

	int startHour = 9;
	int finishHour = 17;
	int hoursPerDay = 8;
	int scrumTime = 11;

	// Auxiliary variables
	size_t taskDays = taskDuration / hoursPerDay;
	int remainingHours = taskDuration % hoursPerDay;
	time_t timeAtStart;
	time_t timeAtFinish;
	time_t finishTime;

	cout << "Time was: " << printTM(startTime) << "\n";

	// If the task spans more than one labour day, we advance the days directly and add the remaining hours
	if (taskDays != 0)
	{
		addDaysToTM(startTime, taskDays);

		cout << "Time is: " << printTM(startTime) << "\n";

		// Task duration is set to the remaining hours
		taskDuration = remainingHours;

	}

	// The duration is added to the time
	addHoursToTM(startTime, taskDuration);

	cout << "Time is: " << printTM(startTime) << "\n";

	return startTime;
}

int main() {

	tm tmTest = parseDateAndTime("2021-08-11T9:00:00");

	getFinishTimeNew(12, tmTest);

	//CalendarTest();
	//HourTest();
	//DateAndTimeTest();
}

