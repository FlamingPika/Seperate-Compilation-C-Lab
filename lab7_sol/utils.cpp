#include <iostream>
#include <fstream>

#include "lab7.h"
using namespace std;

/* =================================== TASK 1 =================================== 
   Declare the external function in lab7.h and declare the external variable in utils.cpp
*/
extern int num_covid_patients[NUM_DAYS];

void readPatient(string filename, int list[][NUM_PATIENTS]){
    ifstream file;
    file.open(filename);
    for(int i = 0; i < NUM_DAYS; ++i){
        for(int j = 0; j < NUM_PATIENTS; ++j){
            file >> list[i][j];
        }
    }
}

void update(Room r[], int size){
    for(int i = 0; i < size; ++i){
        if (r[i].occupied){
            r[i].duration_left--;
        }
        if (r[i].duration_left == 0){
            r[i].occupied = false;
        }
    }
}

void registerPatient(int list[][NUM_PATIENTS], Patients sick_patient[][NUM_PATIENTS]){
    Diseases current_disease = NULL_DISEASE;
    int same_disease = 0;
    char current_type;
    for(int i = 0; i < NUM_DAYS; ++i){
        for(int j = 0; j < NUM_PATIENTS; ++j){

            switch(static_cast<Diseases>(list[i][j])){
                case COVID:
                    sick_patient[i][j].disease = COVID; 
                    sick_patient[i][j].covid_infected = true;
                    break;
                case DIABETES:
                    sick_patient[i][j].disease = DIABETES; 
                    sick_patient[i][j].covid_infected = false;
                    break;
                case HEART_FAILURE:
                    sick_patient[i][j].disease = HEART_FAILURE; 
                    sick_patient[i][j].covid_infected = false;
                    break;
                case KIDNEY_FAILURE:
                    sick_patient[i][j].disease = KIDNEY_FAILURE; 
                    sick_patient[i][j].covid_infected = false;
                    break;
                case CHICKEN_POX:
                    sick_patient[i][j].disease = CHICKEN_POX; 
                    sick_patient[i][j].covid_infected = false;
                    break;
                default:
                    sick_patient[i][j].disease = NO_DISEASE; 
                    sick_patient[i][j].covid_infected = false;
            }

            if (sick_patient[i][j].disease == current_disease){
                same_disease++;
            }
            else{

                current_disease = sick_patient[i][j].disease;

                if(same_disease == 0){
                    current_type = 'S';
                }
                else if(same_disease == 1){
                    current_type = 'C';
                }

                for(int k = 1; k <= (same_disease+1); ++k){
                    sick_patient[i][j-k].type = current_type;
                }

                same_disease = 0;
            }
        }

        current_disease = NULL_DISEASE;
        if(same_disease == 0){
            current_type = 'S';
        }
        else if(same_disease == 1){
            current_type = 'C';
        }
        
        for(int k = 0; k <= same_disease; ++k){
            sick_patient[i][NUM_PATIENTS-1-k].type = current_type;
        }
    
    }

}

/*
    =================================== TASK 2 ===================================
    Run through the sick_patient list and register each patient that got infected
    by covid. In addition, count the number of covid-positive patient each day and
    add it to num_covid_patients list where the index+1 represents the day
    ex. index = 0 denotes the first day.

*/

void registerCovidPatient(Patients covid_list[][MAX_COVID_INFECTED], Patients sick_patient[][NUM_PATIENTS]){
    int counter = 0;
    for(int i = 0; i < NUM_DAYS; ++i){
        for(int j = 0; j < NUM_PATIENTS; ++j){
            if(sick_patient[i][j].disease == COVID){
                covid_list[i][counter] = sick_patient[i][j];
                counter++;
            }
        }
        num_covid_patients[i] = counter;
        counter = 0;
    }

}

/*
    =================================== TASK 3 ===================================
    Complete all 3 functions below to complete the room checking function. Write a 
    simple code to check whether the room number is a prime in checkPrime() and implement
    it into roomEligibility() to check whether the requested room fulfill the criteria.
    Then, implement and roomEligibility() into checkRoomAvailability().
    
    Make sure to modify the value of OCCUPIED and DURATION_LEFT member / object 
    inside the struct Room based on the corresponding duration and use the string 
    constant that have been defined in lab7.h as the text output warning.

*/

int checkPrime(int num){
    if(num == 1){ 
        return 0;
    }
    for(int i = 2; i <= num/2; ++i){
        if(num % i == 0){
            return 0;
        }
    }
    return num;
}

bool roomEligibility(int num){
    int counter = 1;
    for(int i = 1; i <= MAX_ROOM; ++i){
        if(checkPrime(i)){
            if(i+counter == num){
                return true;
            }
            else{
                ++counter;
            }
        }
        else if(i > num){
            break;
        }
    }
    return false;
}

bool checkRoomAvailability(Room rooms[], char type, int room_req){
    switch(type){
        case 'S':
            if(rooms[room_req-1].occupied){
                cout << ROOM_OCCUPIED << endl;
                return false;
            }
            else{
                rooms[room_req-1].occupied = true;
                rooms[room_req-1].duration_left = 5;
            }
            break;

        case 'C':
            if(roomEligibility(room_req)){
                if(rooms[room_req-1].duration_left){
                    cout << ROOM_OCCUPIED << endl;
                    return false;
                }
                rooms[room_req-1].occupied = true;
                rooms[room_req-1].duration_left = 4;
            }
            else{
                cout << NOT_ELIGIBILE << endl;
                return false;
            }
            break;
    
    }
    return true;

}

/*
    =================================== TASK 4 ===================================
    Since we have increased the value of day in lab7.cpp, fetch the patient from 
    covid_list for TODAY to the active_array. Make sure to implement the circular
    array concept and increase the value of fill_index

    return: the number of newly added patient from covid_list

*/

int transferringPatient(Patients covid_list[][MAX_COVID_INFECTED], Patients active_array[], int& fill_index, int day_index){
    int transfer_patient = 0;
    for(int i = 0; i < num_covid_patients[day_index % NUM_DAYS]; ++i){
        active_array[fill_index % NUM_PATIENTS] = covid_list[day_index][i];
        fill_index++;
        ++transfer_patient;
    }
    return transfer_patient;

}

