#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "123admin"

struct USER
{
    char username[50];
    char password[100];
    char role[50];
};

struct BUS
{
    int busid;
    char company[100];
    char type[50]; //ac non-ac
    int seats;
};

struct ROUTE
{
    int routeid;
    char start[100];
    char end[100];
    float fare;
};

struct RESERVATION
{
    int reservationid;
    char username[50];
    int busid;
    int routeid;
    int seatnum;
    char date[50];
    char day[50];
    char month[50];
    char year[50];
};

char role[50]; // global role
char loggedinusername[50];

void registeruser();
int loginuser();
void addbus();
void viewbuses();
void addroute();
void viewroutes();
void makereservation(char *username);
void viewreservations(char *username);
void viewavailabletickets();

int main()
{
    int choice;
    srand(time(NULL));

    while(1)
    {
        system("cls");
        printf("---- WELCOME TO SSF BUS TICKETING SERVICE ----");
        printf("\n1.Login\n2.Register\n3.Exit\n\nEnter your choice: ");
        scanf("%d",&choice);
        getchar();

        switch (choice) {
        case 1:
            if (loginuser()) {  //uses the global role
                if (strcmp(role, "admin") == 0) {
                    int adminchoice;
                    while (1) {
                        system("cls");
                        printf("----- Admin Menu -----\n\n");
                        printf("1.Add Bus\n2.View Buses\n3.Add Route\n4.View Routes\n5.View Available Tickets\n6.Logout\nEnter your choice: ");
                        scanf("%d",&adminchoice);
                        switch (adminchoice) {
                        case 1:
                            addbus();
                            break;
                        case 2:
                            viewbuses();
                            break;
                        case 3:
                            addroute();
                            break;
                        case 4:
                            viewroutes();
                            break;
                        case 5:
                            viewavailabletickets();
                            break;
                        case 6:
                            break;
                        default:
                            printf("Invalid choice!\n");
                            system("pause");
                        }
                        if (adminchoice == 6)
                            break;
                    }
                } else {
                    int userchoice;

                    while (1) {
                        system("cls");
                        printf("---- User Menu ----\n\n");
                        printf("1.Make Reservation\n2.View Reservations\n3.View Available Tickets\n4.Logout\nEnter your choice: ");
                        scanf("%d", &userchoice);
                        switch (userchoice) {
                        case 1:
                            makereservation(loggedinusername);
                            break;
                        case 2:
                            viewreservations(loggedinusername);
                            break;
                        case 3:
                            viewavailabletickets();
                            break;
                        case 4:
                            break;
                        default:
                            printf("Invalid choice!\n");
                            system("pause");
                        }
                        if (userchoice == 4)
                            break;
                    }
                }
            }
            break;
        case 2:
            registeruser();
            break;
        case 3:
            exit(0);
        default:
            printf("Invalid choice\n");
            system("pause");
        }
    }
}

void registeruser()
{
    fflush(stdin);
    struct USER user;
    FILE *file=fopen("users.dat","a");
    if (!file)
    {
        printf("Error opening file!\n");
        return;
    }
    printf("Enter username: ");
    fgets(user.username,sizeof(user.username),stdin);
    user.username[strcspn(user.username,"\n")]='\0';
    printf("Enter password: ");
    fgets(user.password, sizeof(user.password), stdin);
    user.password[strcspn(user.password, "\n")] = '\0';

    strcpy(user.role, "user");
    fprintf(file,"%s,%s,%s\n",user.username,user.password,user.role);
    fclose(file);
    printf("User registered successfully!\n");
    system("pause");
}

int loginuser()
{
    fflush(stdin);
    struct USER user;
    FILE *file = fopen("users.dat","r");
    if (!file) {
        printf("Error opening file!\n");
        return 0;
    }

    char username[50], password[50];
    printf("Enter username: ");
    fgets(username,sizeof(username),stdin);
    username[strcspn(username, "\n")]=0;
    printf("Enter password: ");
    fgets(password,sizeof(password),stdin);
    password[strcspn(password,"\n")]=0;

    if (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0) {
        strcpy(role,"admin");
        fclose(file);
        printf("Admin login successful!\n");
        system("pause");
        return 1;
    }

    char buffer[200];
    while (fgets(buffer,sizeof(buffer),file))
    {
        sscanf(buffer,"%[^,],%[^,],%s",user.username,user.password,user.role);
        if (strcmp(user.username,username)==0 && strcmp(user.password,password)==0)
        {
            strcpy(role,user.role);
            strcpy(loggedinusername,user.username);
            fclose(file);
            printf("User login successful!\n");
            system("pause");
            return 1;
        }
    }

    fclose(file);
    printf("Invalid credentials!\n");
    return 0;
}

