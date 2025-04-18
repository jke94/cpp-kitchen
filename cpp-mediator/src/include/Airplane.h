#ifndef AIRPLANE_H
#define AIRPLANE_H

#include <iostream>
#include <string>

#include "IAirTrafficControlApi.h"

namespace trafficControlMediator
{
    /**
     * @brief Concrete implementation of an airplane.
     */
    class Airplane : public IAirplane
    {
    private:
        IAirTrafficControl* airTrafficControlMediator_;
        std::string id;

    public:
        Airplane(IAirTrafficControl* airTrafficControlMediator, std::string identifier);

        void requestLanding() override;

        void requestTakeoff() override;

        void land() override;

        void takeoff() override;

        std::string getID() const override;
    };

} // namespace trafficControlMediator

#endif // AIRPLANE_H