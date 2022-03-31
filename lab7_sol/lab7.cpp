#include <iostream>

#include "lab7.h"
using namespace std;

int num_covid_patients[NUM_DAYS] = {0};

/* Helper function to debug */
void printList(int a[]){                                                //to print the number of covid per day
    int temp = 0;
    cout << "\nTotal cases from the hospital" << '\n' << string(20, '=') << endl;
    for(int i = 0; i < NUM_DAYS; ++i){
        cout << "Day " << i+1 << ". " << a[i] << " covid cases"<< endl;
        temp += a[i];
    }
    cout << "TOTAL = " << temp << " cases" << endl;
}

void printList(int a[][NUM_PATIENTS]){                                  //to print the list from input file
    for(int i = 0; i < NUM_DAYS; ++i){
        for(int j = 0; j < NUM_PATIENTS; ++j){
            cout << a[i][j] << '\t';
        }
        cout << endl;
    }
}

void printList(Patients a[], int size, int start, int end, bool x){     //to print all patients in the active list

    cout << "\nACTIVE PATIENT LIST" << endl;     
    cout << string(101, '=')<< endl;                          
    cout << "| ";
    for(int i = start; i < NUM_PATIENTS; ++i){
        cout << a[i].type << " | ";
    }
    if(NUM_PATIENTS - start < NUM_PATIENTS && x){
        for(int i = 0; i < end ; ++i){
            cout << a[i].type << " | ";
        }
    }
    cout << endl;
    cout << string(101, '=') << endl;
    cout << " ";
    for(int i = 0; i < size; ++i){
        if(i < 10)
            cout << " " << i << "  ";
        if(i >= 10)
            cout << " " << i << " ";
    }
    cout << endl;
}

void printList(Room r[]){                                               //to print the room list
    cout << endl;
    cout << string(35, '=') << " ROOMS " << string(35, '=') << endl;
    for(int i = 0; i < MAX_ROOM; ++i){
        if (i % 10 == 0 && i)
            cout << endl;
        cout << i + 1 << ". " << r[i].duration_left << '\t';
    }
    cout << '\n' << endl;
}

/* Helper function ends here */


int main(){
    char raw_data[3][12] = {"month_1.txt", "month_2.txt", "month_3.txt"};
    Room rooms[MAX_ROOM] = {};
    Patients covid_list[NUM_DAYS][MAX_COVID_INFECTED];
    Patients sick_patient[NUM_DAYS][NUM_PATIENTS];
    Patients active_list[NUM_PATIENTS];
    int hospital_visitor[NUM_DAYS][NUM_PATIENTS];

    bool running = true;
    int day_index = 0;
    int day = 1;
    int patient_index = 0;
    int fill_index = 0;
    int active_patient = 0;
    int cumulative = 0;
    int selected_room; int action;

    cout << string(10, '=') << " WELCOME TO LAB 7 HOSPITAL BOOKING SYSTEM " << string(10, '=') << endl;
    cout << "Please choose which month do you want to process:" << endl;
    for(int i = 0; i < sizeof(raw_data)/sizeof(raw_data[0]); ++i){
        printf("%d. %s", i+1, raw_data[i]);
        cout << endl;
    }
    int selected_file;
    cin >> selected_file;

    cout << "\nRetrieving all hospital visitor history..." << endl;
    readPatient(raw_data[selected_file-1], hospital_visitor);

    cout << "Registering all patient..." << endl;
    registerPatient(hospital_visitor, sick_patient);

    cout << "Registering all covid patient..." << endl;
    registerCovidPatient(covid_list, sick_patient);

    printList(num_covid_patients);

    active_patient = transferringPatient(covid_list, active_list, fill_index, day_index);
    cumulative = active_patient;

    while (running)
    {
        cout << "\nDAY " << day << " - Number of covid = " << active_patient << endl;
        cout << "Please pick an action: " << endl;
        cout << "1. Assign patient to a room" << endl;
        cout << "2. Continue to the next day" << endl;
        cout << "3. Quit" << endl;
        cout << "Your action = ";
        cin >> action;

        if (action != 1 && action != 2 && action != 3)
            cerr << "Input out of bound!" << endl;
        cout << endl;

        if (action == 1){
            while (true)
            {
                printList(rooms);
                cout << "Patient's Type = " << active_list[patient_index % NUM_PATIENTS].type << endl;
                do{
                    cout << "Input room number = ";
                    cin >> selected_room;
                }
                while(selected_room <= 0 || selected_room > 50);

                cout << endl;
                if(checkRoomAvailability(rooms, active_list[patient_index % NUM_PATIENTS].type, selected_room)){
                    switch (active_list[patient_index % NUM_PATIENTS].type){
                        case 'S':
                            active_patient -= 1;
                            patient_index += 1;
                            break;
                        case 'C':
                            active_patient -= 2;
                            patient_index += 2;
                            break;
                    }
                    cout << "The room has been successfully occupied.\n" << endl;
                }

                //if ran out of patient
                if(active_patient == 0){
                    num_covid_patients[day_index % NUM_DAYS] = 0;
                    ++day_index;
                    update(rooms, MAX_ROOM);
                    int temp = transferringPatient(covid_list, active_list, fill_index, day_index);
                    active_patient += temp;
                    cumulative += temp;
                    if(cumulative > NUM_PATIENTS) 
                        printList(active_list, active_patient, patient_index % NUM_PATIENTS, fill_index % NUM_PATIENTS, true);
                    else
                        printList(active_list, active_patient, patient_index % NUM_PATIENTS, fill_index % NUM_PATIENTS, false);
                    day++;
                    break;
                }

                cout << "UPDATE: Number of covid = " << active_patient << endl;

                do{
                    cout << "Continue? (1. Yes/ 0. No) ";
                    cin >> action;
                }while(action > 1 || action < 0);
                
                cout << endl;

                if(action == 0){ 
                    break; 
                } 
            }
        }

        else if (action == 2){
            if ((fill_index + num_covid_patients[day_index + 1] - 1 > NUM_PATIENTS) && (fill_index + num_covid_patients[day_index + 1]) % NUM_PATIENTS > patient_index){
                cout << "There is not enough space for transferring the patient. Please assign some patient to the room first.\n" << endl;
                cout << "Incoming patient = " << num_covid_patients[day_index + 1] << " people" << endl;
            }
            else{
                num_covid_patients[day_index % NUM_DAYS] = 0;
                ++day_index;
                update(rooms, MAX_ROOM);
                int temp = transferringPatient(covid_list, active_list, fill_index, day_index);
                active_patient += temp;
                cumulative += temp;
                if(cumulative > NUM_PATIENTS + 1) 
                    printList(active_list, active_patient, patient_index % NUM_PATIENTS, fill_index % NUM_PATIENTS, true);
                else
                    printList(active_list, active_patient, patient_index % NUM_PATIENTS, fill_index % NUM_PATIENTS, false);
                day++;
            } 
        }

        else if (action == 3)
            running = false;
        else
            cerr << "Input out of bound!" << endl;

    }

    cout << "System shutting down..." << endl;
    
    return 0;
}