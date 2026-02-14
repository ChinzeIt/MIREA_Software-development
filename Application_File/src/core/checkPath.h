#pragma once

#include <array>
#include <string_view>

struct PathValidationResult {
    bool empty;
    bool exists;
    bool isFile;
    bool readable;

    bool hasExtension;

    bool canonicalizable;
    bool absolute;

    bool insideWorkingDir;
    bool parentDirExists;
    bool hasTraversal;

    bool userReadable;
    bool userWritable;

    bool isSymlink;
    bool symlinkResolved;

    auto get_fields() {
        return std::array<std::pair<std::string_view, bool*>, 14>{{
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
            {"userWritable", &userWritable},
            {"isSymlink", &isSymlink},
            {"symlinkResolved", &symlinkResolved}
        }};
    }
};
