#include "MyTemplate.h"
#include "MyTypeTrait.h"

using namespace cppTraitsAndFunctorsApi;

int main() 
{
    MyTemplate<int, MyTraitType> obj;
    obj.execute(42);
}
