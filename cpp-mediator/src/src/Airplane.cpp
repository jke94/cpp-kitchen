#include "Airplane.h"

#include <iostream>
#include <string>
#include <memory>

Airplane::Airplane(
    IAirTrafficControl* airTrafficControlMediator, 
    std::string identifier
): 
    airTrafficControlMediator_(airTrafficControlMediator), 
    id(identifier) 
{
    
}

void Airplane::requestLanding() 
{
    std::cout << id << " solicita aterrizaje." << std::endl;
    airTrafficControlMediator_->requestLanding(this);
}

void Airplane::requestTakeoff() 
{
    std::cout << id << " solicita despegue." << std::endl;
    airTrafficControlMediator_->requestTakeoff(this);
}

void Airplane::land()
 {
    std::cout << id << " aterrizó con éxito." << std::endl;
}

void Airplane::takeoff() 
{
    std::cout << id << " despegó con éxito." << std::endl;
}

std::string Airplane::getID() const 
{
    return id;
}