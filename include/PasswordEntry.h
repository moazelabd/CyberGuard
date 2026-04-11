#pragma once

#include "Secret.h"
#include <string>

class PasswordEntry : public Secret {
private:
    std::string username;
    std::string encryptedPassword;
    std::string websiteUrl;

public:
    PasswordEntry(
        int id,
        const std::string& title,
        const std::string& createdAt,
        const std::string& username,
        const std::string& encryptedPassword,
        const std::string& websiteUrl
    );

    std::string getUsername() const;
    std::string getEncryptedPassword() const;
    std::string getWebsiteUrl() const;

    void viewDetails() const override;
};