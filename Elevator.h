#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <vector>
#include <string>
using namespace std;


class Elevator {
    private:
        int id;
        int currentFloor;
        int totalFloors;
        string direction;
        string doorState;

        vector<int> floorQueue;
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
        void step();
        void openDoor();
        void closeDoor();
        string getStatusString() const;


};

#endif
