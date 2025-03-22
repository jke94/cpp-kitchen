#include "WoodenDoor.h"
#include "DoorType.h"

namespace HouseNamespace
{
    WoodenDoor::WoodenDoor()
    {
    }

    DoorType WoodenDoor::getDoorType()
    {
        return DoorType::Wooden;
    }
}