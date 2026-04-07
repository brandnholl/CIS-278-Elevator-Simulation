#include "Building.h"
using namespace std;

Building::Building(int numFloors, int numElevators)
    : numFloors(numFloors) {
    for (int i = 0; i < numElevators; i++) {
        elevators.push_back(Elevator(i + 1, numFloors));
    }
}

int Building::getNumFloors() const { return numFloors; }
int Building::getNumElevators() const { return elevators.size(); }

const vector<Elevator>& Building::getElevators() const { return elevators; }
const vector<string>& Building::getLog() const { return messageLog; }
int Building::getPendingCount() const { return requestSystem.getPendingCount(); }

void Building::addMessage(const string& msg) {
    messageLog.push_back(msg);
    while (messageLog.size() > MAX_LOG_LINES) {
        messageLog.erase(messageLog.begin());
    }
}

void Building::requestElevator(int floor) {
    int bestIndex = 0;
    int bestDistance = numFloors + 1;

    for (int i = 0; i < elevators.size(); i++) {
        int dist = abs(elevators[i].getCurrentFloor() - floor);
        if (dist < bestDistance) {
            bestDistance = dist;
            bestIndex = i;
        }
    }

    elevators[bestIndex].addRequest(floor);
    requestSystem.addRequest(floor, -1, "call");
    addMessage("Assigned E" + to_string(elevators[bestIndex].getId()) +
               " to pick up at F" + to_string(floor));
}

void Building::requestFloor(int elevatorId, int floor) {
    for (int i = 0; i < elevators.size(); i++) {
        if (elevators[i].getId() == elevatorId) {
            elevators[i].addRequest(floor);
            requestSystem.addRequest(elevators[i].getCurrentFloor(), floor, "destination");
            addMessage("E" + to_string(elevatorId) +
                       " will go to F" + to_string(floor));
            return;
        }
    }
    addMessage("Elevator " + to_string(elevatorId) + " not found.");
}

void Building::simulateStep() {
    for (int i = 0; i < elevators.size(); i++) {
        elevators[i].step(messageLog);

        while (messageLog.size() > MAX_LOG_LINES) {
            messageLog.erase(messageLog.begin());
        }

        if (elevators[i].getDoorState() == "open") {
            requestSystem.removeRequest(elevators[i].getCurrentFloor());
        }
    }
}
