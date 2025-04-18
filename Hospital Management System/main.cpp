#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <ctime>
using namespace std;

#define reset "\033[0m"
#define red "\033[1;31m"
#define green "\033[1;32m"
#define yellow "\033[1;33m"
#define blue "\033[1;34m"
#define magenta "\033[1;35m"
#define cyan "\033[1;36m"
#define white "\033[1;37m"

void addPatient() {
    int id, age, phone;
    char gender;
    string name, address, admitDate, disease, bloodGroup, doctor;

    cout << "Enter new patient ID - ";
    cin >> id;
    cin.ignore();
    cout << "Enter the patient Name - ";
    getline(cin, name);
    cout << "Enter the disease patient has - ";
    getline(cin, disease);
    cout << "Enter the patient's age - ";
    cin >> age;
    cout << "Enter the patient's gender (M/F/O) - ";
    cin >> gender;
    cout << "Enter the patient's phone number - ";
    cin >> phone;
    cin.ignore();
    cout << "Enter the patient's address - ";
    getline(cin, address);
    cout << "Enter the admit date (DD/MM/YYYY) - ";
    getline(cin, admitDate);
    cout << "Enter the blood group - ";
    getline(cin, bloodGroup);
    cout << "Enter the name of doctor assigned - ";
    getline(cin, doctor);

    ofstream file("patient.txt", ios::app);
    if (!file) {
        cout << "Fatal Error 404. Try again.\n";
    } else {
        file << "---" << endl;
        file << "ID: " << id << endl;
        file << "Name: " << name << endl;
        file << "Age: " << age << endl;
        file << "Gender: " << gender << endl;
        file << "Disease: " << disease << endl;
        file << "Address: " << address << endl;
        file << "Blood Group: " << bloodGroup << endl;
        file << "Admit Date: " << admitDate << endl;
        file << "Phone: " << phone << endl;
        file << "Doctor: " << doctor << endl;
        file << "---" << endl;
        file.close();
        cout << "✅ Patient record saved successfully!\n";
    }
}

void showPatientRec() {
    cout << " ==== All Patient Records ==== \n";
    ifstream file("patient.txt");
    if (!file) {
        cout << "File can't be opened or doesn't exist!" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }
    file.close();
}

void searchPatientId() {
    string searchkey;
    cout << "Enter the patient ID or Name: ";
    cin >> searchkey;

    ifstream file("patient.txt");
    string line;
    bool found = false;
    string patientRecord = "";  // To hold the whole patient's record

    while (getline(file, line)) {
        // Accumulate lines in patientRecord
        patientRecord += line + "\n";

        // Check if any line contains the search key (ID or Name)
        if (line.find(searchkey) != string::npos) {
            found = true;
        }

        // If we reach the end of a patient's record (block ends with "---")
        if (line == "---") {
            if (found) {
                cout << "==== Patient Found ====\n";
                cout << patientRecord << endl;  // Print the entire patient's record
            }
            patientRecord = "";  // Reset the record for the next patient
            found = false;  // Reset the found flag
        }
    }

    if (found) {
        cout << "❌ No matching patient record found.\n";
    }

    file.close();
}

void deleteRecord() {
    string dltp;
    cout << "Enter the ID or Name you want to delete: ";
    cin.ignore();
    getline(cin, dltp);

    ifstream inputFile("patient.txt");
    ofstream tempFile("temp.txt");

    string line;
    bool skip = false, found = false;

    while (getline(inputFile, line)) {
        if (line.find("ID: ") != string::npos && line.find(dltp) != string::npos) {
            skip = true;
            found = true;
        }
        if (!skip) tempFile << line << endl;
        if (line == "---") skip = false;
    }

    inputFile.close();
    tempFile.close();

    remove("patient.txt");
    rename("temp.txt", "patient.txt");

    if (found)
        cout << "✅ Patient record deleted successfully.\n";
    else
        cout << "❌ No patient found with ID or Name: " << dltp << ".\n";
}

void updateDetails() {
    string idToUpdate;
    cout << "Enter the Patient ID to update: ";
    cin >> idToUpdate;
    cin.ignore();

    ifstream inputFile("patient.txt");
    ofstream tempFile("temp.txt");

    string line, block = "", newName, newDisease;
    bool insideBlock = false, found = false;

    while (getline(inputFile, line)) {
        if (line == "---") {
            if (insideBlock) {
                if (block.find("ID: " + idToUpdate) != string::npos) {
                    found = true;
                    cout << "Enter new name: ";
                    getline(cin, newName);
                    cout << "Enter new disease: ";
                    getline(cin, newDisease);

                    size_t namePos = block.find("Name: ");
                    size_t diseasePos = block.find("Disease: ");

                    if (namePos != string::npos) block.replace(namePos, block.find("\n", namePos) - namePos, "Name: " + newName);
                    if (diseasePos != string::npos) block.replace(diseasePos, block.find("\n", diseasePos) - diseasePos, "Disease: " + newDisease);
                }
                tempFile << block << "---" << endl;
                block = "";
                insideBlock = false;
            } else {
                insideBlock = true;
                block = "---\n";
            }
        } else if (insideBlock) {
            block += line + "\n";
        }
    }

    inputFile.close();
    tempFile.close();

    remove("patient.txt");
    rename("temp.txt", "patient.txt");

    if (found)
        cout << "✅ Patient record updated successfully.\n";
    else
        cout << "❌ No record found with ID " << idToUpdate << ".\n";
}

