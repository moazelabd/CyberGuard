#include "../include/User.h"
#include "../include/Database.h"

User::User(Database& db) : database(db) {}

bool User::isFirstRun() {
    return !database.hasMasterPassword();
}

bool User::setupMasterPassword(const std::string& password) {
    return database.saveMasterPassword(password);
}

bool User::login(const std::string& inputPassword) {
    return inputPassword == database.getMasterPassword();
}