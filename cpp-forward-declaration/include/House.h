#ifndef HOUSE_H
#define HOUSE_H

#include "IHouse.h"

namespace HouseNamespace
{
    class House : public IHouse
    {
    private:
        IDoor* door_;
        float price_;   

    public:

        House(float price, IDoor* door);
        ~House();
        IDoor* getDoor() override;
        DoorType getDoorType() override;
    };
};

#endif