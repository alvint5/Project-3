#include <iomanip>
#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <chrono>
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

			Game gameProperties(title.substr(1, title.size() - 2), year, month, avg, gain, peak, avgpeak.substr(1, avgpeak.size() - 2));
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
				while (i < i + m) {
					if (games[i].getYear() == _year && games[i].getMonth() == _month) {
						return games[i];
					}
					i++;
				}
				//cout << "not in bucket" << endl;
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
						//cout << "not in bucket" << endl;
						Game temp(2012);
						return temp;
					}
				}
				//cout << "not in bucket" << endl;
			}
		}
	}
	Game temp(2012);
	return temp;
}

Game FibonacciSearch(unordered_multimap<string, Game>& myMap, string _name, int _month, int _year)
{
	vector<Game> games = getBucketData(myMap, _name);
	int n = games.size();
	int fibOne = 0;						// (n - 2)'th Fibonacci Number
	int fibTwo = 1;						// (n - 1)'th Fibonacci Number
	int fibN = fibOne + fibTwo;			// n'th Fibonacci Number


	while (fibN < n)
	{
		fibTwo = fibOne;
		fibOne = fibN;
		fibN = fibOne + fibTwo;
	}

	int offset = -1;


	while (fibN > 1)
	{
		int i = min(offset + fibTwo, n - 1);			// Finds the minimum between two index values

		// Edge case check for if the Fibonacci search reaches the front of the vector
		if (i == 0)
		{
			// Checks for whether the month/year provided by the user is "too recent" (no data for the input given)
			if (games[i].getYear() == _year)
			{
				if (games[i].getMonth() < _month)
				{
					Game temp(12);
					return temp;
				}
			}
			else
			{
				Game temp(12);
				return temp;
			}
		}


		if (games[i].getYear() > _year)
		{
			fibN = fibOne;
			fibOne = fibTwo;
			fibTwo = fibN - fibOne;
			offset = i;
		}

		else if (games[i].getYear() < _year)
		{
			fibN = fibTwo;
			fibOne = fibOne - fibTwo;
			fibTwo = fibN - fibOne;
		}

		else if (games[i].getYear() == _year)
		{
			if (games[i].getMonth() > _month)
			{
				fibN = fibOne;
				fibOne = fibTwo;
				fibTwo = fibN - fibOne;
				offset = i;
			}

			else if (games[i].getMonth() < _month)
			{
				fibN = fibTwo;
				fibOne = fibOne - fibTwo;
				fibTwo = fibN - fibOne;
			}
			else
			{
				return games[i];
			}
		}

		else
		{
			return games[i];
		}
	}

	// Checks the last entry in the vector
	if (fibOne && games[offset - 1].getYear() == _year && games[offset - 1].getMonth() == _month)
	{
		return games[offset - 1];
	}

	Game temp(12);
	return temp;
}


struct myComparator {
	int operator() (Game& _game1, Game& _game2) {
		return _game1.getAvg() < _game2.getAvg();
	}
};

	
vector<Game> top10GamesJump(unordered_multimap<string, Game>& myMap, int _month, int _year) {
	priority_queue<Game, vector<Game>, myComparator> maxHeap;
	vector<Game> games;

	for (int i = 0; i < myMap.bucket_count(); ++i) {
		auto iter = myMap.begin(i);
		if (iter != myMap.end(i)) {
			Game temp = jumpSearch(myMap, iter->first, _month, _year);
			if (temp.getYear() != -1) {
				maxHeap.push(temp);
			}
		}
	}

	int temp = 10;

	while (temp > 0) {
		Game temp2 = maxHeap.top();
		games.push_back(temp2);
		maxHeap.pop();
		temp--;
	}

	return games;
}

vector<Game> top10GamesFib(unordered_multimap<string, Game>& myMap, int _month, int _year) {
	priority_queue<Game, vector<Game>, myComparator> maxHeap;
	vector<Game> games;

	for (int i = 0; i < myMap.bucket_count(); ++i) {
		auto iter = myMap.begin(i);
		if (iter != myMap.end(i)) {
			Game temp = FibonacciSearch(myMap, iter->first, _month, _year);
			if (temp.getYear() != -1) {
				maxHeap.push(temp);
			}
		}
	}

	int temp = 10;

	while (temp > 0) {
		Game temp2 = maxHeap.top();
		games.push_back(temp2);
		maxHeap.pop();
		temp--;
	}

	return games;
}

