#pragma once

template <class Rep, class Period>
class DateAndTime {

public:
	std::chrono::year_month_day date;
	std::chrono::duration<Rep, Period> time;

public:
	DateAndTime() {
	}

	DateAndTime(std::chrono::year_month_day date, std::chrono::duration<long double, std::ratio<1i64, 1i64>> time) {
		this->date = date;
		this->time = time;

	}

	DateAndTime(std::chrono::duration<long double, std::ratio<1i64, 1i64>> time) {
		this->time = time;

	}

	DateAndTime(std::chrono::year_month_day date) {
			this->date = date;
	}

	void printDateAndTime() {

		std::cout << format("Date and time: {:%d/%m/%Y}", this->date) << ' ' << hh_mm_ss(this->time);
	}

};