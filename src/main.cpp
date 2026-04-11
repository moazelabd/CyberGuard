#include "../include/Database.h"
#include "../include/User.h"
#include "../include/Vault.h"
#include <iostream>
#include <string>

int main() {
    Database db;

    if (!db.initialize()) {
        std::cout << "Database initialization failed.\n";
        return 1;
    }

    User user(db);
    std::string password;

    if (user.isFirstRun()) {
        std::cout << "First time setup\n";
        std::cout << "Enter new master password: ";
        std::getline(std::cin, password);

        if (!user.setupMasterPassword(password)) {
            std::cout << "Failed to save master password.\n";
            return 1;
        }

        std::cout << "Master password saved successfully.\n";
    }
    else {
        std::cout << "Enter master password: ";
        std::getline(std::cin, password);

        if (!user.login(password)) {
            std::cout << "Access denied.\n";
            return 1;
        }

        std::cout << "Login successful.\n";
    }

    Vault vault(db);

    int choice = 0;

    do {
        std::string title, createdAt, username, entryPassword, websiteUrl;

        std::cout << "\n=== CyberGuard Menu ===\n";
        std::cout << "1. Add Password Entry\n";
        std::cout << "2. View All Passwords\n";
        std::cout << "3. Search Password by Title\n";
        std::cout << "4. Remove Password by Title\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
        case 1:
            std::cout << "Title: ";
            std::getline(std::cin, title);

            std::cout << "Created At: ";
            std::getline(std::cin, createdAt);

            std::cout << "Username: ";
            std::getline(std::cin, username);

            std::cout << "Password: ";
            std::getline(std::cin, entryPassword);

            std::cout << "Website URL: ";
            std::getline(std::cin, websiteUrl);

            if (vault.addPassword(title, createdAt, username, entryPassword, websiteUrl)) {
                std::cout << "Password entry added successfully.\n";
            }
            else {
                std::cout << "Failed to add password entry.\n";
            }
            break;

        case 2:
            vault.viewAllPasswords();
            break;

        case 3:
            std::cout << "Enter title to search: ";
            std::getline(std::cin, title);
            vault.searchPasswordByTitle(title);
            break;

        case 4:
            std::cout << "Enter title to remove: ";
            std::getline(std::cin, title);

            if (vault.removePasswordByTitle(title)) {
                std::cout << "Password entry removed.\n";
            }
            else {
                std::cout << "Failed to remove password entry.\n";
            }
            break;

        case 5:
            std::cout << "Goodbye.\n";
            break;

        default:
            std::cout << "Invalid choice.\n";
        }

    } while (choice != 5);

    return 0;
}