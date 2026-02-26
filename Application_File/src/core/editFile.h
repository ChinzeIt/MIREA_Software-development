#pragma once

#include <string>

class EditFile {
    public:
    // RAII must realization block and unblock file when file edit
    virtual ~EditFile() = default;

    virtual std::string getText() = 0;
    virtual void setText(std::string text) = 0;
};