#ifndef TOWER_CONTROL_H
#define TOWER_CONTROL_H

#include "IAirTrafficControlApi.h"

/**
 * @brief Concrete Mediator class.
 */
class AirTrafficControlTower : public IAirTrafficControl 
{
private:

    /**
     * @brief Flag to indicate if the runway is free (only one runway).
     */
    bool runwayFree = true;  

public:

    void requestLanding(IAirplane* airplane) override;
    void requestTakeoff(IAirplane* airplane) override;
};


#endif // TOWER_CONTROL_H