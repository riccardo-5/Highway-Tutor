//Autore: Longo Domenico 2101783
#ifndef HIGHWAY_H
#define HIGHWAY_H 

//File: Highway.h (autostrada)
#include <vector>
#include <string>

#include "Junction.h"
#include "Passage.h"

class Highway {
private:
    std::vector<Passage> passages_; //vettore dei varchi
    std::vector<Junction> junctions_; //vettore degli svincoli
public:

    void loadFromFile(const std::string& path); //carica l'autostrada dal file

    const std::vector<Passage>& getPassages() const { return passages_; } //restituisce i varchi
    const std::vector<Junction>& getJunctions() const { return junctions_; } //restituisce gli svincoli

    std::vector<int> getPassagesInBetween(int entryJunctionId, int exitJunctionId) const; //restituisce gli id dei varchi tra due svincoli
    std::vector<int> getJunctionsInBetween(int entryPassageId, int exitPassageId) const; //restituisce gli id degli svincoli tra due varchi

    double getJunctionKm(int junctionId) const; //restituisce la distanza in km di uno svincolo
    double getPassageKm(int passageId) const; //restituisce la distanza in km di un varco
};

#endif