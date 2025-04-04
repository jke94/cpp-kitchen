#include <iostream>
#include <string>

#include "TowerControl.h"
#include "IAirplane.h"

void TowerControl::requestLanding(IAirplane* airplane)
{
    if(runwayFree) 
    {
        std::cout << "Torre: Aprobado aterrizaje para " << airplane->getID() << ".\n";
        runwayFree = false;
        airplane->land();
        runwayFree = true;
    } 
    else 
    {
        std::cout << "Torre: Pista ocupada, " << airplane->getID() << " debe esperar.\n";
    }
}

void TowerControl::requestTakeoff(IAirplane* airplane)
{
    if(runwayFree) 
    {
        std::cout << "Torre: Aprobado despegue para " << airplane->getID() << ".\n";
        runwayFree = false;
        airplane->takeoff();
        runwayFree = true;
    } 
    else 
    {
        std::cout << "Torre: Pista ocupada, " << airplane->getID() << " debe esperar.\n";
    }
}