#include "../include/Database.h"
#include "../third_party/sqlite/sqlite3.h"
#include "../include/Utils.h"
#include <iostream>

Database::Database(const std::string& path)
    : db(nullptr), dbPath(path) {}

Database::~Database() {
    close();
}

bool Database::open() {
    if (db != nullptr) {
        return true;
    }

    int result = sqlite3_open(dbPath.c_str(), &db);

    if (result != SQLITE_OK) {
        std::cerr << "Failed to open database.\n";
        close();
        return false;
    }

    std::cout << "Database opened successfully.\n";
    return true;
}

void Database::close() {
    if (db != nullptr) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool Database::initialize() {
    if (!open()) {
        return false;
    }

    return createTables();
}

bool Database::execute(const std::string& sql) {
    char* errorMessage = nullptr;

    int result = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errorMessage);

    if (result != SQLITE_OK) {
        std::cerr << "SQL error: "
            << (errorMessage ? errorMessage : "Unknown error")
            << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }

    return true;
}

bool Database::createTables() {
    const std::string createUsersTable = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            master_password TEXT NOT NULL
        );
    )";

    const std::string createPasswordEntriesTable = R"(
        CREATE TABLE IF NOT EXISTS password_entries (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            created_at TEXT NOT NULL,
            username TEXT NOT NULL,
            encrypted_password TEXT NOT NULL,
            website_url TEXT
        );
    )";

    return execute(createUsersTable) && execute(createPasswordEntriesTable);
}
bool Database::hasMasterPassword() {
    const char* sql = "SELECT COUNT(*) FROM users;";
    sqlite3_stmt* stmt = nullptr;
    int count = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return count > 0;
}

bool Database::saveMasterPassword(const std::string& password) {
    const char* sql = "INSERT INTO users (master_password) VALUES (?);";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, password.c_str(), -1, SQLITE_TRANSIENT);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    return success;
}

std::string Database::getMasterPassword() {
    const char* sql = "SELECT master_password FROM users LIMIT 1;";
    sqlite3_stmt* stmt = nullptr;
    std::string password;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return "";
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* text = sqlite3_column_text(stmt, 0);
        if (text != nullptr) {
            password = reinterpret_cast<const char*>(text);
        }
    }

    sqlite3_finalize(stmt);
    return password;
}
bool Database::addPasswordEntry(
    const std::string& title,
    const std::string& createdAt,
    const std::string& username,
    const std::string& encryptedPassword,
    const std::string& websiteUrl
) {
    const char* sql =
        "INSERT INTO password_entries "
        "(title, created_at, username, encrypted_password, website_url) "
        "VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, createdAt.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, encryptedPassword.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, websiteUrl.c_str(), -1, SQLITE_TRANSIENT);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    return success;
}

bool Database::deletePasswordEntryByTitle(const std::string& title) {
    const char* sql = "DELETE FROM password_entries WHERE title = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    return success;
}

void Database::viewAllPasswordEntries() const {
    bool hasData = false;
    const char* sql =
        "SELECT id, title, created_at, username, encrypted_password, website_url "
        "FROM password_entries;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cout << "Failed to fetch entries.\n";
        return;
    }

    std::cout << "\n=== All Password Entries ===\n";

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        hasData = true;
        std::cout << "ID: " << sqlite3_column_int(stmt, 0) << "\n";

        std::cout << "Title: "
            << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)) << "\n";

        std::cout << "Created At: "
            << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) << "\n";

        std::cout << "Username: "
            << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) << "\n";

        const unsigned char* raw = sqlite3_column_text(stmt, 4);
        std::string encryptedPassword = raw ? reinterpret_cast<const char*>(raw) : "";
        std::string decryptedPassword = Utils::xorEncryptDecrypt(encryptedPassword);

        // Hide password
        std::string hidden(decryptedPassword.length(), '*');
        std::cout << "Password: " << hidden << "\n";

        // Ask user if they want to reveal
        char show;
        std::cout << "display hidden? (y/n): ";
        std::cin >> show;
        std::cin.ignore();

        if (show == 'y' || show == 'Y') {
            std::cout << "Actual Password: " << decryptedPassword << "\n";
        }

        std::cout << "Website: "
            << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)) << "\n";

        std::cout << "-----------------------------\n";
    }
    if (hasData==false) {
        std::cout << "No password entries found.\n";
    }
    sqlite3_finalize(stmt);
}

void Database::searchPasswordEntryByTitle(const std::string& title) const {
    const char* sql =
        "SELECT id, title, created_at, username, encrypted_password, website_url "
        "FROM password_entries WHERE title = ?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cout << "Search failed.\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);

    bool found = false;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        found = true;

        std::cout << "\n=== Search Result ===\n";
        std::cout << "ID: " << sqlite3_column_int(stmt, 0) << "\n";
        std::cout << "Title: " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)) << "\n";
        std::cout << "Created At: " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) << "\n";
        std::cout << "Username: " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) << "\n";

        const unsigned char* raw = sqlite3_column_text(stmt, 4);
        std::string encryptedPassword = raw ? reinterpret_cast<const char*>(raw) : "";
        std::string decryptedPassword = Utils::xorEncryptDecrypt(encryptedPassword);

        std::string hidden(decryptedPassword.length(), '*');
        std::cout << "Password: " << hidden << "\n";

        char show;
        std::cout << "Show password? (y/n): ";
        std::cin >> show;
        std::cin.ignore();

        if (show == 'y' || show == 'Y') {
            std::cout << "Actual Password: " << decryptedPassword << "\n";
        }

        std::cout << "Website: " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)) << "\n";
        std::cout << "-----------------------------\n";
    }

    if (!found) {
        std::cout << "No entry found with this title.\n";
    }

    sqlite3_finalize(stmt);
}
bool Database::updatePasswordByTitle(const std::string& title, const std::string& newEncryptedPassword) {
    const char* sql =
        "UPDATE password_entries "
        "SET encrypted_password = ? "
        "WHERE title = ?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cout << "Failed to prepare update query.\n";
        return false;
    }

    sqlite3_bind_text(stmt, 1, newEncryptedPassword.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, title.c_str(), -1, SQLITE_TRANSIENT);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);

    if (!success) {
        std::cout << "Failed to update password.\n";
    }

    sqlite3_finalize(stmt);
    return success;
}
bool Database::deletePasswordById(int id) {
    const char* sql = "DELETE FROM password_entries WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, id);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);
    return success;
}
bool Database::updatePasswordById(int id, const std::string& newPassword) {
    std::string encrypted = Utils::xorEncryptDecrypt(newPassword);

    const char* sql = "UPDATE password_entries SET encrypted_password = ? WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, encrypted.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, id);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);
    return success;
}