void addbus()
{
    struct BUS bus;
    FILE *file=fopen("buses.dat","a");
    if (!file)
    {
        printf("Error opening file!\n");
        return;
    }

    fflush(stdin);
    printf("Enter bus ID: ");
    scanf("%d",&bus.busid);
    getchar();
    printf("Enter Company Name: ");
    fgets(bus.company,sizeof(bus.company),stdin);
    bus.company[strcspn(bus.company,"\n")]=0;
    printf("Enter Bus Type (AC/Non-AC): ");
    fgets(bus.type, sizeof(bus.type), stdin);
    bus.type[strcspn(bus.type,"\n")]=0;
    printf("Enter Number of Seats: ");
    scanf("%d",&bus.seats);
    getchar();

    fprintf(file,"%d,%s,%s,%d\n",bus.busid,bus.company,bus.type,bus.seats);
    fclose(file);
    printf("Bus added successfully!\n");
    system("pause");
    return;
}


void viewbuses()
{
    struct BUS bus;
    FILE *file=fopen("buses.dat","r");

    if (!file)
    {
        printf("Error opening buses file!\n");
        return;
    }

    printf("\nAvailable Buses:\n");
    printf("------------------------------------------------------\n");
    printf("Bus ID | Company Name          | Type      | Seats\n");
    printf("------------------------------------------------------\n");

    while (fscanf(file,"%d,%99[^,],%49[^,],%d\n",&bus.busid,bus.company,bus.type,&bus.seats)==4)
    {
        printf("%-6d | %-20s | %-9s | %d\n",bus.busid,bus.company,bus.type,bus.seats);
    }
    fclose(file);
    system("pause");
}

void addroute() {
    struct ROUTE route;
    FILE *file=fopen("routes.dat","a");

    if (!file) {
        printf("Error opening routes file!\n");
        system("pause");
        return;
    }

    fflush(stdin);
    printf("Enter route ID: ");
    scanf("%d",&route.routeid);
    getchar();
    printf("Enter starting point: ");
    fgets(route.start,sizeof(route.start),stdin);
    route.start[strcspn(route.start,"\n")]='\0';
    printf("Enter destination: ");
    fgets(route.end,sizeof(route.end),stdin);
    route.end[strcspn(route.end,"\n")]='\0';
    printf("Enter Fare: ");
    scanf("%f",&route.fare);

    fprintf(file,"%d,%s,%s,%.2f\n",route.routeid,route.start,route.end,route.fare);
    fclose(file);
    printf("Route added successfully!\n");
    system("pause");
}


void viewroutes()
{
    struct ROUTE route;
    FILE *file=fopen("routes.dat","r");
    if (!file)
    {
        printf("Error opening routes file!\n");
        return;
    }

    printf("\nAvailable Routes:\n");
    printf("------------------------------------------------------------\n");
    printf("Route ID | Start Point          | Destination        | Fare\n");
    printf("------------------------------------------------------------\n");
    while (fscanf(file, "%d,%99[^,],%99[^,],%f\n",&route.routeid,route.start,route.end,&route.fare)==4)
    {
        printf("%-8d | %-20s | %-20s | %.2f\n",route.routeid,route.start,route.end,route.fare);
    }
    fclose(file);
    system("pause");
}


