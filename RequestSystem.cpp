#include "RequestSystem.h"
#include <iostream>
using namespace std;

void RequestSystem::addRequest(int fromFloor, int toFloor, string type) {
    pendingRequests.push_back(Request(fromFloor, toFloor, type));
}

bool RequestSystem::hasPendingRequests() const {
    return !pendingRequests.empty();
}

Request RequestSystem::getNextRequest() const {
    return pendingRequests.front();
}

void RequestSystem::removeRequest(int fromFloor) {
    for (int i = 0; i < (int)pendingRequests.size(); i++) {
        if (pendingRequests[i].getFromFloor() == fromFloor) {
            pendingRequests.erase(pendingRequests.begin() + i);
            return;
        }
    }
}

void RequestSystem::displayPendingRequests() const {
    if (pendingRequests.empty()) {
        cout << "No pending requests." << endl;
        return;
    }
    cout << "Pending Requests:" << endl;
    for (int i = 0; i < pendingRequests.size(); i++) {
        cout << "  Floor " << pendingRequests[i].getFromFloor()
             << " -> Floor " << pendingRequests[i].getToFloor()
             << " (" << pendingRequests[i].getType() << ")" << endl;
    }
}

int RequestSystem::getPendingCount() const {
    return pendingRequests.size();
}
