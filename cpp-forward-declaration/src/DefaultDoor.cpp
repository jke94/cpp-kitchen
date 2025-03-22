#include "DefaultDoor.h"
#include "DoorType.h"

namespace HouseNamespace
{
    DefaultDoor::DefaultDoor()
    {
    }

    DoorType DefaultDoor::getDoorType()
    {
        return DoorType::Unknown;
    }
};