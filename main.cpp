#include <iomanip>
#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <vector>
#include <string>
#include "readdata.h"
using namespace std;

void getCVSData(string filePath, vector<readdata>& gameVector) {
	ifstream inFile(filePath);

	if (inFile.is_open()) {
		string line;
		getline(inFile, line);

		while (getline(inFile, line)) {
			string temp;
			string title;
			string year;
			int month; //convert month into integer representation with if-statements?
			double avg;
			double gain;
			int peak;
			string avgpeak; //this is in percentages.

			istringstream stream(line);

			getline(stream, title, ',');
			getline(stream, year, ',');
			//year = stoi(temp);
			getline(stream, temp, ',');
			if (temp == "\"January\"")
				month = 1;
			else if (temp == "\"February\"")
				month = 2;
			else if (temp == "\"March\"")
				month = 3;
			else if (temp == "\"April\"")
				month = 4;
			else if (temp == "\"May\"")
				month = 5;
			else if (temp == "\"June\"")
				month = 6;
			else if (temp == "\"July\"")
				month = 7;
			else if (temp == "\"August\"")
				month = 8;
			else if (temp == "\"September\"")
				month = 9;
			else if (temp == "\"October\"")
				month = 10;
			else if (temp == "\"November\"")
				month = 11;
			else if (temp == "\"December\"")
				month = 12;
			else
				month = 0;	//error.

			getline(stream, temp, ',');
			avg = stod(temp);
			getline(stream, temp, ',');
			if (temp == "NA") {
				gain = 0;
			}
			else {
				gain = stod(temp);
			}
			getline(stream, temp, ',');
			peak = stoi(temp);
			getline(stream, avgpeak, ',');

			readdata gameProperties(title, year, month, avg, gain, peak, avgpeak);
			gameVector.push_back(gameProperties);
		}
	}
	else {
		cout << "file not opened." << endl;
	}
}


int main()
{
	/*======= Load data from file(s) =======*/
	vector<readdata> gameVector;
	getCVSData("data/steamcharts.csv", gameVector);

	/*======= Print out how many sets were loaded =======*/
	int count = gameVector.size();
	cout << "Total number of sets: " << count << endl;

	/*======= Based on the choice, execute the appropriate task and show the results =======*/
	if (count != 0) {

		cout << "Game Title: " << gameVector.at(0).getTitle() << endl;
		cout << "Year: " << gameVector.at(0).getYear() << endl;
		cout << "Month: " << gameVector.at(0).getMonth() << endl;
		cout << "Average: " << gameVector.at(0).getAvg() << endl;
		cout << "Monthly Gain: " << gameVector.at(0).getGain() << endl;
		cout << "Monthly Peak: " << gameVector.at(0).getPeak() << endl;
		cout << "Average Peak Percentage: " << gameVector.at(0).getAvgPeak() << endl;
	}

	return 0;
}