vector<Game> top10AllTime(unordered_multimap<string, Game>& myMap) {
	priority_queue<Game, vector<Game>, myComparator> maxHeap;
	vector<Game> games;

	for (auto iter = myMap.begin(); iter != myMap.end(); ++iter) {
		if (iter->second.getYear() != -1) {
			maxHeap.push(iter->second);
		}
	}

	int temp = 10;

	while (temp > 0) {
		Game temp2 = maxHeap.top();
		games.push_back(temp2);
		maxHeap.pop();
		temp--;
	}

	return games;
}



int main()
{
	cout << std::fixed << setprecision(5);
	cout << "Welcome User! One second while we retrieve our data." << endl;

	/*======= Load data from file(s) =======*/
	vector<Game> gameVector;
	unordered_multimap<string, Game> gameMap;
	getCVSData("data/steamcharts.csv", gameVector, gameMap);

	int choice = 1;
	while (choice != 7) {
		cout << "1. Game Information\n";
		cout << "2. Highest Average Playerbase\n";
		cout << "3. Monthly Gain in Playerbase\n";
		cout << "4. Monthly Peak in Playerbase\n";
		cout << "5. Average Peak Percentage in Playerbase\n";
		cout << "6. All-time Records\n";
		cout << "7. End Program\n\n";
		cin >> choice;
		cin.get(); //clears newline character.

		if (choice == 1) {
			string temp;
			cout << "Insert game title: " << endl;
			getline(cin, temp);
			cout << "Insert year: " << endl;
			int temp2;
			cin >> temp2;
			cout << "Insert month: " << endl;
			int temp3;
			cin >> temp3;
			auto start = chrono::steady_clock::now();
			Game temp4 = jumpSearch(gameMap, temp, temp3, temp2);
			auto end = chrono::steady_clock::now();
			chrono::duration<double> jump_time = end - start;
			
			start = chrono::steady_clock::now();
			Game temp5 = FibonacciSearch(gameMap, temp, temp3, temp2);
			end = chrono::steady_clock::now();
			chrono::duration<double> fib_time = end - start;
			if (temp4.getYear() == -1) {
				cout << "Game not found in data." << endl;
			}
			else {
				cout << temp4.getTitle() << endl;
				cout << "Average Player Count: " << temp4.getAvg() << endl;
				cout << "Peak Player Count: " << temp4.getPeak() << endl;
				cout << "Gain from Previous Month: " << temp4.getGain() << endl;
				cout << "Average/Peak Percentage: " << temp4.getAvgPeak() << endl;
				cout << "Jump Search Algorithm Time: " << jump_time.count() << " s" << endl;
				cout << "Fibonacci Search Algorithm Time: " << fib_time.count() << " s\n" << endl;
			}
		}
		else if (choice == 2) {
			int temp;
			int temp2;
			cout << "Insert year: " << endl;
			cin >> temp;
			cout << "Insert month: " << endl;
			cin >> temp2;
			auto start = chrono::steady_clock::now();
			vector<Game> test = top10GamesFib(gameMap, temp2, temp);
			auto end = chrono::steady_clock::now();
			chrono::duration<double> jump_time = end - start;
			start = chrono::steady_clock::now();
			test = top10GamesFib(gameMap, temp2, temp);
			end = chrono::steady_clock::now();
			for (int i = 0; i < test.size(); i++) {
				cout << i + 1 << ": " << test[i].getTitle() << " - " << test[i].getAvg() << endl;
			}
			chrono::duration<double> fib_time = end - start;
			cout << "\nJump Search Algorithm Time: " << jump_time.count() << " s" << endl;
			cout << "Fibonacci Search Algorithm Time: " << fib_time.count() << " s\n" << endl;
		}
		else if (choice == 3) {
			cout << "Insert game title:" << endl;
			string temp3;
			getline(cin, temp3);
			cout << "Insert year: " << endl;
			int temp;
			cin >> temp;
			auto start = chrono::steady_clock::now();
			for (int i = 1; i < 13; i++) {
				Game temp2 = jumpSearch(gameMap, temp3, i, temp);
				if (temp2.getYear() == -1) {
					cout << i << ": No Info For This Month" << endl;
				}
				else {
					cout << i << ": " << temp2.getGain() << endl;
				}
			}
			auto end = chrono::steady_clock::now();
			chrono::duration<double> jump_time = end - start;

			start = chrono::steady_clock::now();
			for (int i = 1; i < 13; i++) {
				Game temp2 = FibonacciSearch(gameMap, temp3, i, temp);
				if (temp2.getYear() == -1) {
					cout << "";
				}
				else {
					cout << "";
				}
			}
			end = chrono::steady_clock::now();
			chrono::duration<double> fib_time = end - start;
			
			cout << "\nJump Search Algorithm Time: " << jump_time.count() << " s" << endl;
			cout << "Fibonacci Search Algorithm Time: " << fib_time.count() << " s\n" << endl;
		}
		else if (choice == 4) {
			cout << "Insert game title:" << endl;
			string temp3;
			getline(cin, temp3);
			cout << "Insert year: " << endl;
			int temp;
			cin >> temp;
			auto start = chrono::steady_clock::now();
			for (int i = 1; i < 13; i++) {
				Game temp2 = jumpSearch(gameMap, temp3, i, temp);
				if (temp2.getYear() == -1) {
					cout << i << ": No Info For This Month" << endl;
				}
				else {
					cout << i << ": " << temp2.getPeak() << endl;
				}
			}
			auto end = chrono::steady_clock::now();
			chrono::duration<double> jump_time = end - start;

			start = chrono::steady_clock::now();
			for (int i = 1; i < 13; i++) {
				Game temp2 = FibonacciSearch(gameMap, temp3, i, temp);
				if (temp2.getYear() == -1) {
					cout << "";
				}
				else {
					cout << "";
				}
			}
			end = chrono::steady_clock::now();
			chrono::duration<double> fib_time = end - start;

			cout << "\nJump Search Algorithm Time: " << jump_time.count() << " s" << endl;
			cout << "Fibonacci Search Algorithm Time: " << fib_time.count() << " s\n" << endl;
		}
		else if (choice == 5) {
			cout << "Insert game title:" << endl;
			string temp3;
			getline(cin, temp3);
			cout << "Insert year: " << endl;
			int temp;
			cin >> temp;
			auto start = chrono::steady_clock::now();
			for (int i = 1; i < 13; i++) {
				Game temp2 = jumpSearch(gameMap, temp3, i, temp);
				if (temp2.getYear() == -1) {
					cout << i << ": No Info For This Month" << endl;
				}
				else {
					cout << i << ": " << temp2.getAvgPeak() << endl;
				}
			}
			auto end = chrono::steady_clock::now();
			chrono::duration<double> jump_time = end - start;

			start = chrono::steady_clock::now();
			for (int i = 1; i < 13; i++) {
				Game temp2 = FibonacciSearch(gameMap, temp3, i, temp);
				if (temp2.getYear() == -1) {
					cout << "";
				}
				else {
					cout << "";
				}
			}
			end = chrono::steady_clock::now();
			chrono::duration<double> fib_time = end - start;

			cout << "\nJump Search Algorithm Time: " << jump_time.count() << " s" << endl;
			cout << "Fibonacci Search Algorithm Time: " << fib_time.count() << " s\n" << endl;
		}
		else if (choice == 6) {
			
		}
		else if (choice == 7) {
			cout << "Thanks for your time!" << endl;
		}
		else {
			cout << "invalid input" << endl;
		}
	}
	
	

	/*======= Print out how many sets were loaded =======*/
	int count = gameVector.size();
	cout << "Total number of sets: " << count << endl;

	/*======= Based on the choice, execute the appropriate task and show the results =======
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

	Game searchResult = FibonacciSearch(gameMap, "Counter-Strike: Global Offensive", 3, 2022);
	cout << searchResult.getMonth() << "/" << searchResult.getYear() << endl;

	auto start = chrono::steady_clock::now();
	test = top10GamesJump(gameMap, 9, 2012);
	auto end = chrono::steady_clock::now();
	for (int i = 0; i < test.size(); i++) {
		cout << i + 1 << ": " << test[i].getTitle() << " " << test[i].getAvg() << endl;
	}
	chrono::duration<double> jump_time = end - start;
	cout << "Time elapsed: " << jump_time.count() << " s" << endl;


	start = chrono::steady_clock::now();
	test = top10GamesFib(gameMap, 9, 2012);
	end = chrono::steady_clock::now();
	for (int i = 0; i < test.size(); i++) {
		cout << i + 1 << ": " << test[i].getTitle() << " " << test[i].getAvg() << endl;
	}
	chrono::duration<double> fib_time = end - start;
	cout << "Time elapsed: " << fib_time.count() << " s" << endl;

	*/

	
	
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