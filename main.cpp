#include <iostream>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <string>

using namespace std;

enum class Status { AVAILABLE, FULL };

class ParkingSpot {
private:
    string spotNumber;

public:
    ParkingSpot(string spotNumber) : spotNumber(spotNumber) {}

    string getSpotNumber() {
        return spotNumber;
    }
};

class DisplayBoard {
private:
    Status status;

public:
    DisplayBoard() {
        status = Status::AVAILABLE;
    }

    void update(Status newStatus) {
        status = newStatus;
    }

    Status getStatus() {
        return status;
    }
};

class Admin {
public:
    string name;
};

class Vehicle {
private:
    string vehicleNumber;

public:
    Vehicle(string vehicleNumber) : vehicleNumber(vehicleNumber) {}

    string getVehicleNumber() {
        return vehicleNumber;
    }
};

class Ticket {
private:
    static int idCounter;
    int id;
    string vehicleRegistrationNumber;
    string parkingSpotNumber;
    time_t arrivalTime;

public:
    Ticket(Vehicle vehicle, ParkingSpot spot) {
        id = ++idCounter;
        vehicleRegistrationNumber = vehicle.getVehicleNumber();
        parkingSpotNumber = spot.getSpotNumber();
        arrivalTime = time(0);
    }

    int getId() {
        return id;
    }

    time_t getArrivalTime() {
        return arrivalTime;
    }

    string toString() {
        return "Ticket{id=" + to_string(id) + ", vehicleRegistrationNumber='" + vehicleRegistrationNumber +
               "', arrivalTime=" + ctime(&arrivalTime) + ", parkingSpotNumber='" + parkingSpotNumber + "'}";
    }
};

int Ticket::idCounter = 0;

class ParkingLot;

class AutomatedSystem {
private:
    int id;
    ParkingLot* parkingLot;
    unordered_map<int, Ticket> currentTickets;

    ParkingSpot fetchAvailableSpot();

public:
    AutomatedSystem(int id) : id(id), parkingLot(nullptr) {}

    void setParkingLot(ParkingLot* lot) {
        parkingLot = lot;
    }

    Ticket generateTicket(Vehicle vehicle);
    Ticket scanTicket();
    double calculateCharges();
    void openEntryBarrier();
    void closeEntryBarrier();
    void openExitBarrier();
    void closeExitBarrier();
};

class ParkingLot {
private:
    string name;
    int capacity;
    string location;
    double parkingCharges;
    Admin admin;
    AutomatedSystem automatedSystem;
    DisplayBoard displayBoard;
    vector<ParkingSpot> parkingSpots;
    vector<ParkingSpot> availableSpots;

    vector<ParkingSpot> createParkingSpots(int capacity);

public:
    ParkingLot(string name, int capacity, string location, double parkingCharges, AutomatedSystem system, DisplayBoard board)
        : name(name), capacity(capacity), location(location), parkingCharges(parkingCharges), automatedSystem(system), displayBoard(board) {
        parkingSpots = createParkingSpots(capacity);
        availableSpots = parkingSpots;
        automatedSystem.setParkingLot(this);
    }

    double getParkingCharges() {
        return parkingCharges;
    }

    DisplayBoard getDisplayBoard() {
        return displayBoard;
    }

    ParkingSpot getAvailableSpot() {
        ParkingSpot spot = availableSpots.back();
        availableSpots.pop_back();
        return spot;
    }

    int getAvailability() {
        return availableSpots.size();
    }

    void updateParkingCharges(double newCharges) {
        parkingCharges = newCharges;
    }

    void setAdmin(Admin admin) {
        this->admin = admin;
    }
};

vector<ParkingSpot> ParkingLot::createParkingSpots(int capacity) {
    vector<ParkingSpot> tempList;
    for (int i = 0; i < capacity; ++i) {
        tempList.push_back(ParkingSpot("MPL " + to_string(i)));
    }
    return tempList;
}

ParkingSpot AutomatedSystem::fetchAvailableSpot() {
    return parkingLot->getAvailableSpot();
}

Ticket AutomatedSystem::generateTicket(Vehicle vehicle) {
    ParkingSpot availableSpot = fetchAvailableSpot();
    Ticket ticket(vehicle, availableSpot);
    currentTickets[ticket.getId()] = ticket;
    return ticket;
}

Ticket AutomatedSystem::scanTicket() {
    // Code to scan and return ticketId, assuming id 1234 for now
    return currentTickets[1234];
}

double AutomatedSystem::calculateCharges() {
    Ticket ticket = scanTicket();
    time_t now = time(0);
    double duration = difftime(now, ticket.getArrivalTime()) / 3600;  // hours
    return duration * parkingLot->getParkingCharges();
}

void AutomatedSystem::openEntryBarrier() {
    parkingLot->getDisplayBoard().update(Status::FULL);
}

void AutomatedSystem::closeEntryBarrier() {}

void AutomatedSystem::openExitBarrier() {
    parkingLot->getDisplayBoard().update(Status::AVAILABLE);
}

void AutomatedSystem::closeExitBarrier() {}

int main() {
    DisplayBoard displayBoard;
    AutomatedSystem automatedSystem(1);
    ParkingLot myParkingLot("MyParkingLot", 100, "Sec-135, Noida", 50, automatedSystem, displayBoard);

    // Check availability
    int availability = myParkingLot.getAvailability();
    cout << "Availability: " << availability << endl;

    // Create customer and generate ticket
    Vehicle piyushVehicle("UP85 AX 5454");
    Ticket piyushTicket = automatedSystem.generateTicket(piyushVehicle);
    cout << piyushTicket.toString() << endl;

    // Create another customer and generate ticket
    Vehicle ayushVehicle("UP86 AB 9999");
    Ticket ayushTicket = automatedSystem.generateTicket(ayushVehicle);
    cout << ayushTicket.toString() << endl;

    return 0;
}
