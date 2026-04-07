# CIS-278-Elevator-Simulation in C++

This project is a C++ simulation of an elevator system in a multi-floor building. The program models elevator movement, handles user requests, and displays the system state through a console interface. The goal is to demonstrate object-oriented programming and system design.

Features
-Configurable number of floors
-One or more elevators
-Elevator movement between floors
-Floor request system
-Idle state when no requests
-Console display of elevator status
-User input during runtime
-Step-based simulation

## How to Compile the Simulation:

Make sure you have g++ and the ncurses library installed
```bash
g++ -std=c++17 main.cpp Building.cpp Elevator.cpp RequestSystem.cpp -lncurses -o elevator
```

##How to Run
```bash
./elevator
```

-Enter floor requests when prompted
-The system will assign an elevator
-Ensure terminal window is at it's maximum view for best experience
-Watch the console to see movement and status updates

## Commands
| Command | Shortcut | Description |
|---|---|---|
| `call <floor>` | `c <floor>` | Call an elevator to a floor (hall button) |
| `go <id> <floor>` | `g <id> <floor>` | Send elevator #id directly to a floor |
| `pause` | `p` | Pause the simulation |
| `resume` | `r` | Resume the simulation |
| `speed <ms>` | — | Set simulation speed in milliseconds (200–5000) |
| `help` | `h` | Show command reference |
| `quit` | `q` | Exit the simulation |

Team Members
Brandon Holland | Built Building, RequestSystem, Main, Elevator classes(cpp), UI, and debugging |
James So | Elevator, Building, RequestSystem logic(headers) & debugging
Han Bhone Hset |Elevator, Building, Main, and debuggin |
