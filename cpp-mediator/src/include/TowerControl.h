#ifndef TOWER_CONTROL_H
#define TOWER_CONTROL_H

#include "IAirTrafficControl.h"

/**
 * @brief Forward declaration of IAirplane.
 */
class IAirplane;

/**
 * @brief Concrete Mediator class.
 */
class TowerControl : public IAirTrafficControl 
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