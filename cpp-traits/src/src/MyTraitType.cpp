#include "MyTypeTrait.h"

#include <iostream>

#include "IntProcessor.h" // Here we include the external class
#include "StringProcessor.h" // Including StringProcessor for potential future use

namespace cppTraitsApi
{
    // Note: Using the namespace where ExternalClass is defined :)
    using namespace cppTraits;

    void MyTraitType::doSomething(int x)
    {
        std::cout << "Processing int: " << x << std::endl;

        // Here we can use the external class.
        IntProcessor intProcessor;
        intProcessor.function(x);
    }

    void MyTraitType::doSomething(const std::string& str)
    {
        std::cout << "Processing string: " << str << std::endl;

        StringProcessor stringProcessor;
        stringProcessor.processString(str);
    }

} // namespace cppTraitsApi
