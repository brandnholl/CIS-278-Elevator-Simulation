#ifndef BUILDING_H
#define BUILDING_H

#include "Elevator.h"
#include <vector>
using namespace std;

class Building {
    private: 
        int numFloors;
        vector<Elevator> elevators;

    public :
        Building(int numFloors, int numElevators);

        void requestElevator(int floor);
        void requestFloor(int elevatorId, int floor);
        void simulateStep();
        void displayStatus();int getNumFloors() const;

};

#endif