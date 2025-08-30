#include "ExternalClass.h"

#include <iostream>

namespace cppTraitsAndFunctors
{
    void ExternalClass::function(int x)
    {
        std::cout << "Processing value in ExternalClass: " << x << std::endl;
    }

} // namespace cppTraitsAndFunctorsApi