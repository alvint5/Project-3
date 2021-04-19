#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;

class Game //change readdata to Game.
{
    string title;
    int year;
    int month;
    double avg;
    double gain;
    int peak;
    string avgpeak; //this is in percentages.

public:
    Game();
    Game(string title, int year, int month, double avg, int gain, int peak, string avgpeak);
    Game(int year);
    //accessor functions.
    string getTitle();
    int getYear();
    int getMonth();
    double getAvg();
    double getGain();
    int getPeak();
    string getAvgPeak();
};

