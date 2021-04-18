#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;

class readdata
{
    string title;
    string year;
    int month; //convert month into integer representation with if-statements?
    double avg;
    double gain;
    int peak;
    string avgpeak; //this is in percentages.

public:
    readdata(string title, string year, int month, double avg, int gain, int peak, string avgpeak);
    string getTitle();
    string getYear();
    int getMonth();
    double getAvg();
    double getGain();
    int getPeak();
    string getAvgPeak();
};
