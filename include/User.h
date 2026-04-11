#pragma once

#include <string>

class Database;

class User {
private:
    Database& database;

public:
    User(Database& db);

    bool isFirstRun();
    bool setupMasterPassword(const std::string& password);
    bool login(const std::string& inputPassword);
};