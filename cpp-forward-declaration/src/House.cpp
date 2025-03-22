#include "House.h"
#include "IDoor.h"      // NOTE: Supprerssing the warning of incomplete type of IDoor
#include "DoorType.h"   // NOTE: Supprerssing the warning of incomplete type of DoorType

namespace HouseNamespace
{
    House::House(float price, IDoor* door) : 
        price_(price), 
        door_(door)  
        {}

    House::~House()
    {
        delete door_;
    }

    IDoor* House::getDoor()
    {
        return door_;
    }

    DoorType House::getDoorType()
    {
        if(door_ == nullptr)
        {
            return DoorType::Unknown;
        }

        return door_->getDoorType();
    }

}