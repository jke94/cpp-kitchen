#ifndef GLASS_DOOR_H
#define GLASS_DOOR_H

#include "IDoor.h"

namespace HouseNamespace
{
    class GlassDoor : public IDoor
    {
    public:
        GlassDoor();
        DoorType getDoorType() override;
    };
};

#endif