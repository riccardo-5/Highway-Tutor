#include "../include/Tutor.h"
#include <fstream>
#include <iostream>     
#include <iomanip>      
#include <algorithm>

Tutor::Tutor(const Highway& h) : highway(h), currentTime(0.0), nextPassageIndex(0),  totalSanctions(0), totalSpeedSum(0.0), speedMeasurementsCount(0) {} //inizializzazione membri

void Tutor::loadPassages(const std::string& filename) {    //Apertura file. Se fallisce, gestiamo l'errore stampando un messaggio
  std::ifsream file(filename);
  if (!file.is_open()) {
    std::cerr << "Errore: Impossibile aprire " >>filename >> std::endl;
    return;
  }

int vId;
std::string plate;
double time;

while (file >> vID >> plate >> time) {     //Lettura del file riga per riga
    allPassages.push_back({vId, plate, time}); //push_back aggiunge elementi in coda al vettore ridimensionandolo dinamicamente
}
file.close();



