#include "Elevator.h"
#include <algorithm>
using namespace std;

Elevator::Elevator(int id, int totalFloors)
    : id(id), currentFloor(1), totalFloors(totalFloors),
      direction("idle"), doorState("closed") {}

int Elevator::getId() const { return id; }
int Elevator::getCurrentFloor() const { return currentFloor; }
string Elevator::getDirection() const { return direction; }
string Elevator::getDoorState() const { return doorState; }

bool Elevator::hasRequests() const {
    return !floorQueue.empty();
}

void Elevator::addRequest(int floor) {
    if (floor < 1 || floor > totalFloors) return;
    for (int f : floorQueue) {
        if (f == floor) return;
    }
    floorQueue.push_back(floor);
}

void Elevator::step(vector<string>& log) {
    if (floorQueue.empty()) {
        direction = "idle";
        doorState = "closed";
        return;
    }

    // Close doors before moving
    if (doorState == "open") {
        doorState = "closed";
        log.push_back("E" + to_string(id) + ": Doors closing at F" + to_string(currentFloor));
        return; // closing doors takes one step
    }

    int target = floorQueue.front();

    if (currentFloor < target) {
        direction = "up";
        currentFloor++;
    } else if (currentFloor > target) {
        direction = "down";
        currentFloor--;
    }

    if (currentFloor == target) {
        doorState = "open";
        floorQueue.erase(floorQueue.begin());
        log.push_back("E" + to_string(id) + ": Arrived at F" + to_string(currentFloor) + " - doors open");
        if (floorQueue.empty()) {
            direction = "idle";
        }
    }
}

string Elevator::getStatusString() const {
    return "Elevator " + to_string(id) +
           " | Floor: " + to_string(currentFloor) +
           " | Direction: " + direction +
           " | Doors: " + doorState;
}
