#include "MyTemplate.h"
#include "MyTypeTrait.h"

#include <string>

using namespace cppTraitsApi;

int main() 
{
    MyTemplate<int, MyTraitType> obj;
    obj.execute(42);

    MyTemplate<std::string, MyTraitType> stringObj;
    stringObj.execute("Hello, Traits!");

    return 0;
}
