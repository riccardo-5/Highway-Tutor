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

//std::sort(allPassages.begin(), allPassages.end(), [](const Passage& a, const Passage& b) {
//  return a.timestamp < b.timestamp;   //lambda definisce il criterio di ordinamentoper tempo       

void Tutor::setTime(double secondsToAdd) {
  if (secondsToAdd < 0) {
      std::cout << "Error." << std::endl;
      return;
  }
double targetTime = currentTime + secondsToAdd;

while (nextPassageIndex < allPAssage.size()) {
  const Passage& p = allPassages[nextPassageIndex];

if (p.timestamp > targetTime) {
    break;        //fermati se il passaggio è nel futuro rispetto alla simulazione
}

processPassage(p);
nextPassageIndex++;
  }
  currentTime = targetTime;
}

void Tutor::processPassage(const Passage& currentP) {
    varcoCounts[currentP.varcoId]++; //accediamo al varco usando l'Id

  auto it = activeVehicles.find(currentP.plate); //cerchiamo se la targa è gia passata

if (it!= activeVehicles.end()) { //se 'it' non è uguale a .end(),significa che abbiamo trovato un passaggio precedente per questa targa
  Passage previousP = it->second;






