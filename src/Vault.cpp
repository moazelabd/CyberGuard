#include "../include/Vault.h"
#include "../include/Database.h"
#include "../include/Utils.h"

Vault::Vault(Database& db) : database(db) {}

bool Vault::addPassword(
    const std::string& title,
    const std::string& createdAt,
    const std::string& username,
    const std::string& password,
    const std::string& websiteUrl
) {
    std::string encryptedPassword = Utils::xorEncryptDecrypt(password);
    return database.addPasswordEntry(title, createdAt, username, encryptedPassword, websiteUrl);
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
bool Vault::updatePasswordByTitle(const std::string& title, const std::string& newPassword) {
    std::string encryptedPassword = Utils::xorEncryptDecrypt(newPassword);
    return database.updatePasswordByTitle(title, encryptedPassword);
}