#pragma once

#include <array>
#include <string_view>

struct PathValidationResult {
    bool empty = true;
    bool exists = true;
    bool isFile = true;

    bool noFileExists=true;
    bool validName=true; 
    bool isSystemLink=true; 

    bool readable = true;
    bool writable = true;
    bool removable = true;

    bool hasExtension = true;

    bool canonicalizable = true;
    bool absolute = true;

    bool insideWorkingDir = true;
    bool parentDirExists = true;
    bool hasTraversal = true;

    bool userReadable = true;
    bool userWritable = true;

    bool isSymlink = true;
    bool symlinkResolved = true;

    auto get_fields() {
        return std::array<std::pair<std::string_view, bool*>, 19>{{
            {"empty", &empty},
            {"exists", &exists},
            {"isFile", &isFile},
            {"noFileExists", &noFileExists},
            {"validName", &validName},
            {"isSystemLink", &isSystemLink},
            {"readable", &readable},
            {"writable", &writable},
            {"removable", &removable},
            {"hasExtension", &hasExtension},
            {"canonicalizable", &canonicalizable},
            {"absolute", &absolute},
            {"insideWorkingDir", &insideWorkingDir},
            {"parentDirExists", &parentDirExists},
            {"hasTraversal", &hasTraversal},
            {"userReadable", &userReadable},
            {"userWritable", &userWritable},
            {"isSymlink", &isSymlink},
            {"symlinkResolved", &symlinkResolved}
        }};
    }
};

enum class PathAccessMode {
    Create,
    Read,
    Write,
    Remove
};