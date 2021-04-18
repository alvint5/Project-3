#include <iomanip>
#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include "Game.h"
using namespace std;

//#include "matplotlibcpp.h"
//namespace plt = matplotlibcpp;
//tiffany: setting up matplot, need to download + place into source folder.


void getCVSData(string filePath, vector<Game>& gameVector) {
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

			Game gameProperties(title, year, month, avg, gain, peak, avgpeak);
			gameVector.push_back(gameProperties);
		}
	}
	else {
		cout << "file not opened." << endl;
	}
}



//Game jumpSearch(unordered_map<string, Game>& myMap, string _name, int _month, int _year) {

	
//}


int main()
{
	/*======= Load data from file(s) =======*/
	vector<Game> gameVector;
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
	
	//tiffany: matplotlib work.
	//plotting a game's change in popularity by comparing:
	//average player count per month.
	//player gain per month (acceleration in change).
	//player peak per month (highest number of players on at the same time).

	/*
	plt::figure_size(1200, 780); //default 1200, 780 per pixel.
	//have to define x and y.
	//x will be month&year, y will be player count/stats.
	plt::plot(x,y);
	
	plt::xlim(0, 1000*1000);	//player count goes from 0 to 1 mil. should be scaled down for less popular games.
								//highest player count is csgo with 700k.

	std::stringstream titlecreate;
	//x axis: months of year, y axis: playercount.
	titlecreate << "Average Player Count for Year of " << [gameyear] << " for " << [gametitle];
	string titlecard = titlecreate.str();
	plt::title(titlecard);
	plt::legend();

	//save image.
	string imgname = "./" << [type of graph, ex: avg player count] & [gameyear] & [gametitle] << ".png";
	plt::save(imgname);
	
	//g++ basic.cpp -I/usr/include/python2.7 -lpython2.7 makefile cmd(?)
	*/
	return 0;
}