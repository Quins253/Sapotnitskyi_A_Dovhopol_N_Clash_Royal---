#include "ManagerProfile.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <random>

using namespace std;
namespace fs = filesystem;

static const string ActiveUserFile = "data/active_user.txt";
static const string ProfilesDir = "data/profiles/";


//Check active profile
bool profileManager::activeUser() {
	ifstream file(ActiveUserFile);
	
	return file.good() && file.peek() != ifstream::traits_type::eof();
}

// Get file name(active user)
string profileManager::getActiveUser() {
	
	ifstream file(ActiveUserFile);
	string name;
	getline(file, name);
	
	return name;
}

void profileManager::setActiveUser(const string& username) {
	ofstream file(ActiveUserFile);
	file << username;
}

void profileManager::clearActiveUser() {
	ofstream file(ActiveUserFile, ios::trunc);
}

//Coins & Cup
void profileManager::updateBattleResult(const std::string& username, bool win) {
	int coins = stoi(loadUserData(username, "coins"));
	int trophies = stoi(loadUserData(username, "trophies"));

	int coinsEarned = win ? rand() % 11 + 20 : rand() % 6 + 5;
	int trophiesChange = win ? rand() % 6 + 26 : -26;

	coins += coinsEarned;

	if (!win && trophies + trophiesChange < 0)
		trophies = 0;
	else
		trophies += trophiesChange;

	savedUserData(username, "coins", to_string(coins));
	savedUserData(username, "trophies", to_string(trophies));
}


//Overwriting key and value mapping data
void profileManager::savedUserData(const string& username, const string& key, const string& val) {
	
	string path = ProfilesDir + username + ".txt";
	unordered_map<string, string> data;

	ifstream inFile(path);
	string line;
	while (getline(inFile, line)) {
		istringstream iss(line);
		string lineKey, lineVal;
		if (getline(iss, lineKey, '=') && getline(iss, lineVal)) {
			data[lineKey] = lineVal;
		}
	}
	inFile.close();

	data[key] = val;

	ofstream outfile(path);
	for (const auto& pair : data) {
		outfile << pair.first << '=' << pair.second << endl;
	}

}

string profileManager::loadUserData(const string& username, const string& key) {
	
	string path = ProfilesDir + username + ".txt";
	ifstream file(path);
	string line;

	while (getline(file, line)) {
		istringstream iss(line);
		string lineKey, lineVal;
		
		if (getline(iss, lineKey, '=') && getline(iss, lineVal)) {
			if (lineKey == key) {
				return lineVal;
			}
		}
	}
	return "";
}