#include <iostream>
#include <vector>
#include <string>

#include "TowerControl.h"
#include "IAirplane.h"
#include "Airplane.h"

int main() 
{
    TowerControl tower;

    IAirplane* plane1 = new Airplane(&tower, "AA123");
    IAirplane* plane2 = new Airplane(&tower, "BB456");
    IAirplane* plane3 = new Airplane(&tower, "JK1994");
    IAirplane* plane4 = new Airplane(&tower, "ALEX1994");

    plane1->requestLanding();
    plane2->requestLanding();

    plane1->requestTakeoff();
    plane3->requestLanding();
    plane4->requestLanding();    
    plane2->requestTakeoff();

    delete plane1;
    delete plane2;
    delete plane3;
    delete plane4;

    return 0;
}