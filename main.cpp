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
void getCVSData(string filePath, unordered_multimap<string, Game>& gameMap, unordered_set<string>& gameNames) {
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
			gameMap.emplace(title.substr(1, title.size() - 2), gameProperties); //insert game object into hash table with name as key
			gameNames.insert(title.substr(1, title.size() - 2)); //keep track of which game titles are in table with set
		}
	}
	else {
		cout << "file not opened." << endl;	//error.
	}
}


vector<Game> getBucketData(unordered_multimap<string, Game>& myMap, string _name) {
	int i = myMap.bucket(_name); //finds out which bucket of hash table game objects are in 
	vector<Game> temp;
	for (auto iter = myMap.begin(i); iter != myMap.end(i); ++iter) {
		temp.push_back(iter->second); //creates a vector of all elements in that bucket of hash table
	}
	return temp;
}

Game jumpSearch(unordered_multimap<string, Game>& myMap, string _name, int _month, int _year) {
	vector<Game> games = getBucketData(myMap, _name);
	int m = sqrt(games.size()); //optimal value obtained from geeks for geeks explanation of jump search

	for (int i = 0; i < games.size(); i += m) { //index incremented by m rather than 1
		if (games[i].getYear() <= _year) {
			if (games[i].getMonth() < _month) {
				i -= m; //went too far in vector so go back a step
				while (i < i + m) { //linear search until element is found
					if (games[i].getYear() == _year && games[i].getMonth() == _month) {
						return games[i];
					}
					i++;
				}
				//if gets outside of while loop, it's not in table
			}
			else { //year is correct but month is greater than target
				while (i < i + 12) { //12 months in a year
					if (i < games.size()) {
						if (games[i].getYear() == _year && games[i].getMonth() == _month) {
							return games[i];
						}
						i++;
					}
					else {
						//i has grown too large and is no longer in vector range, data not in table
						Game temp(2012); //sets year to -1 for future checks
						return temp; 
					}
				}
			}
		}
	}
	Game temp(2012); //sets year to -1
	return temp;
}


/*======= Perform Fibonacci Search algorithm; derived from https://www.geeksforgeeks.org/fibonacci-search/ =======*/
/*======= Modified to support edge cases and a reverse sorted vector =======*/
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

		// Fibonacci method of stepping through the vector
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

		// Ensures both month and year match
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
	int operator() (Game& _game1, Game& _game2) { //max heap comparator using Avg players as comparison
		return _game1.getAvg() < _game2.getAvg();
	}
};

	
vector<Game> top10GamesJump(unordered_multimap<string, Game>& myMap, int _month, int _year) { //determines top 10 most popular games in specified month and year
	priority_queue<Game, vector<Game>, myComparator> maxHeap;
	vector<Game> games;

	for (int i = 0; i < myMap.bucket_count(); ++i) { //checks all the buckets
		auto iter = myMap.begin(i);
		if (iter != myMap.end(i)) { //bucket is not empty
			Game temp = jumpSearch(myMap, iter->first, _month, _year);
			if (temp.getYear() != -1) { //false game object created during jumpsearch
				maxHeap.push(temp); //actual game object with specified month and year is pushed into max heap
			}
		}
	}

	int temp = 10;

	while (temp > 0) {
		Game temp2 = maxHeap.top(); 
		games.push_back(temp2); //push 10 most popular games into vector and return it
		maxHeap.pop();
		temp--;
	}

	return games;
}

