# 🔐 CyberGuard - Password & Secret Manager (CLI)

CyberGuard is a lightweight and secure command-line application for managing passwords and sensitive information using modern **C++ OOP principles** and **SQLite**.

---

## 🚀 Features

* 🔑 Master Password Authentication
* 🔒 Secure Storage of Passwords
* 🧠 Clean Object-Oriented Design
* 📂 SQLite Database (No setup required)
* 🔍 Search Secrets by Title
* 🗑️ Delete Stored Entries
* 📋 View All Stored Secrets

---

## 🧱 Project Structure

```
CyberGuard/
│
├── include/        # Header files (.h)
├── src/            # Source files (.cpp)
├── db/             # SQLite database file
├── third_party/    # External libraries (SQLite)
│
├── CMakeLists.txt
├── README.md
```

---

## 🧠 OOP Concepts Used

* **Encapsulation** → Sensitive data (passwords) are private
* **Inheritance** → `PasswordEntry` inherits from `Secret`
* **Polymorphism** → Virtual methods like `viewDetails()`
* **Abstraction** → Base class `Secret`

---

## 💾 Database

* Uses **SQLite (embedded database)**

* Database file:

  ```
  db/cyberguard.db
  ```

* Automatically created on first run

* No installation or configuration required

---

## 🛠️ Requirements

* C++17 or later
* Visual Studio / VS Code
* CMake (optional but recommended)

---

## ⚙️ Build & Run

### ▶️ Using Visual Studio

1. Open solution file:

   ```
   CyberGuard.slnx
   ```
2. Build the project
3. Run the program

---

### ▶️ Using CMake (optional)

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

---

## 🔐 First Run

* On first launch:

  * You will be asked to set a **Master Password**
* This password is required to access the vault

---

## 📌 Usage

Main Menu:

```
1. Add Password Entry
2. View All Secrets
3. Search by Title
4. Remove Secret
5. Exit
```

---

## ⚠️ Security Note

This project uses **basic encryption for educational purposes**.

For production-level security:

* Use strong encryption libraries (e.g., OpenSSL)
* Implement hashing (e.g., bcrypt, SHA-256)

---

## 📦 Distribution

CyberGuard is designed to be:

* ✅ Portable
* ✅ Easy to use
* ✅ No database setup required

Simply share:

```
CyberGuard.exe + db folder
```

---

## 👨‍💻 Author

Developed by a CIE student focused on:

* Systems Programming
* Cybersecurity Concepts
* Clean Software Architecture

---

## ⭐ Future Improvements

* AES Encryption
* GUI Version
* Auto password generator
* File encryption support

---

## 📄 License

This project is for educational and personal use.
Feel free to modify and extend.

---
