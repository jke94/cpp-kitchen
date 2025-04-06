#ifndef ALPHA_SERVICE_H
#define ALPHA_SERVICE_H

#include "IService.h"

class AlphaService : public IService
{
public:
    AlphaService() = default;
    ~AlphaService() override = default;
    void execute() override;
};

#endif // ALPHA_SERVICE_H