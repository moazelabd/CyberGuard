#include "../include/PasswordEntry.h"
#include <iostream>

PasswordEntry::PasswordEntry(
    int id,
    const std::string& title,
    const std::string& createdAt,
    const std::string& username,
    const std::string& encryptedPassword,
    const std::string& websiteUrl
)
    : Secret(id, title, createdAt),
    username(username),
    encryptedPassword(encryptedPassword),
    websiteUrl(websiteUrl) {}

std::string PasswordEntry::getUsername() const {
    return username;
}

std::string PasswordEntry::getEncryptedPassword() const {
    return encryptedPassword;
}

std::string PasswordEntry::getWebsiteUrl() const {
    return websiteUrl;
}

void PasswordEntry::viewDetails() const {
    std::cout << "\n--- Password Entry ---\n";
    std::cout << "ID: " << id << "\n";
    std::cout << "Title: " << title << "\n";
    std::cout << "Created At: " << createdAt << "\n";
    std::cout << "Username: " << username << "\n";
    std::cout << "Password: " << encryptedPassword << "\n";
    std::cout << "Website: " << websiteUrl << "\n";
}