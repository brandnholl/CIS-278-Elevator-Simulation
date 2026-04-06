#include "Building.h"
#include <ncurses.h>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
using namespace std;

// Globals for threading
mutex buildingMutex;
atomic<bool> running(true);
atomic<bool> paused(false);
atomic<int> simSpeed(1000);

// ncurses windows
WINDOW* shaftWin = nullptr;
WINDOW* logWin = nullptr;
WINDOW* inputWin = nullptr;

void drawShaft(Building& building) {
    werase(shaftWin);
    int numFloors = building.getNumFloors();
    const vector<Elevator>& elevators = building.getElevators();

    // Title
    wattron(shaftWin, A_BOLD);
    mvwprintw(shaftWin, 0, 1, "=== Elevator Simulation ===");
    wattroff(shaftWin, A_BOLD);
    mvwprintw(shaftWin, 1, 1, "%d floors | %d elevator(s) | %s | Speed: %dms",
              numFloors, (int)elevators.size(),
              paused ? "PAUSED" : "RUNNING", simSpeed.load());

    // Column headers
    int startRow = 3;
    mvwprintw(shaftWin, startRow, 1, "      ");
    for (int e = 0; e < (int)elevators.size(); e++) {
        wprintw(shaftWin, "  E%d  ", elevators[e].getId());
    }

    // Draw floors top-down
    for (int floor = numFloors; floor >= 1; floor--) {
        int row = startRow + 1 + (numFloors - floor);
        mvwprintw(shaftWin, row, 1, " F%2d |", floor);
        for (int e = 0; e < (int)elevators.size(); e++) {
            if (elevators[e].getCurrentFloor() == floor) {
                string dir = elevators[e].getDirection();
                string door = elevators[e].getDoorState();
                char symbol;
                if (door == "open")       symbol = 'O';
                else if (dir == "up")     symbol = '^';
                else if (dir == "down")   symbol = 'v';
                else                      symbol = '-';

                wattron(shaftWin, A_BOLD | COLOR_PAIR(1));
                wprintw(shaftWin, " [%c] ", symbol);
                wattroff(shaftWin, A_BOLD | COLOR_PAIR(1));
            } else {
                wprintw(shaftWin, "  .   ");
            }
        }
    }

    // Legend
    int legendRow = startRow + 1 + numFloors + 1;
    mvwprintw(shaftWin, legendRow, 1, "[^]up [v]down [-]idle [O]doors open");

    // Pending requests
    int pending = building.getPendingCount();
    if (pending > 0) {
        mvwprintw(shaftWin, legendRow + 1, 1, "Pending requests: %d", pending);
    } else {
        mvwprintw(shaftWin, legendRow + 1, 1, "No pending requests");
    }

    wrefresh(shaftWin);
}

void drawLog(Building& building) {
    werase(logWin);
    box(logWin, 0, 0);
    wattron(logWin, A_BOLD);
    mvwprintw(logWin, 0, 2, " Log ");
    wattroff(logWin, A_BOLD);

    const vector<string>& log = building.getLog();
    int maxLines = getmaxy(logWin) - 2;
    int startIdx = (int)log.size() > maxLines ? log.size() - maxLines : 0;
    for (int i = startIdx; i < (int)log.size(); i++) {
        mvwprintw(logWin, 1 + (i - startIdx), 1, "%s", log[i].c_str());
    }
    wrefresh(logWin);
}

void drawInput(const string& input) {
    werase(inputWin);
    box(inputWin, 0, 0);
    mvwprintw(inputWin, 1, 1, "> %s", input.c_str());
    mvwprintw(inputWin, 2, 1, "call <floor> | go <elev> <floor> | pause | resume | speed <ms> | help | quit");
    wrefresh(inputWin);
    // Position cursor after user text
    wmove(inputWin, 1, 3 + input.size());
    wrefresh(inputWin);
}

void drawAll(Building& building, const string& input) {
    drawShaft(building);
    drawLog(building);
    drawInput(input);
}

void simulationLoop(Building& building, string& currentInput) {
    while (running) {
        this_thread::sleep_for(chrono::milliseconds(simSpeed.load()));
        if (!running) break;
        if (paused) continue;

        lock_guard<mutex> lock(buildingMutex);
        building.simulateStep();
        drawAll(building, currentInput);
    }
}

