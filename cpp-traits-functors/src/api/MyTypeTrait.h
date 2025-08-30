#ifndef MY_TYPE_TRAIT_H
#define MY_TYPE_TRAIT_H

#include <iostream>

namespace cppTraitsAndFunctorsApi
{
    /**
     * @brief Trait structure that defines a static method to process integers.
     */
    class MyTraitType 
    {
    public:    
        static void doSomething(int x);
    };
    
} // namespace cppTraitsAndFunctorsApi

#endif // MY_TYPE_TRAIT_H
