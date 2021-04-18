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


void getCVSData(string filePath, vector<Game>& gameVector, unordered_multimap<string, Game>& gameMap) {
	ifstream inFile(filePath);

	if (inFile.is_open()) {
		string line;
		getline(inFile, line);

		while (getline(inFile, line)) {
			string temp;
			string temp2;
			string title;
			int year;
			int month; //convert month into integer representation with if-statements?
			double avg;
			double gain;
			int peak;
			string avgpeak; //this is in percentages.

			istringstream stream(line);

			getline(stream, title, ',');
			getline(stream, temp2, ',');
			year = stoi(temp2.substr(1, temp2.size() - 2));
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

			Game gameProperties(title.substr(1, title.size() - 2), year, month, avg, gain, peak, avgpeak);
			gameVector.push_back(gameProperties);
			gameMap.emplace(title.substr(1, title.size() - 2), gameProperties);
		}
	}
	else {
		cout << "file not opened." << endl;
	}
}

vector<Game> getBucketData(unordered_multimap<string, Game>& myMap, string _name) {
	int i = myMap.bucket(_name);
	vector<Game> temp;
	for (auto iter = myMap.begin(i); iter != myMap.end(i); ++iter) {
		temp.push_back(iter->second);
	}
	return temp;
}

Game jumpSearch(unordered_multimap<string, Game>& myMap, string _name, int _month, int _year) {
	vector<Game> games = getBucketData(myMap, _name);
	int m = sqrt(games.size());

	for (int i = 0; i < games.size(); i += m) {
		if (games[i].getYear() <= _year) {
			if (games[i].getMonth() < _month) {
				i -= m;
				if (i < 0) {
					i = 0;
				}
				while (i < i + m) {
					if (i < games.size()) {
						if (games[i].getYear() == _year && games[i].getMonth() == _month) {
							return games[i];
						}
						i++;
					}
					else {
						cout << "not in bucket" << endl;
						Game temp(2012);
						return temp;
					}
				}
				cout << "not in bucket" << endl;
			}
			else {
				while (i < i + 12) {
					if (i < games.size()) {
						if (games[i].getYear() == _year && games[i].getMonth() == _month) {
							return games[i];
						}
						i++;
					}
					else {
						cout << "not in bucket" << endl;
						Game temp(2012);
						return temp;
					}
				}
				cout << "not in bucket" << endl;
			}
		}
	}
	cout << "no data available" << endl;
	Game temp(2012);
	return temp;
}


int main()
{
	/*======= Load data from file(s) =======*/
	vector<Game> gameVector;
	unordered_multimap<string, Game> gameMap;
	getCVSData("data/steamcharts.csv", gameVector, gameMap);

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

	vector<Game> test = getBucketData(gameMap, "Counter-Strike: Global Offensive");
	cout << "Bucket Size: " << test.size() << endl;
	for (int i = 0; i < test.size(); i++) {
		cout << test[i].getTitle() << " " << test[i].getMonth() << "/" << test[i].getYear() << endl;
	}

	Game searchResult = jumpSearch(gameMap, "Secrets of Grindea", 3, 2021);
	if (searchResult.getYear() != -1) {
		cout << searchResult.getMonth() << "/" << searchResult.getYear() << endl;
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