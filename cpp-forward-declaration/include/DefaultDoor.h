#ifndef DEFAULT_DOOR_H
#define DEFAULT_DOOR_H

#include "IDoor.h"

namespace HouseNamespace
{
    class DefaultDoor : public IDoor
    {
    public:
        DefaultDoor();
        DoorType getDoorType() override;
    };
};

#endif