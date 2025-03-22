#ifndef IDOOR_H
#define IDOOR_H

namespace HouseNamespace
{
    enum class DoorType;

    class IDoor
    {
    public:
        virtual ~IDoor() = default;
        virtual DoorType getDoorType() = 0;
    };
};

#endif