void processCommand(Building& building, const string& line, int numFloors, int numElevators) {
    istringstream iss(line);
    string command;
    iss >> command;

    if (command == "quit" || command == "q") {
        running = false;

    } else if (command == "help" || command == "h") {
        building.addMessage("-- Commands --");
        building.addMessage("call <floor>       - Call elevator to floor");
        building.addMessage("go <elev> <floor>  - Send elevator to floor");
        building.addMessage("pause / resume     - Pause/resume sim");
        building.addMessage("speed <200-5000>   - Set ms per step");
        building.addMessage("quit               - Exit");

    } else if (command == "pause" || command == "p") {
        paused = true;
        building.addMessage("Simulation paused.");

    } else if (command == "resume" || command == "r") {
        paused = false;
        building.addMessage("Simulation resumed.");

    } else if (command == "speed") {
        int ms;
        if (iss >> ms && ms >= 200 && ms <= 5000) {
            simSpeed = ms;
            building.addMessage("Speed set to " + to_string(ms) + "ms.");
        } else {
            building.addMessage("Usage: speed <200-5000>");
        }

    } else if (command == "call" || command == "c") {
        int floor;
        if (iss >> floor) {
            if (floor < 1 || floor > numFloors) {
                building.addMessage("Invalid floor (1-" + to_string(numFloors) + ").");
            } else {
                building.requestElevator(floor);
            }
        } else {
            building.addMessage("Usage: call <floor>");
        }

    } else if (command == "go" || command == "g") {
        int elevId, floor;
        if (iss >> elevId >> floor) {
            if (floor < 1 || floor > numFloors) {
                building.addMessage("Invalid floor (1-" + to_string(numFloors) + ").");
            } else if (elevId < 1 || elevId > numElevators) {
                building.addMessage("Invalid elevator (1-" + to_string(numElevators) + ").");
            } else {
                building.requestFloor(elevId, floor);
            }
        } else {
            building.addMessage("Usage: go <elevator_id> <floor>");
        }

    } else if (!command.empty()) {
        building.addMessage("Unknown command. Type 'help'.");
    }
}

int main() {
    // Setup phase uses plain cout (before ncurses)
    int numFloors, numElevators;

    printf("=== Elevator Simulation Setup ===\n");
    printf("Enter number of floors (5-20): ");
    if (scanf("%d", &numFloors) != 1 || numFloors < 5 || numFloors > 20) {
        printf("Invalid. Defaulting to 10.\n");
        numFloors = 10;
    }
    printf("Enter number of elevators (1-4): ");
    if (scanf("%d", &numElevators) != 1 || numElevators < 1 || numElevators > 4) {
        printf("Invalid. Defaulting to 2.\n");
        numElevators = 2;
    }

    Building building(numFloors, numElevators);

    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);
    refresh();

    // Setup colors
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
    }

    // Create windows
    int termH, termW;
    getmaxyx(stdscr, termH, termW);

    int shaftH = numFloors + 7; // floors + header + legend + padding
    int logH = 12;
    int inputH = 4;

    shaftWin = newwin(shaftH, termW, 0, 0);
    logWin = newwin(logH, termW, shaftH, 0);
    inputWin = newwin(inputH, termW, shaftH + logH, 0);

    building.addMessage("Simulation started! Type 'help' for commands.");

    string currentInput;
    drawAll(building, currentInput);

    // Launch sim thread
    thread simThread(simulationLoop, ref(building), ref(currentInput));

    // Input loop
    while (running) {
        int ch = wgetch(inputWin);

        lock_guard<mutex> lock(buildingMutex);

        if (ch == '\n' || ch == KEY_ENTER) {
            if (!currentInput.empty()) {
                processCommand(building, currentInput, numFloors, numElevators);
                currentInput.clear();
            }
        } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
            if (!currentInput.empty()) {
                currentInput.pop_back();
            }
        } else if (ch >= 32 && ch < 127) {
            currentInput += (char)ch;
        }

        drawAll(building, currentInput);
    }

    running = false;
    simThread.join();

    // Cleanup ncurses
    delwin(shaftWin);
    delwin(logWin);
    delwin(inputWin);
    endwin();

    printf("Simulation ended.\n");
    return 0;
}
