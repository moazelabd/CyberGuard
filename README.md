# 🔐 CyberGuard - CLI Password & Secret Manager

CyberGuard is a lightweight command-line application designed to securely manage passwords and sensitive information. The project is implemented in **C++** using **Object-Oriented Programming (OOP)** principles and integrates with an **SQLite** database for persistent storage.

---

## 🚀 Overview

CyberGuard provides a simple and secure way to store and manage credentials through a console-based interface. The system uses a **master password authentication mechanism** to protect access to all stored data.

---

## ✨ Features

* 🔑 **Master Password Authentication**

  * First-time setup and secure login system

* 🔒 **Password Storage**

  * Store credentials including title, username, password, and website

* 🔍 **Search Functionality**

  * Retrieve stored entries by title

* 🗑️ **Delete Entries**

  * Remove stored credentials from the database

* 📋 **View All Entries**

  * Display all stored passwords

* ✏️ **Update Password**

  * Modify an existing password entry

* 🔐 **Basic Encryption**

  * Passwords are stored using a simple XOR-based encryption (for educational purposes)

* 📅 **Automatic Timestamp**

  * Entries are saved with creation date automatically

* 🧠 **Password Strength Indicator**

  * Evaluates password strength (Weak / Medium / Strong)

* 🎲 **Password Generator**

  * Generates random strong passwords

---

## 🧱 Architecture

The project follows a modular OOP design:

* **User** → Handles authentication and master password logic
* **Vault** → Manages high-level operations (add, search, update, delete)
* **Database** → Handles all SQLite operations
* **Secret (Base Class)** → Abstract representation of stored data
* **PasswordEntry** → Derived class representing password records
* **Utils** → Helper functions (encryption, date, validation, generation)

---

## 💾 Database

* Uses **SQLite (embedded database)**
* No installation or configuration required
* Automatically initializes on first run
* Data is stored in a local `.db` file

---

## ⚙️ Build & Run

### Requirements

* C++17 or later
* Visual Studio (or any C++ compiler)

### Steps

```bash
git clone https://github.com/moazelabd/CyberGuard.git
cd CyberGuard
```

Build and run using your preferred IDE (Visual Studio recommended).

---

## 🔐 Security Note

This project uses a **basic XOR encryption** mechanism for demonstration and educational purposes only.

> ⚠️ Not suitable for production-level security.

---

## 📁 Project Structure

```
CyberGuard/
├── include/        # Header files
├── src/            # Source files
├── third_party/    # SQLite library
├── db/             # Database file (ignored in Git)
```

---

## 📌 Notes

* The database file is excluded from version control using `.gitignore`
* All data is generated locally during runtime
* No external dependencies or servers are required

---

## 👨‍💻 Author

Developed as a systems programming and OOP project focusing on:

* Clean architecture
* Database integration
* Security fundamentals

---

## 📄 License

This project is intended for educational and personal use.
