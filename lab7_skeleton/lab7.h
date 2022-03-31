#ifndef LAB7_H
#define LAB7_H

#include <string>
#include <vector>
using namespace std;

/* Global constant definition */
const int NUM_PATIENTS = 25;
const int NUM_DAYS = 10;
const int MAX_ROOM = 50;
const int MAX_COVID_INFECTED = 100;
const std::string ROOM_OCCUPIED = "The room is occupied!\n";
const std::string NOT_ELIGIBILE = "Not eligible for this room number!\n";


/* User-defined data types */
enum Diseases{
    NO_DISEASE = 0,
    COVID = 1,
    DIABETES = 2,
    HEART_FAILURE = 3,
    KIDNEY_FAILURE = 4,
    CHICKEN_POX = 5,
    NULL_DISEASE = 6
};

struct Patients{
    char type;
    Diseases disease;
    bool covid_infected;
};

struct Room{
    bool occupied = false;
    int duration_left = 0;

};

/* External Function Declaration */

/* =================================== TASK 1 =================================== 
   Declare the external function in lab7.h and declare the external variable in utils.cpp
*/

//TODO

#endif