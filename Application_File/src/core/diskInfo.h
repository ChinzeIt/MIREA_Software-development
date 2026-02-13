#pragma once

#include <string>

struct DiskInfo
{
    std::string nameD;
    std::string fileSystem;
    unsigned long long memoryCapacity;
    unsigned long long memoryFree;
    unsigned long long memoryAviable;
    unsigned long long memoryOccupied;
    bool readOnly = false;
};