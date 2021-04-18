#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;

class Game //change readdata to Game
{
    string title;
    string year;
    int month;
    double avg;
    double gain;
    int peak;
    string avgpeak; //this is in percentages.

public:
    Game();
    Game(string title, string year, int month, double avg, int gain, int peak, string avgpeak);
    string getTitle();
    string getYear();
    int getMonth();
    double getAvg();
    double getGain();
    int getPeak();
    string getAvgPeak();
};

