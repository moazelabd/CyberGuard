#pragma once

#include <string>

class Database;

class Vault {
private:
    Database& database;

public:
    Vault(Database& db);

    bool addPassword(
        const std::string& title,
        const std::string& createdAt,
        const std::string& username,
        const std::string& password,
        const std::string& websiteUrl
    );

    void viewAllPasswords() const;
    void searchPasswordByTitle(const std::string& title) const;
    bool removePasswordByTitle(const std::string& title);
};