#include <iostream>

#include "IHouse.h"
#include "House.h"

#include "IDoor.h"
#include "WoodenDoor.h"
#include "GlassDoor.h"
#include "DefaultDoor.h"
#include "DoorType.h"

std::ostream& operator<<(std::ostream& os, HouseNamespace::DoorType door);

int main(int argc, char* argv[])
{
    HouseNamespace::IDoor* woodenDoor = new HouseNamespace::WoodenDoor();
    HouseNamespace::IDoor* glassDoor = new HouseNamespace::GlassDoor();
    HouseNamespace::IDoor* defaultDoor = new HouseNamespace::DefaultDoor();

    HouseNamespace::IHouse* houseA = new HouseNamespace::House(230000.00, woodenDoor);
    HouseNamespace::IHouse* houseB = new HouseNamespace::House(210000.00, glassDoor);
    HouseNamespace::IHouse* houseC = new HouseNamespace::House(200000.00, defaultDoor);

    std::cout << "House A port type: " << houseA->getDoor()->getDoorType() << std::endl;
    std::cout << "House B port type: " << houseB->getDoor()->getDoorType() << std::endl;
    std::cout << "House C port type: " << houseC->getDoor()->getDoorType() << std::endl;
    
    delete houseA;
    delete houseB;
    delete houseC;

    return 0;
}

std::ostream& operator<<(std::ostream& os, HouseNamespace::DoorType door)
{
    switch (door)
    {
        case HouseNamespace::DoorType::Wooden:
            os << "Wooden";
            break;
        case HouseNamespace::DoorType::Glass:
            os << "Glass";
            break;
        case HouseNamespace::DoorType::Unknown:
            os << "Unknown";
            break;
        default:
            os << "NotDefined";
            break;
    }

    return os;
}