#include "../include/User.h"
#include "../include/Database.h"
#include "../include/Utils.h"

User::User(Database& db) : database(db) {}

bool User::isFirstRun() {
    return !database.hasMasterPassword();
}

bool User::setupMasterPassword(const std::string& password) {
    std::string hashedPassword = Utils::hashString(password);
    return database.saveMasterPassword(hashedPassword);
}

bool User::login(const std::string& inputPassword) {
    std::string hashedInput = Utils::hashString(inputPassword);
    return hashedInput == database.getMasterPassword();
}