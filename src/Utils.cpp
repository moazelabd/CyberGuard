#include "../include/Utils.h"

#include <ctime>
#include <iomanip>
#include <random>
#include <sstream>

namespace Utils {

    std::string xorEncryptDecrypt(const std::string& text, char key) {
        std::string result = text;

        for (char& ch : result) {
            ch ^= key;
        }

        return result;
    }

    std::string getCurrentDate() {
        std::time_t now = std::time(nullptr);
        std::tm localTime{};

#ifdef _WIN32
        localtime_s(&localTime, &now);
#else
        localTime = *std::localtime(&now);
#endif

        std::ostringstream out;
        out << std::put_time(&localTime, "%Y-%m-%d");
        return out.str();
    }

    std::string generatePassword(int length) {
        const std::string chars =
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "0123456789"
            "!@#$%^&*";

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, static_cast<int>(chars.size() - 1));

        std::string password;
        for (int i = 0; i < length; i++) {
            password += chars[dist(gen)];
        }

        return password;
    }

    std::string checkPasswordStrength(const std::string& password) {
        bool hasUpper = false;
        bool hasLower = false;
        bool hasDigit = false;
        bool hasSpecial = false;

        for (char ch : password) {
            if (std::isupper(static_cast<unsigned char>(ch))) {
                hasUpper = true;
            }
            else if (std::islower(static_cast<unsigned char>(ch))) {
                hasLower = true;
            }
            else if (std::isdigit(static_cast<unsigned char>(ch))) {
                hasDigit = true;
            }
            else {
                hasSpecial = true;
            }
        }

        int score = 0;
        if (password.length() >= 8) score++;
        if (hasUpper) score++;
        if (hasLower) score++;
        if (hasDigit) score++;
        if (hasSpecial) score++;

        if (score <= 2) return "Weak";
        if (score <= 4) return "Medium";
        return "Strong";
    }

}