#include "MainService.h"

#include <memory>

#include "IMultiService.h"

MainService::MainService(std::shared_ptr<IMultiService> multiService) : 
    multiService_(std::move(multiService))
{
    // Constructor implementation
}

int MainService::computeNumber(int x, int y)
{
    if (std::abs(x) == std::abs(y)) 
    {
        return 1994;
    }

    int result = multiService_->compute(x, y);

    return result;
}