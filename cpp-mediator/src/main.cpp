#include <memory>
#include <string>

#include "IAirTrafficControlApi.h"

using namespace trafficControlMediator;

int main() 
{
    // Create the air traffic control tower
    std::shared_ptr<IAirTrafficControl> airTrafficControlTower = createAirTrafficControlTower();

    // Create airplanes
    std::shared_ptr<IAirplane> plane1 = createAirplane(airTrafficControlTower.get(), "AA123");
    std::shared_ptr<IAirplane> plane2 = createAirplane(airTrafficControlTower.get(), "BB456");
    std::shared_ptr<IAirplane> plane3 = createAirplane(airTrafficControlTower.get(), "JK1994");
    std::shared_ptr<IAirplane> plane4 = createAirplane(airTrafficControlTower.get(), "ALEX19");
    
    // Simulate airplane landing and takeoff requests.
    plane1->requestLanding();
    plane2->requestLanding();
    plane1->requestTakeoff();
    plane3->requestLanding();
    plane4->requestLanding();    
    plane2->requestTakeoff();

    return 0;
}