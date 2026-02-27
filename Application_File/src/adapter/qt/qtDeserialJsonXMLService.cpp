#include "qtDeserialJsonXMLService.h"

void QTDeserialJsonXMLService::checkDes(std::string text, TypeD type) {
    QString qText = QString::fromStdString(text).trimmed();

    if (qText.isEmpty()) {
        throw std::runtime_error("Input text is empty");
    }

    switch (type) {
        case TypeD::JSON: {
            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(qText.toUtf8(), &error);

            if (error.error != QJsonParseError::NoError) {
                throw std::runtime_error(
                    ("JSON parse error: " + error.errorString()).toStdString()
                );
            }

            if (!doc.isObject() && !doc.isArray()) {
                throw std::runtime_error("JSON is neither object nor array");
            }

            break;
        }

        case TypeD::XML: {
            QDomDocument doc;
            QString errorMsg;
            int errorLine = 0;
            int errorColumn = 0;

            if (!doc.setContent(qText, &errorMsg, &errorLine, &errorColumn)) {
                throw std::runtime_error(
                    ("XML parse error at line " +
                     std::to_string(errorLine) +
                     ", column " +
                     std::to_string(errorColumn) +
                     ": " +
                     errorMsg.toStdString())
                );
            }

            break;
        }

        default:
            throw std::runtime_error("Unknown deserialization type");
    }
}