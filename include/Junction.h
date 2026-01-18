//Autore: Longo Domenico 2101783
#ifndef JUNCTION_H
#define JUNCTION_H

class Junction {
private:
    int id_; //identificativo univoco dello svincolo
    double km_; //distanza in km dal punto di partenza  
public:
    Junction(int id, double km) : id_(id), km_(km) {} //costruttore
    int getId() const { return id_; } //restituisce l'id dello svincolo
    double getKm() const { return km_; } //restituisce la distanza in km
};

#endif