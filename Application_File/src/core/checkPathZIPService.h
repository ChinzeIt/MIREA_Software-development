#include "checkPathZIP.h"

class CheckPathZIPService {
    public:
    virtual ~CheckPathZIPService() = default;
    virtual  bool checking(const std::string& rawPath, PathAccessModeZIP  mode) = 0;

    virtual std::string error() = 0;
};