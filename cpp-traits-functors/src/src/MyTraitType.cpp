#include "MyTypeTrait.h"

#include "ExternalClass.h" // Here we include the external class 

namespace cppTraitsAndFunctorsApi
{
    // Note: Using the namespace where ExternalClass is defined :)
    using namespace cppTraitsAndFunctors;

    void MyTraitType::doSomething(int x)
    {
        std::cout << "Processing int: " << x << std::endl;

        // Here we can use the external class.
        ExternalClass externalClassInstance;
        externalClassInstance.function(x);
    }

} // namespace cppTraitsAndFunctorsApi
