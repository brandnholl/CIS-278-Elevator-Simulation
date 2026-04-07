#ifndef REQUESTSYSTEM_H
#define REQUESTSYSTEM_H

#include <vector>
#include <string>
using namespace std;

class Request {
    private: 
        int fromFloor; // The floor request came from
        int toFloor; // Destination floor (-1 if unknown)
        string type; // "call" = hall button, "destination" = floor button inside elevator
    public:
        Request(int from, int to, string t):
            fromFloor(from), toFloor(to), type(t) {}
        
        int getFromFloor() const { return fromFloor;}
        int getToFloor() const { return toFloor; }
        string getType() const { return type; }

};

//Manages the collection of all active/pending requests in the building

class RequestSystem {
    private:
        vector<Request> pendingRequests;
    public:
        // Add a new request to the queue
        void addRequest(int fromFloor, int toFloor, string type);
        
        // Returns true if there are any unresolved requests
        bool hasPendingRequests() const;
        
        //Returns the first pending request
        Request getNextRequest() const;
        
        //Removes any request originating from the given floor
        void removeRequest(int fromFloor);
       
        // Prints all pending requests to stdout
        void displayPendingRequests() const;
        
        // Returns how many requests are currently pending
        int getPendingCount() const;
};



#endif