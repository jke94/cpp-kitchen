#include "IAirTrafficControlApi.h"

#include <memory>
#include <string>

#include "Airplane.h"
#include "AirTrafficControlTower.h"

namespace trafficControlMediator
{
    std::shared_ptr<IAirplane> createAirplane(IAirTrafficControl* airTrafficControlTower, std::string id)
    {
        return std::make_shared<Airplane>(airTrafficControlTower, id);
    }
    
    std::shared_ptr<IAirTrafficControl> createAirTrafficControlTower() 
    {
        return std::make_shared<AirTrafficControlTower>();
    }
} // namespace trafficControlMediator