void makereservation(char *username) {
    struct RESERVATION reservation;
    struct BUS bus;
    struct ROUTE route;
    FILE *busfile = fopen("buses.dat", "r");
    FILE *reservationfile = fopen("reservations.dat", "a+");  // Open in append and read mode
    FILE *existingReservations = fopen("reservations.dat", "r");
    FILE *routefile = fopen("routes.dat", "r");

    if (!busfile || !reservationfile || !existingReservations || !routefile) {
        printf("Error opening file(s)!\n");
        return;
    }

    int bookedSeats[100] = {0}; // Assume max seats 100 per bus
    char existingDate[11]; // For checking if the reservation date matches

    // Display available buses
    printf("\nAvailable Buses:\n");
    printf("Bus ID | Company          | Type     | Seats\n");
    printf("--------------------------------------------------\n");
    while (fscanf(busfile, "%d,%99[^,],%49[^,],%d\n", &bus.busid, bus.company, bus.type, &bus.seats) == 4) {
        printf("%-7d | %-15s | %-8s | %d\n", bus.busid, bus.company, bus.type, bus.seats);
    }
    rewind(busfile); // Reset file pointer for busfile

    // Display available routes
    printf("\nAvailable Routes:\n");
    printf("Route ID | Source          | Destination    | Price\n");
    printf("-------------------------------------------------------\n");
    while (fscanf(routefile, "%d,%49[^,],%49[^,],%f\n", &route.routeid, route.start, route.end, &route.fare) == 4) {
        printf("%-8d | %-15s | %-15s | %.2f\n", route.routeid, route.start, route.end, route.fare);
    }
    rewind(routefile);

    printf("\nEnter Bus ID for reservation: ");
    scanf("%d", &reservation.busid);

    int validBus = 0, totalSeats = 0;
    while (fscanf(busfile, "%d,%99[^,],%49[^,],%d\n", &bus.busid, bus.company, bus.type, &bus.seats) == 4) {
        if (bus.busid == reservation.busid) {
            validBus = 1;
            totalSeats = bus.seats;
            break;
        }
    }

    if (!validBus) {
        printf("Invalid Bus ID!\n");
        fclose(busfile);
        fclose(reservationfile);
        fclose(existingReservations);
        fclose(routefile);
        system("pause");
        return;
    }

    // Load booked seats for this bus and check for existing reservations on the same date
    while (fscanf(existingReservations, "%d,%49[^,],%d,%d,%d,%49[^\n]\n",
                  &reservation.reservationid, reservation.username,
                  &reservation.busid, &reservation.routeid,
                  &reservation.seatnum, existingDate) == 6) {
        if (reservation.busid == bus.busid && strcmp(existingDate, reservation.date) == 0) {
            bookedSeats[reservation.seatnum - 1] = 1;  // Mark seat as booked
        }
    }

    printf("\nChoose your seat: ");
    int anyAvailable = 0;
    for (int i = 0; i < totalSeats; i++) {
        if (!bookedSeats[i]) {
            printf("%d ", i + 1);
            anyAvailable = 1;
        }
    }
    if (!anyAvailable) {
        printf("No available seats.\n");
        fclose(busfile);
        fclose(reservationfile);
        fclose(existingReservations);
        fclose(routefile);
        system("pause");
        return;
    }
    printf("\n");

    // Seat selection
    do {
        printf("Enter Seat Number: ");
        scanf("%d", &reservation.seatnum);
        if (reservation.seatnum < 1 || reservation.seatnum > totalSeats || bookedSeats[reservation.seatnum - 1]) {
            printf("Invalid or already booked seat. Please try again.\n");
        }
    } while (reservation.seatnum < 1 || reservation.seatnum > totalSeats || bookedSeats[reservation.seatnum - 1]);

    // Select route
    printf("Enter Route ID for reservation: ");
    scanf("%d", &reservation.routeid);
    getchar();

    // Validate Route ID
    int validRoute = 0;
    while (fscanf(routefile, "%d,%49[^,],%49[^,],%f\n", &route.routeid, route.start, route.end, &route.fare) == 4) {
        if (route.routeid == reservation.routeid) {
            validRoute = 1;
            break;
        }
    }

    if (!validRoute) {
        printf("Invalid Route ID!\n");
        fclose(busfile);
        fclose(reservationfile);
        fclose(existingReservations);
        fclose(routefile);
        system("pause");
        return;
    }

    // Validate and format the date
    int validDate = 0;
    do {
        printf("Enter Date of Travel (DD-MM-YYYY): ");
        fgets(reservation.date, sizeof(reservation.date), stdin);
        reservation.date[strcspn(reservation.date, "\n")] = '\0';

        // Check if the date has correct format (DD-MM-YYYY)
        if (strlen(reservation.date) == 10 && reservation.date[2] == '-' && reservation.date[5] == '-') {
            // Try to validate date components
            int day, month, year;
            if (sscanf(reservation.date, "%d-%d-%d", &day, &month, &year) == 3) {
                if (day >= 1 && day <= 31 && month >= 1 && month <= 12 && year > 0) {
                    validDate = 1;
                }
            }
        }

        if (!validDate) {
            printf("Invalid date format. Please enter the date in DD-MM-YYYY format.\n");
        }

    } while (!validDate);  // Repeat until a valid date is entered

    // Generate Reservation ID randomly
    reservation.reservationid = rand() % 10000;  // Random ID generation for uniqueness

    strcpy(reservation.username, username);

    // Calculate final price (considering AC ticket price multiplier)
    float finalPrice = route.fare;
    if (strcmp(bus.type, "AC") == 0) {
        finalPrice *= 1.5;  // Apply 1.5x multiplier for AC buses
    }

    // Save the new reservation
    fprintf(reservationfile, "%d,%s,%d,%d,%d,%s\n",
            reservation.reservationid, reservation.username,
            reservation.busid, reservation.routeid,
            reservation.seatnum, reservation.date);

    fclose(busfile);
    fclose(reservationfile);
    fclose(existingReservations);
    fclose(routefile);

    // Display the final price
    printf("Reservation made successfully! Your Reservation ID is: %d\n", reservation.reservationid);
    printf("Total price for your ticket: %.2f\n", finalPrice);

    system("pause");
}




