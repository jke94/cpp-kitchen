#include "GlassDoor.h"
#include "DoorType.h"

namespace HouseNamespace
{
    GlassDoor::GlassDoor()
    {
    }

    DoorType GlassDoor::getDoorType()
    {
        return DoorType::Glass;
    }
}