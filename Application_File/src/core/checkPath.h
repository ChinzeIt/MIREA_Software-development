#pragma once

#include <array>
#include <string_view>

struct PathValidationResult {
    bool empty=true;
    bool exists=true;
    bool isFile=true;
    bool readable=true;

    bool hasExtension=true;

    bool canonicalizable=true;
    bool absolute=true;

    bool insideWorkingDir=true;
    bool parentDirExists=true;
    bool hasTraversal=true;

    bool userReadable=true;

    bool isSymlink=true;
    bool symlinkResolved=true;

    auto get_fields() {
        return std::array<std::pair<std::string_view, bool*>, 13>{{
            {"empty", &empty},
            {"exists", &exists},
            {"isFile", &isFile},
            {"readable", &readable},
            {"hasExtension", &hasExtension},
            {"canonicalizable", &canonicalizable},
            {"absolute", &absolute},
            {"insideWorkingDir", &insideWorkingDir},
            {"parentDirExists", &parentDirExists},
            {"hasTraversal", &hasTraversal},
            {"userReadable", &userReadable},
            {"isSymlink", &isSymlink},
            {"symlinkResolved", &symlinkResolved}
        }};
    }
};