void viewreservations(char *username) {
    struct RESERVATION reservation;
    FILE *reservationfile = fopen("reservations.dat", "r");

    if (!reservationfile) {
        printf("Error opening reservations file!\n");
        return;
    }

    printf("\nReservations for User: %s\n", username);
    printf("------------------------------------------------------------\n");
    printf("Reservation ID | Bus ID | Route ID | Seat Number | Date\n");
    printf("------------------------------------------------------------\n");

    int found = 0;
    char buffer[200];  // To hold the entire line
    while (fgets(buffer, sizeof(buffer), reservationfile)) {

        // Try to parse the line in the format: reservationid, username, busid, routeid, seatnum, DD-MM-YY
        if (sscanf(buffer, "%d,%49[^,],%d,%d,%d,%s",
                   &reservation.reservationid, reservation.username,
                   &reservation.busid, &reservation.routeid,
                   &reservation.seatnum, reservation.date) == 6) {

            // Manually parse the date into day, month, and year
            int day, month, year;
            if (sscanf(reservation.date, "%d-%d-%d", &day, &month, &year) == 3) {
                // Store the parsed date in the correct format
                sprintf(reservation.date, "%02d-%02d-%02d", day, month, year);

                // Check if the username matches
                if (strcmp(reservation.username, username) == 0) {
                    printf("%-15d | %-6d | %-8d | %-11d | %s\n",
                           reservation.reservationid, reservation.busid,
                           reservation.routeid, reservation.seatnum,
                           reservation.date);
                    found = 1;
                }
            }
        }
    }

    if (!found) {
        printf("No reservations found for this user.\n");
    }

    fclose(reservationfile);
    system("pause");
}



void viewavailabletickets() {
    struct BUS bus;
    struct RESERVATION reservation;
    struct ROUTE route;
    FILE *busfile = fopen("buses.dat", "r");
    FILE *reservationfile = fopen("reservations.dat", "r");
    FILE *routefile = fopen("routes.dat", "r");

    if (!busfile || !reservationfile || !routefile) {
        printf("Error opening file(s)!\n");
        system("pause");
        return;
    }

    int bookedSeats[100] = {0};  // Array to track booked seats for each bus (assuming max 100 buses)

    // Read reservations and mark booked seats
    char buffer[200];
    while (fgets(buffer,sizeof(buffer),reservationfile)) {
        if (sscanf(buffer,"%d,%[^,],%d,%d,%[^,\n]",
                   &reservation.reservationid,
                   reservation.username,
                   &reservation.busid,
                   &reservation.seatnum,
                   reservation.date) == 5) {
            if (reservation.busid >= 0 && reservation.busid < 10000 &&
                reservation.seatnum > 0 && reservation.seatnum <= 100) {
                bookedSeats[reservation.busid] |= (1 << (reservation.seatnum - 1)); // Mark seat as booked
            }
        }
    }
    fclose(reservationfile);

    printf("\nAvailable Tickets:\n");
    printf("--------------------------------------------------------\n");

    // Read buses and display available seats with fare
    while (fgets(buffer, sizeof(buffer), busfile)) {
        if (sscanf(buffer, "%d,%[^,],%[^,],%d",
                   &bus.busid,
                   bus.company,
                   bus.type,
                   &bus.seats) == 4) {

            // Display Bus Information
            printf("Bus ID: %d | Company: %s | Type: %s | Total Seats: %d\n",
                   bus.busid, bus.company, bus.type, bus.seats);

            // Calculate fare adjustment for AC buses
            float fareMultiplier = (strcmp(bus.type, "AC") == 0) ? 1.5 : 1.0;
            float baseFare = 0.0;

            // Find corresponding route fare
            rewind(routefile);  // Reset file pointer for route file
            while (fgets(buffer, sizeof(buffer), routefile)) {
                if (sscanf(buffer, "%d,%[^,],%[^,],%f",
                           &route.routeid, route.start, route.end, &route.fare) == 4) {
                    baseFare = route.fare;
                    printf("Base Fare: %.2f | Adjusted Fare: %.2f\n",
                           baseFare, baseFare * fareMultiplier);
                    break;
                }
            }

            printf("Available Seats: ");
            int anyAvailable = 0;
            for (int i = 1; i <= bus.seats; i++) {
                if (!(bookedSeats[bus.busid] & (1 << (i - 1)))) {
                    printf("%d ", i); // Print available seat number
                    anyAvailable = 1;
                }
            }

            if (!anyAvailable) {
                printf("No available seats\n");
            }

            printf("\n--------------------------------------------------------\n");
        }
    }

    fclose(busfile);
    fclose(routefile);
    system("pause");
}
