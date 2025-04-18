#ifndef I_AIR_TRAFFIC_CONTROL_H
#define I_AIR_TRAFFIC_CONTROL_H

#include <string>

/**
 * @brief Interface for an airplane.
 */
class IAirplane
{
    
public:
    virtual ~IAirplane() = default;
    virtual void requestLanding() = 0;
    virtual void requestTakeoff() = 0;
    virtual void land() = 0;
    virtual void takeoff() = 0;
    virtual std::string getID() const = 0;

protected:
    IAirplane() = default;
    IAirplane(const IAirplane&) = delete;
    IAirplane& operator=(const IAirplane&) = delete;
    IAirplane(IAirplane&&) = delete;
    IAirplane& operator=(IAirplane&&) = delete;
};

/**
 * @brief Mediator interface for air traffic control.
 */
class IAirTrafficControl 
{
public:
    virtual void requestLanding(IAirplane* airplane) = 0;
    virtual void requestTakeoff(IAirplane* airplane) = 0;

protected:
    IAirTrafficControl() = default;
    IAirTrafficControl(const IAirTrafficControl&) = delete;
    IAirTrafficControl& operator=(const IAirTrafficControl&) = delete;
    IAirTrafficControl(IAirTrafficControl&&) = delete;
    IAirTrafficControl& operator=(IAirTrafficControl&&) = delete;
};

IAirplane* createAirplane(IAirTrafficControl* airTrafficControlTower, std::string id);
IAirTrafficControl* createAirTrafficControlTower();


#endif // I_AIR_TRAFFIC_CONTROL_H