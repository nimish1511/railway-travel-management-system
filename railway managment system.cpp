#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <climits>
#include <ctime>
#include <cstdlib>
#include <map>
#include <iomanip>
#include <queue>
#include <algorithm>

using namespace std;

struct Station {
    string name;
    vector<pair<int, int> > connections; // pair<station_index, distance>
};

enum TicketClass {
    GENERAL = 1,
    SLEEPER = 2,
    AC = 3,
    FIRST_CLASS = 4
};

struct FareInfo {
    int generalFare;
    int sleeperFare;
    int acFare;
    int firstClassFare;
    
    FareInfo(int distance) {
        generalFare = distance * 2;      // 2 per km
        sleeperFare = distance * 3;      // 3 per km
        acFare = distance * 5;           // 5 per km
        firstClassFare = distance * 8;   // 8 per km
    }
};

struct TimeInfo {
    double hours;
    double minutes;
    
    TimeInfo(int distance) {
        double averageSpeed = 60.0; // km/hr
        hours = distance / averageSpeed;
        minutes = (hours - (int)hours) * 60;
    }
};

struct User {
    string username;
    string password;
    vector<int> bookingIds;
};

struct Booking {
    int ticketNumber;
    string passengerName;
    int age;
    int fromStation;
    int toStation;
    TicketClass ticketClass;
    int distance;
    int fare;
    double travelTime;
    string username;
    bool isActive;
    
    Booking(int ticketNum, string name, int passengerAge, int from, int to, 
            TicketClass tClass, int dist, int fareAmount, double time, string user) {
        ticketNumber = ticketNum;
        passengerName = name;
        age = passengerAge;
        fromStation = from;
        toStation = to;
        ticketClass = tClass;
        distance = dist;
        fare = fareAmount;
        travelTime = time;
        username = user;
        isActive = true;
    }
};

vector<Station> stations;
vector<User> users;
vector<Booking> bookings;
string currentUser = "";
int nextTicketNumber = 1001;

void addStation(const string& name) {
    Station newStation;
    newStation.name = name;
    stations.push_back(newStation);
    cout << "Station added: " << name << endl;
}

void addConnection(int from, int to, int distance) {
    stations[from].connections.push_back(make_pair(to, distance));
    stations[to].connections.push_back(make_pair(from, distance));
}

void displayStations() {
    cout << "\nAvailable Stations:\n";
    for (int i = 0; i < stations.size(); i++) {
        cout << i << ". " << stations[i].name << endl;
    }
}

void displayFareOptions() {
    cout << "\nTicket Classes:\n";
    cout << "1. General (₹2 per km)\n";
    cout << "2. Sleeper (₹3 per km)\n";
    cout << "3. AC (₹5 per km)\n";
    cout << "4. First Class (₹8 per km)\n";
}

void calculateFare(int distance, TicketClass ticketClass) {
    FareInfo fareInfo(distance);
    TimeInfo timeInfo(distance);
    
    cout << "\n=== Route Information ===\n";
    cout << "Distance: " << distance << " km\n";
    cout << "Estimated Travel Time: " << (int)timeInfo.hours << " hours " 
         << (int)timeInfo.minutes << " minutes\n";
    
    switch(ticketClass) {
        case GENERAL:
            cout << "Ticket Class: General\n";
            cout << "Fare: ₹" << fareInfo.generalFare << endl;
            break;
        case SLEEPER:
            cout << "Ticket Class: Sleeper\n";
            cout << "Fare: ₹" << fareInfo.sleeperFare << endl;
            break;
        case AC:
            cout << "Ticket Class: AC\n";
            cout << "Fare: ₹" << fareInfo.acFare << endl;
            break;
        case FIRST_CLASS:
            cout << "Ticket Class: First Class\n";
            cout << "Fare: ₹" << fareInfo.firstClassFare << endl;
            break;
    }
    
    cout << "\nAll Fare Options:\n";
    cout << "General: ₹" << fareInfo.generalFare << endl;
    cout << "Sleeper: ₹" << fareInfo.sleeperFare << endl;
    cout << "AC: ₹" << fareInfo.acFare << endl;
    cout << "First Class: ₹" << fareInfo.firstClassFare << endl;
}

