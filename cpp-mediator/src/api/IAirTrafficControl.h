#ifndef I_AIR_TRAFFIC_CONTROL_H
#define I_AIR_TRAFFIC_CONTROL_H

/**
 * @brief Forward declaration of IAirplane.
 */
class IAirplane;

/**
 * @brief Mediator interface for air traffic control.
 */
class IAirTrafficControl 
{
public:
    virtual void requestLanding(IAirplane* airplane) = 0;
    virtual void requestTakeoff(IAirplane* airplane) = 0;
};

#endif // I_AIR_TRAFFIC_CONTROL_H