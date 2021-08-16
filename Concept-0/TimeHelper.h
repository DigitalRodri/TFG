#pragma once

#include <chrono>
#include <string>
#include <iostream>

/* Helper class to add functions to the tm and time_t C++ types */

char* printTM(tm time) {

    char printBuffer[26];
    strftime(printBuffer, 26, "%d-%m-%Y %H:%M:%S", &time);
    return printBuffer;
}

tm parseDateAndTime(std::string dateWithTime) {
    tm startDate = {};
    std::locale mylocale("");

    // Parseamos el string a un stringstream
    std::istringstream ss(dateWithTime);

    // Agregamos el formato horario
    ss.imbue(std::locale(mylocale));

    // Agregamos el formato de fecha
    ss >> std::get_time(&startDate, "%Y-%m-%dT%H:%M:%S");

    // Cogemos el horario de verano local
    time_t now = time(0);
    tm nowTM;
    localtime_s(&nowTM, &now);
    startDate.tm_isdst = nowTM.tm_isdst;

    return startDate;
}

tm time_TtoTM(time_t time) {
    tm response;
    localtime_s(&response, &time);
    return response;
}

time_t tmToTime_t(tm time) {
    return mktime(&time);
}

void addSecondsToTm(tm& time, int seconds) {

    time_t auxTime;
    auxTime = mktime(&time);
    auxTime += seconds;
    localtime_s(&time, &auxTime);
}

void addMinutesToTM(tm& time, int minutes) {

    time_t auxTime;
    auxTime = mktime(&time);
    auxTime += (static_cast<unsigned __int64>(minutes)) * 60;
    localtime_s(&time, &auxTime);
}

void addHoursToTM(tm& time, int hours) {

    time_t auxTime;
    auxTime = mktime(&time);
    auxTime += (static_cast<unsigned __int64>(hours)) * 3600;
    localtime_s(&time, &auxTime);
}

void addDaysToTM(tm& time, int days) {

    time_t auxTime;
    auxTime = mktime(&time);
    std::cout << auxTime << "\n";
    auxTime += (static_cast<unsigned __int64>(days)) * 86400;
    std::cout << auxTime << "\n";
    localtime_s(&time, &auxTime);
    std::cout << "Time: " << printTM(time) << "\n";
}