// User Management Functions
bool registerUser() {
    string username, password;
    cout << "\n=== User Registration ===\n";
    cout << "Enter username: ";
    cin >> username;
    
    // Check if username already exists
    for (int i = 0; i < users.size(); i++) {
        if (users[i].username == username) {
            cout << "Username already exists! Please choose a different username.\n";
            return false;
        }
    }
    
    cout << "Enter password: ";
    cin >> password;
    
    User newUser;
    newUser.username = username;
    newUser.password = password;
    users.push_back(newUser);
    
    cout << "Registration successful! You can now login.\n";
    return true;
}

bool loginUser() {
    string username, password;
    cout << "\n=== User Login ===\n";
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    
    for (int i = 0; i < users.size(); i++) {
        if (users[i].username == username && users[i].password == password) {
            currentUser = username;
            cout << "Login successful! Welcome " << username << "!\n";
            return true;
        }
    }
    
    cout << "Invalid username or password!\n";
    return false;
}

void logoutUser() {
    currentUser = "";
    cout << "Logged out successfully!\n";
}

// Optimized Dijkstra with Priority Queue
vector<int> dijkstra(int source, int destination, const vector<Station>& stations) {
    int n = stations.size();
    vector<int> dist(n, INT_MAX);
    vector<int> prev(n, -1);
    vector<bool> visited(n, false);
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq; // (dist, node)

    dist[source] = 0;
    pq.push(make_pair(0, source));

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (visited[u]) continue;
        visited[u] = true;

        for (int j = 0; j < stations[u].connections.size(); j++) {
            pair<int, int> edge = stations[u].connections[j];
            int v = edge.first;
            int weight = edge.second;

            if (!visited[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.push(make_pair(dist[v], v));
            }
        }
    }

    // Reconstruct path from source to destination
    vector<int> path;
    for (int at = destination; at != -1; at = prev[at])
        path.push_back(at);

    reverse(path.begin(), path.end());
    return path;
}

// Booking System Functions
int getFareForClass(int distance, TicketClass ticketClass) {
    FareInfo fareInfo(distance);
    switch(ticketClass) {
        case GENERAL: return fareInfo.generalFare;
        case SLEEPER: return fareInfo.sleeperFare;
        case AC: return fareInfo.acFare;
        case FIRST_CLASS: return fareInfo.firstClassFare;
        default: return fareInfo.generalFare;
    }
}

string getClassString(TicketClass ticketClass) {
    switch(ticketClass) {
        case GENERAL: return "General";
        case SLEEPER: return "Sleeper";
        case AC: return "AC";
        case FIRST_CLASS: return "First Class";
        default: return "General";
    }
}

// Function to display route path as string
string getRoutePathString(const vector<int>& path) {
    string routeStr = "";
    for (int i = 0; i < path.size(); i++) {
        routeStr += stations[path[i]].name;
        if (i < path.size() - 1) routeStr += " -> ";
    }
    return routeStr;
}

// Helper function to get shortest distance between two stations
int getShortestDistance(int from, int to) {
    vector<int> path = dijkstra(from, to, stations);
    if (path.size() <= 1) return -1; // No path available
    
    int distance = 0;
    for (int i = 0; i < path.size() - 1; i++) {
        for (int j = 0; j < stations[path[i]].connections.size(); j++) {
            pair<int, int> conn = stations[path[i]].connections[j];
            if (conn.first == path[i + 1]) {
                distance += conn.second;
                break;
            }
        }
    }
    return distance;
}

