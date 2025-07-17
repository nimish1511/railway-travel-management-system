
## Smart Railway Management System

This project is a command-line based Smart Railway Management System that utilizes graph data structures and Dijkstra’s algorithm to determine the shortest path and fare between stations. Key features include:

* User registration and authentication
* Ticket booking and cancellation
* Route optimization and shortest path calculation
* Fare and travel time estimation

---

## How to Run the Program

### View All Stations

Select **Option 3** from the main menu.
This will display a list of all available stations along with their corresponding index numbers.

### Find Route and Calculate Fare

Select **Option 4** from the main menu.

* Enter the source station index
* Enter the destination station index
* Select the desired ticket class (1 to 4)
* The system will provide the route, distance, estimated travel time, and calculated fare

---

## User Features

### Register a New User

Select **Option 1** from the main menu.

* Provide a unique username and password
* A confirmation message will be displayed upon successful registration

### Login

Select **Option 2** from the main menu.

* Enter your registered username and password
* On successful login, the user menu will be accessible

---

## Features Available After Login

### Find Shortest Paths from a Station

Select **Option 2** from the user menu.

* Enter the source station index
* View all reachable stations with shortest distances and respective routes

### Find Route and Fare Between Two Stations

Select **Option 3** from the user menu.

* Enter source and destination station indices
* Select the ticket class
* View the complete route details along with distance, time, and fare

### Book a Ticket

Select **Option 4** from the user menu.

* Enter passenger name and age
* Provide source and destination station indices
* Choose a ticket class
* A booking confirmation with a unique ticket number will be displayed

### View Your Bookings

Select **Option 5** from the user menu.

* Displays a list of active and cancelled bookings associated with the logged-in user

### Cancel a Booking

Select **Option 6** from the user menu.

* Enter the ticket number to cancel
* A confirmation message will be shown upon successful cancellation

---

## Test Cases

### Test Case 1: Route and Fare Calculation

1. Run the program
2. Select Option 3 to view all stations
3. Choose Option 4 to calculate route and fare
4. Enter:

   * Source station index: 0 (New Delhi)
   * Destination station index: 3 (Chennai)
   * Ticket class: 3 (AC)
5. Verify the displayed route, travel time, distance, and fare

### Test Case 2: User Registration and Login

1. Run the program
2. Select Option 1 to register

   * Username: testuser
   * Password: password123
3. Select Option 2 to login
4. Verify successful login through access to the user menu

### Test Case 3: Ticket Booking and Management

1. Login with your credentials
2. Select Option 4 to book a ticket
3. Provide:

   * Passenger name and age
   * Source and destination stations
   * Ticket class
4. Note the generated ticket number
5. Use Option 5 to view bookings
6. Use Option 6 to cancel the booking by entering the ticket number
7. Recheck bookings (Option 5) to verify cancellation

---

## Preloaded Station Data

### Stations:

| Index | Station   |
| ----- | --------- |
| 0     | New Delhi |
| 1     | Kolkata   |
| 2     | Mumbai    |
| 3     | Chennai   |

### Direct Connections:

* New Delhi to Kolkata: 1450 km
* New Delhi to Mumbai: 1400 km
* Kolkata to Chennai: 1650 km
* Mumbai to Chennai: 1350 km

