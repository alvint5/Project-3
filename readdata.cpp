#include "readdata.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;


readdata::readdata(string title, string year, int month, double avg, int gain, int peak, string avgpeak)
{
    this->title = title;
    this->year = year;
    this->month = month;
    this->avg = avg;
    this->gain = gain;
    this->peak = peak;
    this->avgpeak = avgpeak;
}


string readdata::getTitle() {
    return this->title;
}

string readdata::getYear() {
    return this->year;
}

int readdata::getMonth() {
    return this->month;
}

double readdata::getAvg() {
    return this->avg;
}

double readdata::getGain() {
    return this->gain;
}

int readdata::getPeak() {
    return this->peak;
}

string readdata::getAvgPeak() {
    return this->avgpeak;
}