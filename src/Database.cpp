#include "../include/Database.h"
#include "../third_party/sqlite/sqlite3.h"
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
        std::cout << "ID: " << sqlite3_column_int(stmt, 0) << "\n";
        std::cout << "Title: " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)) << "\n";
        std::cout << "Created At: " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) << "\n";
        std::cout << "Username: " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) << "\n";
        std::cout << "Password: " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)) << "\n";
        std::cout << "Website: " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)) << "\n";
        std::cout << "-----------------------------\n";
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
        std::cout << "Password: " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)) << "\n";
        std::cout << "Website: " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)) << "\n";
    }

    if (!found) {
        std::cout << "No entry found with this title.\n";
    }

    sqlite3_finalize(stmt);
}