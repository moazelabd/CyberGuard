#include "../include/Vault.h"
#include "../include/Database.h"

Vault::Vault(Database& db) : database(db) {}

bool Vault::addPassword(
    const std::string& title,
    const std::string& createdAt,
    const std::string& username,
    const std::string& password,
    const std::string& websiteUrl
) {
    return database.addPasswordEntry(title, createdAt, username, password, websiteUrl);
}

void Vault::viewAllPasswords() const {
    database.viewAllPasswordEntries();
}

void Vault::searchPasswordByTitle(const std::string& title) const {
    database.searchPasswordEntryByTitle(title);
}

bool Vault::removePasswordByTitle(const std::string& title) {
    return database.deletePasswordEntryByTitle(title);
}