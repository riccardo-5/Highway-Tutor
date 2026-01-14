#ifndef TUTOR_H
#define TUTOR_H
#include "Highway.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <map>


struct Passage {  //raggruppare dati
    int varcoId;
    std::string plate;
    double timestamp; 
};

class Tutor {
public:
    Tutor(const Highway& h); //costruttore


    void loadPassages(const std::string& filename);  // Passaggio di stringhe per const reference per evitare copie inutili.
    void setTime(double secondsToAdd); //time

    void printStats() const;
    void reset();

private:

    const Highway& highway;
    double currentTime;


    std::vector<Passage> allPassages;    // Vettore per memorizzare tutti i passaggi letti dal file.
    size_t nextPassageIndex;     // Gestisce automaticamente la memoria

    std::unordered_map<std::string, Passage> activeVehicles;     // associare Targa -> Ultimo Passaggio.
  
    std::map<int, int> varcoCounts;     // Map ordinata per le statistiche (VarcoID -> Conteggio).

    long totalSanctions;
    double totalSpeedSum;
    long speedMeasurementsCount;

    void processPassage(const Passage& p);
};

#endif