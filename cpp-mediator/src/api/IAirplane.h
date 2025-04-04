#ifndef I_AIRPLANE_H
#define I_AIRPLANE_H

#include <string>

class IAirplane
{
public:
    virtual void requestLanding() = 0;
    virtual void requestTakeoff() = 0;
    virtual void land() = 0;
    virtual void takeoff() = 0;
    virtual std::string getID() const = 0;
};

#endif // I_AIRPLANE_H