void bookTicket() {
    if (currentUser == "") {
        cout << "Please login first to book a ticket!\n";
        return;
    }
    
    cout << "\n=== Ticket Booking ===\n";
    
    // Get passenger details
    string passengerName;
    int age;
    cout << "Enter passenger name: ";
    cin.ignore();
    getline(cin, passengerName);
    cout << "Enter passenger age: ";
    cin >> age;
    
    // Select stations
    displayStations();
    int fromStation, toStation;
    cout << "Enter source station index: ";
    cin >> fromStation;
    cout << "Enter destination station index: ";
    cin >> toStation;
    
    if (fromStation < 0 || fromStation >= stations.size() || 
        toStation < 0 || toStation >= stations.size() || 
        fromStation == toStation) {
        cout << "Invalid station selection!\n";
        return;
    }
    
    // Calculate shortest path using optimized Dijkstra
    vector<int> path = dijkstra(fromStation, toStation, stations);
    if (path.size() <= 1) {
        cout << "No available path between selected stations.\n";
        return;
    }
    
    // Calculate total distance from path
    int totalDistance = 0;
    for (int i = 0; i < path.size() - 1; i++) {
        for (int j = 0; j < stations[path[i]].connections.size(); j++) {
            pair<int, int> conn = stations[path[i]].connections[j];
            if (conn.first == path[i + 1]) {
                totalDistance += conn.second;
                break;
            }
        }
    }
    
    TimeInfo timeInfo(totalDistance);
    
    cout << "\n=== Route Details ===\n";
    cout << "From: " << stations[fromStation].name << "\n";
    cout << "To: " << stations[toStation].name << "\n";
    cout << "Distance: " << totalDistance << " km\n";
    cout << "Route Path: " << getRoutePathString(path) << "\n";
    cout << "Estimated Travel Time: " << (int)timeInfo.hours << " hours " 
         << (int)timeInfo.minutes << " minutes\n";
    
    // Select ticket class
    displayFareOptions();
    cout << "Select ticket class (1-4): ";
    int classChoice;
    cin >> classChoice;
    
    if (classChoice < 1 || classChoice > 4) {
        cout << "Invalid ticket class selection!\n";
        return;
    }
    
    TicketClass selectedClass = static_cast<TicketClass>(classChoice);
    int fare = getFareForClass(totalDistance, selectedClass);
    
    // Create booking
    Booking newBooking(nextTicketNumber, passengerName, age, fromStation, toStation,
                       selectedClass, totalDistance, fare, timeInfo.hours, currentUser);
    bookings.push_back(newBooking);
    
    // Add booking ID to user's booking list
    for (int i = 0; i < users.size(); i++) {
        if (users[i].username == currentUser) {
            users[i].bookingIds.push_back(nextTicketNumber);
            break;
        }
    }
    
    // Display booking confirmation
    cout << "\n=== Booking Confirmation ===\n";
    cout << "Ticket Number: " << nextTicketNumber << "\n";
    cout << "Passenger Name: " << passengerName << "\n";
    cout << "Age: " << age << "\n";
    cout << "From: " << stations[fromStation].name << "\n";
    cout << "To: " << stations[toStation].name << "\n";
    cout << "Class: " << getClassString(selectedClass) << "\n";
    cout << "Distance: " << totalDistance << " km\n";
    cout << "Travel Time: " << (int)timeInfo.hours << " hours " 
         << (int)timeInfo.minutes << " minutes\n";
    cout << "Fare: ₹" << fare << "\n";
    cout << "Status: Confirmed\n";
    cout << "\nBooking successful! Please save your ticket number.\n";
    
    nextTicketNumber++;
}

void viewMyBookings() {
    if (currentUser == "") {
        cout << "Please login first!\n";
        return;
    }
    
    cout << "\n=== My Bookings ===\n";
    bool hasBookings = false;
    
    for (int i = 0; i < bookings.size(); i++) {
        if (bookings[i].username == currentUser) {
            hasBookings = true;
            cout << "\n--- Ticket #" << bookings[i].ticketNumber << " ---\n";
            cout << "Passenger: " << bookings[i].passengerName << "\n";
            cout << "Age: " << bookings[i].age << "\n";
            cout << "From: " << stations[bookings[i].fromStation].name << "\n";
            cout << "To: " << stations[bookings[i].toStation].name << "\n";
            cout << "Class: " << getClassString(bookings[i].ticketClass) << "\n";
            cout << "Distance: " << bookings[i].distance << " km\n";
            cout << "Travel Time: " << (int)bookings[i].travelTime << " hours " 
                 << (int)((bookings[i].travelTime - (int)bookings[i].travelTime) * 60) << " minutes\n";
            cout << "Fare: ₹" << bookings[i].fare << "\n";
            cout << "Status: " << (bookings[i].isActive ? "Active" : "Cancelled") << "\n";
        }
    }
    
    if (!hasBookings) {
        cout << "No bookings found.\n";
    }
}

