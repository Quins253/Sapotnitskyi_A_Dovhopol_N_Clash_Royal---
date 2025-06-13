#pragma once
#include <string>
#include <unordered_map>

using namespace std;

class profileManager {
public:
    static bool activeUser();
    static string getActiveUser();
    static void setActiveUser(const std::string& username);
    static void clearActiveUser();
    static void updateBattleResult(const std::string& username, bool win);

    static void savedUserData(const string& username, const string& key, const string& val);
    static string loadUserData(const string& username, const string& key);
};