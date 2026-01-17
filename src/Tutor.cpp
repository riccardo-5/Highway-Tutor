#include "../include/Tutor.h"
#include <fstream>
#include <iostream>     
#include <iomanip>      
#include <algorithm>

Tutor::Tutor(const Highway& h) : highway(h), currentTime(0.0), nextPassageIndex(0),  totalSanctions(0), totalSpeedSum(0.0), speedMeasurementsCount(0) {} //inizializzazione membri

void Tutor::loadPassages(const std::string& filename) {    //Opening a file. If this fails, we handle the error by printing a message.
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open " <<filename << std::endl;
    return;
  }

int vID;
std::string plate;
double time;

while (file >> vID >> plate >> time) {     //Reading the file line by line
    allPassages.push_back({vId, plate, time}); //push_back adds elements to the end of the vector, dynamically resizing it
}
file.close();

std::sort(allPassages.begin(), allPassages.end(), [](const Passage& a, const Passage& b) {return a.timestamp < b.timestamp;}); //lamda eexpression to handle the 'allPassages' vector
}
void Tutor::setTime(double secondsToAdd) { 
    if (secondsToAdd < 0) {                   //We verify that the user does not enter negative times
        std::cout << "Errore." << std::endl;
        return;
    }

    double targetTime = currentTime + secondsToAdd;

    while (nextPassageIndex < allPassages.size()) {
        const Passage& p = allPassages[nextPassageIndex];
        
        if (p.timestamp > targetTime) {
            break;                  // If the transition time is beyond the time requested by the user, we stop.
        }

        processPassage(p);
        nextPassageIndex++;
    }

    currentTime = targetTime;
}
void Tutor::processPassage(const Passage& currentP) {
    varcoCounts[currentP.varcoId]++; 


    auto it = activeVehicles.find(currentP.plate); //vehicle tracking
    

    if (it != activeVehicles.end()) {  // Check if the license plate is already present
        Passage previousP = it->second;
        double distKm = highway.getDistance(previousP.varcoId, currentP.varcoId);// The Tutor asks the Highway class the distance
        if (distKm > 0) {
            
            double hours = (currentP.timestamp - previousP.timestamp) / 3600.0;
            double speed = distKm / hours; //Calculate the average speed (v = s/t).
            totalSpeedSum += speed;
            speedMeasurementsCount++;


            if (speed > 130.0) {    //Check the speed limit and print the fine.
                totalSanctions++;

                std::cout << "[PENALTY] License Plate: " << currentP.plate 
                          << " | Route: Gate " << previousP.varcoId << " -> " << currentP.varcoId
                          << " | Speed: " << std::fixed << std::setprecision(2) << speed << " km/h"
                          << std::endl;
            }
        }
    }

    
    activeVehicles[currentP.plate] = currentP; // We are updating the vehicle's last known location.
}

void Tutor::printStats() const {
    std::cout << "\n=== SYSTEM STATISTICS ===" << std::endl;
    
    
    for (auto const& pair : varcoCounts)      //Let's scroll through the map of the gates.
        double minutes = (currentTime > 0) ? currentTime / 60.0 : 0.0;
        double rate = (minutes > 0) ? pair.second / minutes : 0.0;
        
        std::cout << "Varco " << pair.first << ": " << pair.second << " transits (" << std::setprecision(2) << rate << " vehicles/min)" << std::endl;
    }

    double avgSpeed = (speedMeasurementsCount > 0) ? totalSpeedSum / speedMeasurementsCount : 0.0; 
    std::cout << "Average speed detected: " << avgSpeed << " km/h" << std::endl;
    std::cout << "Total fines issued: " << totalSanctions << std::endl;
}
