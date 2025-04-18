#include "IAirTrafficControlApi.h"

#include <string>

#include "Airplane.h"
#include "AirTrafficControlTower.h"

IAirplane* createAirplane(IAirTrafficControl* airTrafficControlTower, std::string id)
{
    return new Airplane(airTrafficControlTower, id);
}

IAirTrafficControl* createAirTrafficControlTower()
{
    return new AirTrafficControlTower();
}