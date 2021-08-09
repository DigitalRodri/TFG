#pragma once

#include <chrono>
#include <format>
#include <sstream>
#include <string>
#include <iostream>
#include <string_view>

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

int main() {
	//CalendarTest();
	//HourTest();
	DateAndTimeTest();
}

