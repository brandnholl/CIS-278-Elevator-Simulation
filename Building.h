#ifndef BUILDING_H
#define BUILDING_H

#include "Elevator.h"
#include "RequestSystem.h"
#include <vector>
#include <string>
using namespace std;

class Building {
    private:
        int numFloors;
        vector<Elevator> elevators;
        RequestSystem requestSystem;
        vector<string> messageLog;
        static const int MAX_LOG_LINES = 10;

    public :
        Building(int numFloors, int numElevators);

        void requestElevator(int floor);
        void requestFloor(int elevatorId, int floor);
        void simulateStep();
        int getNumFloors() const;
        int getNumElevators() const;
        void addMessage(const string& msg);

        // ncurses rendering
        const vector<Elevator>& getElevators() const;
        const vector<string>& getLog() const;
        int getPendingCount() const;

};

#endif