void cancelBooking() {
    if (currentUser == "") {
        cout << "Please login first!\n";
        return;
    }
    
    cout << "\n=== Cancel Booking ===\n";
    cout << "Enter ticket number to cancel: ";
    int ticketNumber;
    cin >> ticketNumber;
    
    for (int i = 0; i < bookings.size(); i++) {
        if (bookings[i].ticketNumber == ticketNumber && 
            bookings[i].username == currentUser && 
            bookings[i].isActive) {
            
            bookings[i].isActive = false;
            cout << "\nTicket #" << ticketNumber << " has been cancelled successfully.\n";
            cout << "Passenger: " << bookings[i].passengerName << "\n";
            cout << "Route: " << stations[bookings[i].fromStation].name 
                 << " -> " << stations[bookings[i].toStation].name << "\n";
            cout << "Refund Amount: ₹" << bookings[i].fare << "\n";
            return;
        }
    }
    
    cout << "Ticket not found or already cancelled!\n";
}

void printPath(vector<int>& prev, int dest, int src) {
    if (dest == src) {
        cout << stations[src].name;
        return;
    }
    if (prev[dest] == -1) {
        cout << "No path exists";
        return;
    }
    
    printPath(prev, prev[dest], src);
    cout << " -> " << stations[dest].name;
}

