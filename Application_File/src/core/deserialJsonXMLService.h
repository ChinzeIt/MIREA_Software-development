#pragma once

#include <string>

enum class TypeD {
    JSON,
    XML
};

class deserialJsonXMLService {
    public:
    virtual ~deserialJsonXMLService () = default;
    
    virtual void checkDes (std::string text, TypeD) = 0;
};