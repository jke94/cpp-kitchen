#ifndef MY_TYPE_TRAIT_H
#define MY_TYPE_TRAIT_H

#include <string>

namespace cppTraitsApi
{
    /**
     * @brief Trait structure that defines a static method to process integers.
     */
    class MyTraitType 
    {
    public:    
        static void doSomething(int x);
        static void doSomething(const std::string& str);
    };
    
} // namespace cppTraitsApi

#endif // MY_TYPE_TRAIT_H