vector<Game> top10GamesFib(unordered_multimap<string, Game>& myMap, int _month, int _year) { //same function as above, but using fibonacci search instead of jump search to compare them 
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

vector<Game> top10AllTime(unordered_multimap<string, Game>& myMap) { //iterates through entire table to find greatest player bases at any given time
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

vector<Game> top10AllTimeNoDupes(unordered_multimap<string, Game>& myMap) { //same function as above but doesn't allow same game to occur again in leaderboard
	priority_queue<Game, vector<Game>, myComparator> maxHeap;
	vector<Game> games;
	unordered_set<string> names; //keeps track of which games are already in the leaderboard

	for (auto iter = myMap.begin(); iter != myMap.end(); ++iter) {
		if (iter->second.getYear() != -1) {
			maxHeap.push(iter->second);
		}
	}

	int temp = 10;

	while (temp > 0) {
		Game temp2 = maxHeap.top();
		if (names.find(temp2.getTitle()) == names.end()) { //that game is not in leaderboard
			games.push_back(temp2);
			maxHeap.pop();
			temp--;
			names.insert(temp2.getTitle());
		}
		else { //already in leaderboard so not pushed into vector again
			maxHeap.pop();
		}
	}

	return games;
}

bool validYearInput(string _year) { //ensures year inputs are between 2012 and 2021
	try {
		int temp = stoi(_year);
	}
	catch(...) {
		cout << "Please input an integer.\n" << endl;
		return false;
	}
	if (_year > "2021") {
		cout << "Latest data is from 2021.\n" << endl;
		return false;
	}
	else if (_year < "2012") {
		cout << "Earliest data starts from 2012.\n" << endl;
		return false;
	}
	else {
		return true;
	}
}

bool validInput(string _month, string _year) {
	try {
		int temp = stoi(_month);
	}
	catch (...) {
		cout << "Please input an integer.\n" << endl;
		return false;
	}
	if (validYearInput(_year) == true) {
		if (_year == "2012") {
			if (_month > "6" && _month < "13") { //earliest data in table is from 7/2012
				return true;
			}
			else {
				cout << "Earliest 2012 data starts from July (7).\n" << endl;
				return false;
			}
		}
		else if (_year == "2021") {
			if (_month > "0" && _month < "3") { //latest data in table is from 2/2021
				return true;
			}
			else {
				cout << "Latest 2021 data is from February (2).\n" << endl;
			}
		}
		else {
			if (_month > "0" && _month < "13") {
				return true;
			}
			else { //incorrect month input
				cout << "Month should be an integer between 1 and 12.\n" << endl; 
				return false;
			}
		}
	}
}

bool validGameTitle(string name, unordered_set<string>& names) {
	if (names.find(name) != names.end()) { //checks to see inputted game title is in set of names created during initialization
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
	unordered_multimap<string, Game> gameMap;
	unordered_set<string> names;
	getCVSData("data/steamcharts.csv", gameMap, names);

	string choice = "1";
	while (choice != "7") { //end program if user inputs 7 during menu
		cout << "1. Game Information\n"; //choices for user to choose from
		cout << "2. Highest Average Playerbase\n";
		cout << "3. Monthly Gain in Playerbase\n";
		cout << "4. Monthly Peak in Playerbase\n";
		cout << "5. Average Peak Percentage in Playerbase\n";
		cout << "6. All-time Records\n";
		cout << "7. End Program\n\n";
		
		getline(cin, choice); //clears newline character.

		if (choice == "1") {
			string temp;
			cout << "Insert game title: " << endl;
			getline(cin, temp);
			if (validGameTitle(temp, names) == true) { //ensure game is in table
				cout << "Insert year: " << endl;
				string temp2;
				getline(cin, temp2);
				cout << "Insert month: " << endl;
				string temp3;
				getline(cin, temp3);
				if (validInput(temp3, temp2) == true) { //ensure month and year inputs make sense
					int temp6 = stoi(temp2);
					int temp7 = stoi(temp3);
					auto start = chrono::steady_clock::now(); //notes the time before jump search begins
					Game temp4 = jumpSearch(gameMap, temp, temp7, temp6);
					auto end = chrono::steady_clock::now(); //notes time after jump search ends
					chrono::duration<double> jump_time = end - start; //the difference is what is output to the user

					start = chrono::steady_clock::now();
					Game temp5 = FibonacciSearch(gameMap, temp, temp7, temp6);
					end = chrono::steady_clock::now();
					chrono::duration<double> fib_time = end - start;
					if (temp4.getYear() == -1) {
						cout << "Game data not found for this date." << endl; //game came out later than that date
					}
					else {
						cout << temp4.getTitle() << endl;
						cout << "Average Player Count: " << temp4.getAvg() << endl;
						cout << "Peak Player Count: " << temp4.getPeak() << endl;
						cout << "Gain from Previous Month: " << temp4.getGain() << endl;
						cout << "Average/Peak Percentage: " << temp4.getAvgPeak() << endl;
						cout << "\nJump Search Algorithm Time: " << jump_time.count() << " s" << endl; //outputs both search times for comparison
						cout << "Fibonacci Search Algorithm Time: " << fib_time.count() << " s\n" << endl;
					}
				}
			}
		}
		else if (choice == "2") { //top 10 games in user inputted year and month
			string temp;
			string temp2;
			cout << "Insert year: " << endl;
			getline(cin, temp);
			cout << "Insert month: " << endl;
			getline(cin, temp2);
			cout << endl;
			if (validInput(temp2, temp) == true) {
				int num = stoi(temp);
				int num2 = stoi(temp2);
				auto start = chrono::steady_clock::now();
				vector<Game> test = top10GamesFib(gameMap, num2, num);
				auto end = chrono::steady_clock::now();
				chrono::duration<double> jump_time = end - start;
				start = chrono::steady_clock::now();
				test = top10GamesFib(gameMap, num2, num);
				end = chrono::steady_clock::now();
				for (int i = 0; i < test.size(); i++) {
					cout << i + 1 << ": " << test[i].getTitle() << " - " << test[i].getAvg() << endl;
				}
				chrono::duration<double> fib_time = end - start;
				cout << "\nJump Search Algorithm Time: " << jump_time.count() << " s" << endl;
				cout << "Fibonacci Search Algorithm Time: " << fib_time.count() << " s\n" << endl;
			}
		}
		else if (choice == "3") { //12 months of gain for specified year
			cout << "Insert game title:" << endl;
			string temp3;
			getline(cin, temp3);
			if (validGameTitle(temp3, names) == true) {
				cout << "Insert year: " << endl;
				string temp;
				getline(cin, temp);
				cout << endl;
				if (validYearInput(temp) == true) {
					int num = stoi(temp);
					auto start = chrono::steady_clock::now();
					for (int i = 1; i < 13; i++) {
						if (num == 2021 && i > 2) {
							cout << i << ": No Info For This Month" << endl;
						}
						else {
							Game temp2 = jumpSearch(gameMap, temp3, i, num);
							if (temp2.getYear() == -1) {
								cout << i << ": No Info For This Month" << endl;
							}
							else {
								cout << i << ": " << temp2.getGain() << endl;
							}
						}
					}
					auto end = chrono::steady_clock::now();
					chrono::duration<double> jump_time = end - start;

					start = chrono::steady_clock::now();
					for (int i = 1; i < 13; i++) {
						Game temp2 = FibonacciSearch(gameMap, temp3, i, num);
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
		else if (choice == "4") { // 12 months of peak for specified year
			cout << "Insert game title:" << endl;
			string temp3;
			getline(cin, temp3);
			if (validGameTitle(temp3, names) == true) {
				cout << "Insert year: " << endl;
				string temp;
				getline(cin, temp);
				cout << endl;
				if (validYearInput(temp) == true) {
					int num = stoi(temp);
					auto start = chrono::steady_clock::now();
					for (int i = 1; i < 13; i++) {
						if (num == 2021 && i > 2) {
							cout << i << ": No Info For This Month" << endl;
						}
						else {
							Game temp2 = jumpSearch(gameMap, temp3, i, num);
							if (temp2.getYear() == -1) {
								cout << i << ": No Info For This Month" << endl;
							}
							else {
								cout << i << ": " << temp2.getPeak() << endl; //same as 3 but output peak rather than gain
							}
						}
					}
					auto end = chrono::steady_clock::now();
					chrono::duration<double> jump_time = end - start;

					start = chrono::steady_clock::now();
					for (int i = 1; i < 13; i++) {
						Game temp2 = FibonacciSearch(gameMap, temp3, i, num);
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
		else if (choice == "5") { //12 months of percentage for specified year
			cout << "Insert game title:" << endl;
			string temp3;
			getline(cin, temp3);
			if (validGameTitle(temp3, names) == true) {
				cout << "Insert year: " << endl;
				string temp;
				getline(cin, temp);
				cout << endl;
				if (validYearInput(temp) == true) {
					int num = stoi(temp);
					auto start = chrono::steady_clock::now();
					for (int i = 1; i < 13; i++) {
						if (num == 2021 && i > 2) {
							cout << i << ": No Info For This Month" << endl;
						}
						else {
							Game temp2 = jumpSearch(gameMap, temp3, i, num);
							if (temp2.getYear() == -1) {
								cout << i << ": No Info For This Month" << endl;
							}
							else {
								cout << i << ": " << temp2.getAvgPeak() << endl; //same as 4 but output percentage rather than peak
							}
						}
					}
					auto end = chrono::steady_clock::now();
					chrono::duration<double> jump_time = end - start;

					start = chrono::steady_clock::now();
					for (int i = 1; i < 13; i++) {
						Game temp2 = FibonacciSearch(gameMap, temp3, i, num);
						if (temp2.getYear() == -1) {
							cout << ""; //keep output statements empty to keep similar time constraints as jump search
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
		else if (choice == "6") { //all time highest player base games
			cout << "Should duplicates be allowed? Y/N" << endl; //PUBG has 9 slots in leaderboard normally
			string temp;
			getline(cin, temp);
			if (temp != "Y" && temp != "N") {
				cout << "Invalid input!" << endl;
			}
			else if (temp == "Y") {
				cout << endl;
				vector<Game> test = top10AllTime(gameMap);
				for (int i = 0; i < test.size(); i++) {
					cout << i + 1 << ": " << test[i].getTitle() << " - " << test[i].getAvg() << " - " << test[i].getMonth() << "/" << test[i].getYear() << endl;
				}
				cout << endl;
			}
			else {
				cout << endl;
				vector<Game> test = top10AllTimeNoDupes(gameMap);
				for (int i = 0; i < test.size(); i++) {
					cout << i + 1 << ": " << test[i].getTitle() << " - " << test[i].getAvg() << " - " << test[i].getMonth() << "/" << test[i].getYear() << endl;
				}
				cout << endl;
			}
		}
		else if (choice == "7") { //exit while loop
			cout << "\nThanks for your time!" << endl;
		}
		else { //prevents incorrect choice inputs
			cout << "Not a valid choice input!" << endl << endl;
		}
	}
	
	return 0;
}