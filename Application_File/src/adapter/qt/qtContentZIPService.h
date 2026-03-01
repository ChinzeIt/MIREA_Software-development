#pragma once

#include "../../core/contentZIPService.h"

#include <quazip/quazip.h>
#include <quazip/quazipfile.h>
#include <quazip/JlCompress.h>
#include <QDateTime>
#include <QString>
#include <QDebug>

class QTContentZIPService : public ContentZIPService {
    private:
    std::vector<content> c;
    public:
    void loadContent(std::string zip) override;
    std::string formatText() override;
};