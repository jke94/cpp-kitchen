#ifndef WOODEN_DOOR_H
#define WOODEN_DOOR_H

#include "IDoor.h"

namespace HouseNamespace
{
    class WoodenDoor : public IDoor
    {
    public:
        WoodenDoor();
        DoorType getDoorType() override;
    };
};

#endif