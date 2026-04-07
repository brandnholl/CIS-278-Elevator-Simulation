#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "RequestSystem.h"
#include <vector>
#include <string>
using namespace std;

class Elevator {
    private:
        int id; //Unique elevator identifier
        int currentFloor; //Floor elevator is currently on
        int totalFloors; // Total floors in the building
        string direction; // Current movement direction
        string doorState; // Door status

        vector<int> floorQueue; // Ordered list of floors this elevator needs to visit
    public:
        //constructor
        Elevator(int id, int totalFloors);

        // getters
        int getId() const;
        int getCurrentFloor() const;
        string getDirection() const;
        string getDoorState() const;
        bool hasRequests() const;

        // behaviors
        void addRequest(int floor);
        void step(vector<string>& log);
        string getStatusString() const;


};

#endif
