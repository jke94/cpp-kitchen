#include <string>

#include "Airplane.h"
#include "IAirTrafficControl.h"

Airplane::Airplane(
    IAirTrafficControl* med, 
    std::string identifier) 
    : mediator(med), 
    id(identifier) 
{
    
}

void Airplane::requestLanding() 
{
    std::cout << id << " solicita aterrizaje.\n";
    mediator->requestLanding(this);
}

void Airplane::requestTakeoff() 
{
    std::cout << id << " solicita despegue.\n";
    mediator->requestTakeoff(this);
}

void Airplane::land()
 {
    std::cout << id << " aterrizó con éxito.\n";
}

void Airplane::takeoff() 
{
    std::cout << id << " despegó con éxito.\n";
}

std::string Airplane::getID() const 
{
    return id;
}