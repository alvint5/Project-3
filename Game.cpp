#include "Game.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;


Game::Game(string title, string year, int month, double avg, int gain, int peak, string avgpeak)
{
    this->title = title;
    this->year = year;
    this->month = month;
    this->avg = avg;
    this->gain = gain;
    this->peak = peak;
    this->avgpeak = avgpeak;
}


string Game::getTitle() {
    return this->title;
}

string Game::getYear() {
    return this->year;
}

int Game::getMonth() {
    return this->month;
}

double Game::getAvg() {
    return this->avg;
}

double Game::getGain() {
    return this->gain;
}

int Game::getPeak() {
    return this->peak;
}

string Game::getAvgPeak() {
    return this->avgpeak;
}