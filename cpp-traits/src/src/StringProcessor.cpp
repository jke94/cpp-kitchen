#include "StringProcessor.h"

#include <iostream>
#include <string>

namespace cppTraits
{
    void StringProcessor::processString(const std::string& str) 
    {
        std::cout << "Processing value in StringProcessor class: " << str << std::endl;
    }
    
} // namespace cppTraits