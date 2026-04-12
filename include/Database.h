#pragma once

#include <string>

struct sqlite3;

class Database {
private:
    sqlite3* db;
    std::string dbPath;

public:
    Database(const std::string& path = "cyberguard.db");
    ~Database();

    bool open();
    void close();
    bool initialize();

    bool hasMasterPassword();
    bool saveMasterPassword(const std::string& password);
    std::string getMasterPassword();

    bool addPasswordEntry(
        const std::string& title,
        const std::string& createdAt,
        const std::string& username,
        const std::string& encryptedPassword,
        const std::string& websiteUrl
    );

    bool deletePasswordEntryByTitle(const std::string& title);
    void viewAllPasswordEntries() const;
    void searchPasswordEntryByTitle(const std::string& title) const;
    bool updatePasswordByTitle(const std::string& title, const std::string& newEncryptedPassword);

private:
    bool createTables();
    bool execute(const std::string& sql);
};