void dijkstraWithPath(int src, int dest = -1) {
    if (dest != -1) {
        // Use optimized Dijkstra for specific route
        vector<int> path = dijkstra(src, dest, stations);
        
        cout << "\n=== Route Information ===\n";
        cout << "From: " << stations[src].name << "\n";
        cout << "To: " << stations[dest].name << "\n";
        
        if (path.size() <= 1) {
            cout << "No route available!\n";
            return;
        }
        
    // Calculate total distance from path
    int totalDistance = 0;
    for (int i = 0; i < path.size() - 1; i++) {
        for (int j = 0; j < stations[path[i]].connections.size(); j++) {
            pair<int, int> conn = stations[path[i]].connections[j];
            if (conn.first == path[i + 1]) {
                totalDistance += conn.second;
                break;
            }
        }
    }
        
        cout << "Distance: " << totalDistance << " km\n";
        cout << "Route: ";
        for (int i = 0; i < path.size(); i++) {
            cout << stations[path[i]].name;
            if (i < path.size() - 1) cout << " -> ";
        }
        cout << "\n";
        
        // Get fare class choice
        displayFareOptions();
        cout << "Select ticket class (1-4): ";
        int classChoice;
        cin >> classChoice;
        
        if (classChoice >= 1 && classChoice <= 4) {
            calculateFare(totalDistance, static_cast<TicketClass>(classChoice));
        } else {
            cout << "Invalid choice! Showing all fares:\n";
            calculateFare(totalDistance, GENERAL);
        }
    } else {
        // Show all shortest paths using traditional Dijkstra
        int n = stations.size();
        vector<int> dist(n, INT_MAX);
        vector<int> prev(n, -1);
        vector<bool> visited(n, false);

        dist[src] = 0;

        for (int count = 0; count < n - 1; count++) {
            int minDist = INT_MAX, u = -1;
            for (int i = 0; i < n; i++) {
                if (!visited[i] && dist[i] <= minDist) {
                    minDist = dist[i];
                    u = i;
                }
            }

            if (u == -1) break;
            visited[u] = true;

            for (int i = 0; i < stations[u].connections.size(); i++) {
                int v = stations[u].connections[i].first;
                int weight = stations[u].connections[i].second;
                if (!visited[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    prev[v] = u;
                }
            }
        }

        cout << "\nShortest distances and routes from " << stations[src].name << ":\n";
        for (int i = 0; i < n; i++) {
            if (i != src) {
                cout << "\nTo " << stations[i].name << ": ";
                if (dist[i] == INT_MAX) {
                    cout << "Not Reachable\n";
                } else {
                    cout << dist[i] << " km\n";
                    cout << "Route: ";
                    printPath(prev, i, src);
                    cout << "\n";
                }
            }
        }
    }
}

void dijkstra(int src) {
    int n = stations.size();
    vector<int> dist(n, INT_MAX);
    vector<bool> visited(n, false);

    dist[src] = 0;

    for (int count = 0; count < n - 1; count++) {
        int minDist = INT_MAX, u;
        for (int i = 0; i < n; i++) {
            if (!visited[i] && dist[i] <= minDist) {
                minDist = dist[i], u = i;
            }
        }

        visited[u] = true;

        for (int i = 0; i < stations[u].connections.size(); i++) {
            int v = stations[u].connections[i].first;
            int weight = stations[u].connections[i].second;
            if (!visited[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
            }
        }
    }

    cout << "\nShortest distances from " << stations[src].name << ":\n";
    for (int i = 0; i < n; i++) {
        cout << stations[i].name << ": ";
        if (dist[i] == INT_MAX)
            cout << "Not Reachable\n";
        else
            cout << dist[i] << " km\n";
    }
}

int main() {
    addStation("New Delhi");
    addStation("Kolkata");
    addStation("Mumbai");
    addStation("Chennai");

    addConnection(0, 1, 1450);
    addConnection(0, 2, 1400);
    addConnection(1, 3, 1650);
    addConnection(2, 3, 1350);

    while (true) {
        cout << "\n=== Smart Railway Management System ===\n";
        
        if (currentUser == "") {
            cout << "🔐 Authentication Required\n";
            cout << "1. Register New User\n";
            cout << "2. Login\n";
            cout << "3. Show All Stations (Guest)\n";
            cout << "4. Find Route & Fare (Guest)\n";
            cout << "5. Exit\n";
        } else {
            cout << "👤 Welcome, " << currentUser << "!\n";
            cout << "1. Show All Stations\n";
            cout << "2. Find Shortest Paths from a Station\n";
            cout << "3. Find Route & Fare Between Two Stations\n";
            cout << "4. Book Ticket\n";
            cout << "5. View My Bookings\n";
            cout << "6. Cancel Booking\n";
            cout << "7. Logout\n";
            cout << "8. Exit\n";
        }
        
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        if (currentUser == "") {
            // Guest/Authentication menu
            if (choice == 1) {
                registerUser();
            } else if (choice == 2) {
                loginUser();
            } else if (choice == 3) {
                displayStations();
            } else if (choice == 4) {
                displayStations();
                cout << "Enter source station index: ";
                int src;
                cin >> src;
                cout << "Enter destination station index: ";
                int dest;
                cin >> dest;
                
                if (src >= 0 && src < stations.size() && dest >= 0 && dest < stations.size() && src != dest) {
                    dijkstraWithPath(src, dest);
                } else {
                    cout << "Invalid station indices or same source and destination!\n";
                }
            } else if (choice == 5) {
                break;
            } else {
                cout << "Invalid choice. Try again.\n";
            }
        } else {
            // Logged in user menu
            if (choice == 1) {
                displayStations();
            } else if (choice == 2) {
                displayStations();
                cout << "Enter station index to find shortest paths: ";
                int src;
                cin >> src;
                if (src >= 0 && src < stations.size()) {
                    dijkstraWithPath(src);
                } else {
                    cout << "Invalid station index!\n";
                }
            } else if (choice == 3) {
                displayStations();
                cout << "Enter source station index: ";
                int src;
                cin >> src;
                cout << "Enter destination station index: ";
                int dest;
                cin >> dest;
                
                if (src >= 0 && src < stations.size() && dest >= 0 && dest < stations.size() && src != dest) {
                    dijkstraWithPath(src, dest);
                } else {
                    cout << "Invalid station indices or same source and destination!\n";
                }
            } else if (choice == 4) {
                bookTicket();
            } else if (choice == 5) {
                viewMyBookings();
            } else if (choice == 6) {
                cancelBooking();
            } else if (choice == 7) {
                logoutUser();
            } else if (choice == 8) {
                break;
            } else {
                cout << "Invalid choice. Try again.\n";
            }
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nPress Enter to continue...";
        cin.get();
    }

    return 0;
}