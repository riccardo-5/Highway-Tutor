#include "../include/Tutor.h"
#include <fstream>
#include <iostream>     
#include <iomanip>      
#include <algorithm>

Tutor::Tutor(const Highway& h) : highway(h), currentTime(0.0), nextPassageIndex(0),  totalSanctions(0), totalSpeedSum(0.0), speedMeasurementsCount(0) {} //inizializzazione membri

void Tutor::loadPassages(const std::string& filename) {    //Apertura file. Se fallisce, gestiamo l'errore stampando un messaggio
  std::ifsream file(filename);
  if (!file.is_open()) {
    std::cerr << "Errore: Impossibile aprire " <<filename << std::endl;
    return;
  }

int vId;
std::string plate;
double time;

while (file >> vID >> plate >> time) {     //Lettura del file riga per riga
    allPassages.push_back({vId, plate, time}); //push_back aggiunge elementi in coda al vettore ridimensionandolo dinamicamente
}
file.close();

//std::sort(allPassages.begin(), allPassages.end(), //definire  l'ordine di comparazione
 //       [](const Passage& a, const Passage& b) {
 //           return a.timestamp < b.timestamp;
 //   });
}
void Tutor::setTime(double secondsToAdd) { 
    if (secondsToAdd < 0) {                   //Verifichiamo che l'utente non inserisca tempi negativi
        std::cout << "Errore." << std::endl;
        return;
    }

    double targetTime = currentTime + secondsToAdd;

    while (nextPassageIndex < allPassages.size()) {
        const Passage& p = allPassages[nextPassageIndex];
        
        if (p.timestamp > targetTime) {
            break;                  // Se l'istante di passaggio è oltre il tempo richiesto dall'utente, ci fermiamo.
        }

        processPassage(p);
        nextPassageIndex++;
    }

    currentTime = targetTime;
}
void Tutor::processPassage(const Passage& currentP) {
    varcoCounts[currentP.varcoId]++; 


    auto it = activeVehicles.find(currentP.plate); //tracciamento veicoli
    

    if (it != activeVehicles.end()) {  // Controllo se la targa è già presente 
        Passage previousP = it->second;
        double distKm = highway.getDistance(previousP.varcoId, currentP.varcoId);// Il Tutor chiede alla classe Highway la distanza

        if (distKm > 0) {
            
            double hours = (currentP.timestamp - previousP.timestamp) / 3600.0;
            double speed = distKm / hours; // - Calcolo della velocità media (v = s/t).

            totalSpeedSum += speed;
            speedMeasurementsCount++;


            if (speed > 130.0) {    // - Verifica limite di velocità e stampa sanzione.
                totalSanctions++;

                std::cout << "[SANZIONE] Targa: " << currentP.plate 
                          << " | Tratta: Varco " << previousP.varcoId << " -> " << currentP.varcoId
                          << " | Vel: " << std::fixed << std::setprecision(2) << speed << " km/h"
                          << std::endl;
            }
        }
    }

    
    activeVehicles[currentP.plate] = currentP; // Aggiorniamo l'ultima posizione nota del veicolo.
}

void Tutor::printStats() const {
    std::cout << "\n=== STATISTICHE DEL SISTEMA ===" << std::endl;
    
    
    for (auto const& pair : varcoCounts) {     //scorriamo la mappa dei varchi.
        double minutes = (currentTime > 0) ? currentTime / 60.0 : 0.0;
        double rate = (minutes > 0) ? pair.second / minutes : 0.0;
        
        std::cout << "Varco " << pair.first << ": " << pair.second << " transiti (" 
                  << std::setprecision(2) << rate << " veicoli/min)" << std::endl;
    }

    double avgSpeed = (speedMeasurementsCount > 0) ? totalSpeedSum / speedMeasurementsCount : 0.0; 
    std::cout << "Velocità media rilevata: " << avgSpeed << " km/h" << std::endl;
    std::cout << "Totale sanzioni emesse: " << totalSanctions << std::endl;
}

void Tutor::reset() {  // - Svuotamento dei contenitori.
   
    currentTime = 0.0;
    nextPassageIndex = 0;
    activeVehicles.clear();
    varcoCounts.clear();
    totalSanctions = 0;
    totalSpeedSum = 0.0;
    speedMeasurementsCount = 0;
    std::cout << "Sistema resettato." << std::endl;
}










