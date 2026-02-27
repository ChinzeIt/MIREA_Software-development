#pragma once

#include "../../core/deserialJsonXMLService.h"

#include <QJsonDocument>
#include <QJsonParseError>
#include <QString>
#include <QDomDocument>

#include <stdexcept>

class QTDeserialJsonXMLService : public deserialJsonXMLService {
    public:
    void checkDes(std::string text, TypeD type) override;
};