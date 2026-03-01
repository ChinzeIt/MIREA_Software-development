#pragma once

#include <array>
#include <string_view>

struct PathValidationResZIP {
    bool empty = true;
    bool exists = true;
    bool isFile = true;

    bool readable = true;
    bool writable = true;
    bool removable = true;

    bool hasExtension = true;
    bool canonicalizable = true;
    bool insideWorkingDir = true;

    bool zipExtension = true;
    bool zipSignature = true;
    bool zipReadable = true;
    bool zipNotCorrupted = true;

    auto get_fields() const {
        return std::array<std::pair<std::string_view, const bool*>, 13>{{
            {"empty", &empty},
            {"exists", &exists},
            {"isFile", &isFile},
            {"readable", &readable},
            {"writable", &writable},
            {"removable", &removable},
            {"hasExtension", &hasExtension},
            {"canonicalizable", &canonicalizable},
            {"insideWorkingDir", &insideWorkingDir},
            {"zipExtension", &zipExtension},
            {"zipSignature", &zipSignature},
            {"zipReadable", &zipReadable},
            {"zipNotCorrupted", &zipNotCorrupted}
        }};
    }
};

enum class PathAccessModeZIP {
    CONTENT,
    EDIT,
    UPLOAD
};