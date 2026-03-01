#include "contentZIP.h"

class ContentZIPService {
    public:
    virtual ~ContentZIPService() = default;

    virtual void loadContent(std::string zip) = 0; // Must use field std::vector<content> c;
    virtual std::string formatText() = 0;
};