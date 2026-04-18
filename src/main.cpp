#include "../include/Database.h"
#include "../include/User.h"
#include "../include/Vault.h"
#include "../include/Utils.h"

#include <iostream>
#include <string>
#include <limits>

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
        std::cout << "5. Update Password by Title\n";
        std::cout << "6. Generate Strong Password\n";
        std::cout << "7. Exit\n";
        std::cout << "Enter choice: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
        case 1:
            std::cout << "Title: ";
            std::getline(std::cin, title);

            if (Utils::isEmpty(title)) {
                std::cout << "Title cannot be empty.\n";
                break;
            }

            std::cout << "Username: ";
            std::getline(std::cin, username);

            if (Utils::isEmpty(username)) {
                std::cout << "Username cannot be empty.\n";
                break;
            }

            std::cout << "Password: ";
            std::getline(std::cin, entryPassword);

            if (Utils::isEmpty(entryPassword)) {
                std::cout << "Password cannot be empty.\n";
                break;
            }

            std::cout << "Password Strength: "
                << Utils::checkPasswordStrength(entryPassword) << "\n";

            if (!Utils::isStrongEnough(entryPassword)) {
                std::cout << "Password is too weak. Please choose a stronger password.\n";
                break;
            }

            std::cout << "Website URL: ";
            std::getline(std::cin, websiteUrl);

            createdAt = Utils::getCurrentDate();

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

            char confirm;
            std::cout << "Are you sure you want to delete this entry? (y/n): ";
            std::cin >> confirm;
            std::cin.ignore();

            if (confirm == 'y' || confirm == 'Y') {
                if (vault.removePasswordByTitle(title)) {
                    std::cout << "Password entry removed.\n";
                }
                else {
                    std::cout << "Failed to remove password entry.\n";
                }
            }
            else {
                std::cout << "Deletion cancelled.\n";
            }

            break;

        case 5:
            std::cout << "Enter title to update: ";
            std::getline(std::cin, title);

            if (Utils::isEmpty(title)) {
                std::cout << "Title cannot be empty.\n";
                break;
            }

            std::cout << "Enter new password: ";
            std::getline(std::cin, entryPassword);

            if (Utils::isEmpty(entryPassword)) {
                std::cout << "Password cannot be empty.\n";
                break;
            }

            std::cout << "Password Strength: "
                << Utils::checkPasswordStrength(entryPassword) << "\n";

            if (!Utils::isStrongEnough(entryPassword)) {
                std::cout << "Password is too weak. Please choose a stronger password.\n";
                break;
            }

            if (vault.updatePasswordByTitle(title, entryPassword)) {
                std::cout << "Password updated successfully.\n";
            }
            else {
                std::cout << "Failed to update password.\n";
            }
            break;

        case 6:
            std::cout << "Generated Password: "
                << Utils::generatePassword(12) << "\n";
            break;

        case 7:
            std::cout << "Goodbye.\n";
            break;

        default:
            std::cout << "Invalid choice.\n";
        }

    } while (choice != 7);

    return 0;
}