#include "Game.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;

Game::Game() {
    this->title = "";
}

Game::Game(string title, int year, int month, double avg, int gain, int peak, string avgpeak) //change readdata to Game
{
    this->title = title;
    this->year = year;
    this->month = month;
    this->avg = avg;
    this->gain = gain;
    this->peak = peak;
    this->avgpeak = avgpeak;
}

Game::Game(int year) {
    this->year = -1;
}


string Game::getTitle() {
    return this->title;
}

int Game::getYear() {
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