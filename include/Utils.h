#pragma once

#include <string>

namespace Utils {
    std::string xorEncryptDecrypt(const std::string& text, char key = 'K');
    std::string getCurrentDate();
    std::string generatePassword(int length = 12);
    std::string checkPasswordStrength(const std::string& password);
}