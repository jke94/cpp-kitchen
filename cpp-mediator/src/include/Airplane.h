#ifndef AIRPLANE_H
#define AIRPLANE_H

#include <iostream>
#include <string>
#include "IAirplane.h"

class IAirTrafficControl;

class Airplane : public IAirplane
{
protected:
    IAirTrafficControl* mediator;
    std::string id;

public:
    Airplane(IAirTrafficControl* med, std::string identifier);

    void requestLanding() override;

    void requestTakeoff() override;

    void land() override;

    void takeoff() override;

    std::string getID() const override;
};


#endif // AIRPLANE_H