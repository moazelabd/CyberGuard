#pragma once

#include <string>

class Secret {
protected:
    int id;
    std::string title;
    std::string createdAt;

public:
    Secret(int id, const std::string& title, const std::string& createdAt)
        : id(id), title(title), createdAt(createdAt) {}

    virtual ~Secret() = default;

    int getId() const {
        return id;
    }

    std::string getTitle() const {
        return title;
    }

    std::string getCreatedAt() const {
        return createdAt;
    }

    virtual void viewDetails() const = 0;
};