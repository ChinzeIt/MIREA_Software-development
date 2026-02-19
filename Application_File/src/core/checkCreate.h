#pragma once
#include <array>
#include <string_view>

struct FileCreateValidationResult {
    bool empty=true;

    bool insideWorkingDir=true;
    bool parentDirExists=true;
    bool hasTraversal=true;

    bool writable=true;
    bool noFileExists=true;
    bool validName=true; 
    bool isSystemLink=true; 

    auto get_fields() {
        return std::array<std::pair<std::string_view, bool*>, 8>{{
            {"empty", &empty},
            {"insideWorkingDir", &insideWorkingDir},
            {"parentDirExists", &parentDirExists},
            {"hasTraversal", &hasTraversal},
            {"writable", &writable},
            {"fileExists", &noFileExists},
            {"validName", &validName},
            {"isSystemLink", &isSystemLink}
        }};
    }
};