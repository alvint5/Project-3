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
#include <unordered_set>
#include "Game.h"
using namespace std;

//function to retrieve data from CVS files, designed for Steam games.
//implemented personal code from previous lab assignments.
void getCVSData(string filePath, vector<Game>& gameVector, unordered_multimap<string, Game>& gameMap, unordered_set<string>& gameNames) {
	ifstream inFile(filePath);

	if (inFile.is_open()) {
		string line;
		getline(inFile, line);

		while (getline(inFile, line)) {
			string temp;
			string temp2;
			string title;
			int year;
			int month;
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

			//push onto data structures.
			Game gameProperties(title.substr(1, title.size() - 2), year, month, avg, gain, peak, avgpeak.substr(1, avgpeak.size() - 2));
			gameVector.push_back(gameProperties);
			gameMap.emplace(title.substr(1, title.size() - 2), gameProperties);
			gameNames.insert(title.substr(1, title.size() - 2));
		}
	}
	else {
		cout << "file not opened." << endl;	//error.
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

vector<Game> top10AllTimeNoDupes(unordered_multimap<string, Game>& myMap) {
	priority_queue<Game, vector<Game>, myComparator> maxHeap;
	vector<Game> games;
	unordered_set<string> names;

	for (auto iter = myMap.begin(); iter != myMap.end(); ++iter) {
		if (iter->second.getYear() != -1) {
			maxHeap.push(iter->second);
		}
	}

	int temp = 10;

	while (temp > 0) {
		Game temp2 = maxHeap.top();
		if (names.find(temp2.getTitle()) == names.end()) {
			games.push_back(temp2);
			maxHeap.pop();
			temp--;
			names.insert(temp2.getTitle());
		}
		else {
			maxHeap.pop();
		}
	}

	return games;
}

bool validYearInput(int _year) {
	if (_year > 2021) {
		cout << "Latest data is from 2021.\n" << endl;
		return false;
	}
	else if (_year < 2012) {
		cout << "Earliest data starts from 2012.\n" << endl;
		return false;
	}
	else {
		return true;
	}
}

bool validInput(int _month, int _year) {
	if (validYearInput(_year) == true) {
		if (_year == 2012) {
			if (_month > 6 && _month < 13) {
				return true;
			}
			else {
				cout << "Earliest 2012 data starts from July (7).\n" << endl;
				return false;
			}
		}
		else if (_year == 2021) {
			if (_month > 0 && _month < 3) {
				return true;
			}
			else {
				cout << "Latest 2021 data is from February (2).\n" << endl;
			}
		}
		else {
			if (_month > 0 && _month < 13) {
				return true;
			}
			else {
				cout << "Month should be an integer between 1 and 12.\n" << endl;
				return false;
			}
		}
	}
}

bool validGameTitle(string name, unordered_set<string>& names) {
	if (names.find(name) != names.end()) {
		return true;
	}
	else {
		cout << "Game was not found in table.\n" << endl;
		return false;
	}
}

int main()
{
	cout << std::fixed << setprecision(5);
	cout << "Welcome User! One second while we retrieve our data.\n" << endl;

	/*======= Load data from file(s) =======*/
	vector<Game> gameVector;
	unordered_multimap<string, Game> gameMap;
	unordered_set<string> names;
	getCVSData("data/steamcharts.csv", gameVector, gameMap, names);

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
			if (validGameTitle(temp, names) == true) {
				cout << "Insert year: " << endl;
				int temp2;
				cin >> temp2;
				cout << "Insert month: " << endl;
				int temp3;
				cin >> temp3;
				if (validInput(temp3, temp2) == true) {
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
						cout << "\nJump Search Algorithm Time: " << jump_time.count() << " s" << endl;
						cout << "Fibonacci Search Algorithm Time: " << fib_time.count() << " s\n" << endl;
					}
				}
			}
		}
		else if (choice == 2) {
			int temp;
			int temp2;
			cout << "Insert year: " << endl;
			cin >> temp;
			cout << "Insert month: " << endl;
			cin >> temp2;
			cout << endl;
			if (validInput(temp2, temp) == true) {
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
		}
		else if (choice == 3) {
			cout << "Insert game title:" << endl;
			string temp3;
			getline(cin, temp3);
			if (validGameTitle(temp3, names) == true) {
				cout << "Insert year: " << endl;
				int temp;
				cin >> temp;
				if (validYearInput(temp) == true) {
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
			}
		}
		else if (choice == 4) {
			cout << "Insert game title:" << endl;
			string temp3;
			getline(cin, temp3);
			if (validGameTitle(temp3, names) == true) {
				cout << "Insert year: " << endl;
				int temp;
				cin >> temp;
				if (validYearInput(temp) == true) {
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
			}
		}
		else if (choice == 5) {
			cout << "Insert game title:" << endl;
			string temp3;
			getline(cin, temp3);
			if (validGameTitle(temp3, names) == true) {
				cout << "Insert year: " << endl;
				int temp;
				cin >> temp;
				if (validYearInput(temp) == true) {
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
			}
		}
		else if (choice == 6) {
			cout << "Should duplicates be allowed? Y/N" << endl;
			string temp;
			getline(cin, temp);
			if (temp != "Y" && temp != "N") {
				cout << "Invalid input!" << endl;
			}
			else if (temp == "Y") {
				vector<Game> test = top10AllTime(gameMap);
				for (int i = 0; i < test.size(); i++) {
					cout << i + 1 << ": " << test[i].getTitle() << " - " << test[i].getAvg() << " - " << test[i].getMonth() << "/" << test[i].getYear() << endl;
				}
				cout << endl;
			}
			else {
				vector<Game> test = top10AllTimeNoDupes(gameMap);
				for (int i = 0; i < test.size(); i++) {
					cout << i + 1 << ": " << test[i].getTitle() << " - " << test[i].getAvg() << " - " << test[i].getMonth() << "/" << test[i].getYear() << endl;
				}
				cout << endl;
			}
		}
		else if (choice == 7) {
			cout << "Thanks for your time!" << endl;
		}
		else {
			cout << "Not a valid choice input!" << endl;
		}
	}
	
	return 0;
}