void doctorAssign() {
    int ID;
    cout << "Enter patient ID to assign doctor: ";
    cin >> ID;
    cin.ignore();

    string docname;
    cout << "Enter the Doctor Name to assign: ";
    getline(cin, docname);

    ifstream file("patient.txt");
    ofstream tempFile("temp.txt");

    string line, block = "";
    bool insideBlock = false, found = false;

    while (getline(file, line)) {
        if (line == "---") {
            if (insideBlock) {
                if (block.find("ID: " + to_string(ID)) != string::npos) {
                    found = true;
                    size_t doctorPos = block.find("Doctor: ");
                    if (doctorPos != string::npos) {
                        block.replace(doctorPos, block.find("\n", doctorPos) - doctorPos, "Doctor: " + docname);
                    }
                }
                tempFile << block << "---" << endl;
                block = "";
                insideBlock = false;
            } else {
                insideBlock = true;
                block = "---\n";
            }
        } else if (insideBlock) {
            block += line + "\n";
        }
    }

    file.close();
    tempFile.close();

    remove("patient.txt");
    rename("temp.txt", "patient.txt");

    if (found)
        cout << "✅ Doctor assigned successfully!\n";
    else
        cout << "❌ Patient ID not found!\n";
}

void patientByDisease() {
    string disease;
    cout << "Enter the Disease Name: ";
    cin.ignore();
    getline(cin, disease);

    ifstream file("patient.txt");
    string line;
    bool found = false;

    cout << "\n==== Patients with Disease: " << disease << " ====\n";
    while (getline(file, line)) {
        if (line.find("Disease: " + disease) != string::npos) {
            found = true;
            cout << line << endl;
        }
    }

    if (!found) {
        cout << "No matching patient record found.\n";
    }

    file.close();
}

void patientByDoc() {
    string doc;
    cout << "Enter the Doctor Name: ";
    cin.ignore();
    getline(cin, doc);

    ifstream file("patient.txt");
    string line;
    bool found = false;

    cout << "\n==== Patients Assigned to Dr. " << doc << " ====\n";
    while (getline(file, line)) {
        if (line.find("Doctor: " + doc) != string::npos) {
            found = true;
            cout << line << endl;
        }
    }

    if (!found) {
        cout << "No matching patient record found.\n";
    }

    file.close();
}

void emergencyExit() {
    int id, age;
    char gender;
    string name, bloodGroup, disease;
    string emergencyLevel = "EMERGENCY 🚨";

    time_t t = time(0);
    struct tm *now = localtime(&t);
    string admitDate = to_string(now->tm_mday) + "/" + to_string(now->tm_mon + 1) + "/" + to_string(now->tm_year + 1900);

    cout << "Enter ID: ";
    cin >> id;
    cout << "Enter age: ";
    cin >> age;
    cout << "Gender (M/F): ";
    cin >> gender;
    cin.ignore();
    cout << "Enter Name: ";
    getline(cin, name);
    cout << "Enter Blood Group: ";
    getline(cin, bloodGroup);
    cout << "Enter Disease: ";
    getline(cin, disease);
    cout << "Admit Date: " << admitDate << endl;

    ofstream file("emergency.txt", ios::app);
    if (!file) {
        cout << "Fatal Error 404. Try again.\n";
    } else {
        file << "---- EMERGENCY PATIENT ----\n";
        file << "ID - " << id << endl;
        file << "Age - " << age << endl;
        file << "Gender - " << gender << endl;
        file << "Name - " << name << endl;
        file << "Blood Group - " << bloodGroup << endl;
        file << "Disease - " << disease << endl;
        file << "Admit Date - " << admitDate << endl;
        file << "-----------------------------\n";
        file.close();
        cout << "✅ Emergency record saved successfully!\n";
    }
}

int main() {
    int choice = 0;
    system("clear");
    cout << red << "==================================================\n" << reset;
    cout << green << "+       HOSPITAL MANAGEMENT SYSTEM - BY RUDRA    +\n" << reset;
    cout << red << "==================================================\n" << reset;
    cout << white << "Welcome to the hospital server\n" << reset;

    while (choice != 10) {
        cout << "\n1. Add New Patient Record\n";
        cout << "2. Show All Patient Records\n";
        cout << "3. Search Patient by ID or Name\n";
        cout << "4. Delete Patient Record\n";
        cout << "5. Update Patient Details\n";
        cout << "6. Assign Doctor to Patient\n";
        cout << "7. Show Patients by Disease\n";
        cout << "8. Show Patients by Doctor\n";
        cout << "9. Emergency Entry\n";
        cout << "10. Exit the system\n";
        cout << red << "==================================================\n" << reset;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addPatient(); break;
            case 2: showPatientRec(); break;
            case 3: searchPatientId(); break;
            case 4: deleteRecord(); break;
            case 5: updateDetails(); break;
            case 6: doctorAssign(); break;
            case 7: patientByDisease(); break;
            case 8: patientByDoc(); break;
            case 9: emergencyExit(); break;
            case 10: cout << "👋 Exiting from Server. Bye bye!\n"; return 0;
            default: cout << "Invalid choice. Try again.\n"; break;
        }
    }

    return 0;
}
