#ifndef REQUESTSYSTEM_H
#define REQUESTSYSTEM_H

#include <vector>
#include <string>
using namespace std;

class Request {
    private: 
        int fromFloor;
        int toFloor;
        string type;
    public:
        Request(int from, int to, string t):
            fromFloor(from), toFloor(to), type(t) {}
        
        int getFromFloor() const { return fromFloor;}
        int getToFloor() const { return toFloor; }
        string getType() const { return type; }

};

class RequestSystem {
    private:
        vector<Request> pendingRequests;
    public:
        void addRequest(int fromFloor, int toFloor, string type);
        bool hasPendingRequests() const;
        Request getNextRequest() const;
        void removeRequest(int fromFloor);
        void displayPendingRequests() const;
        int getPendingCount() const;
};



#endif