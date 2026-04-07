#ifndef BUILDING_H
#define BUILDING_H

#include "Elevator.h"
#include "RequestSystem.h"
#include <vector>
#include <string>
using namespace std;

class Building {
    private:
        int numFloors; // Total number of floors in the building
        vector<Elevator> elevators; // All elevator cars in the building
        RequestSystem requestSystem; // Tracks all pending requests
        vector<string> messageLog; // Recent log messages shown in the UI
        static const int MAX_LOG_LINES = 10; // Max log lines kept in memory

    public :
        //Constructor
        Building(int numFloors, int numElevators);

        //Called when user presses a hall button on a floor
        //Picks the best available elevator and dispatches it
        void requestElevator(int floor);
        
        //Called when a user sends a specific elevator to a floor
        void requestFloor(int elevatorId, int floor);
        
        //Advances teh entire simulation by one step
        void simulateStep();

        //Getters
        int getNumFloors() const;
        int getNumElevators() const;
        
        //Adds a message to the scrolling log(trims old entries)
        void addMessage(const string& msg);

        // ncurses rendering
        const vector<Elevator>& getElevators() const;
        const vector<string>& getLog() const;
        int getPendingCount() const;

};

#endif
