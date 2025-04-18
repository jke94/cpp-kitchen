#include "AirTrafficControlTower.h"

#include <iostream>
#include <memory>
#include <string>

void AirTrafficControlTower::requestLanding(IAirplane* airplane)
{
    if(runwayFree) 
    {
        std::cout << "Torre: Aprobado aterrizaje para " << airplane->getID() << "." << std::endl;
        runwayFree = false;
        airplane->land();
        runwayFree = true;
    } 
    else 
    {
        std::cout << "Torre: Pista ocupada, " << airplane->getID() << " debe esperar." << std::endl;
    }
}

void AirTrafficControlTower::requestTakeoff(IAirplane* airplane)
{
    if(runwayFree) 
    {
        std::cout << "Torre: Aprobado despegue para " << airplane->getID() << "." << std::endl;
        runwayFree = false;
        airplane->takeoff();
        runwayFree = true;
    } 
    else 
    {
        std::cout << "Torre: Pista ocupada, " << airplane->getID() << " debe esperar." << std::endl;
    }
}