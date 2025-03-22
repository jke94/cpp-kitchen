#ifndef IHOUSE_H
#define IHOUSE_H

namespace HouseNamespace
{
    class IDoor;
    enum class DoorType;

    class IHouse
    {
    public:
        virtual ~IHouse() {}
        virtual IDoor* getDoor() = 0;
        virtual DoorType getDoorType() = 0; 
    };
};
#endif