#ifndef PASSAGE_H
#define PASSAGE_H
//Autore: Longo Domenico 2101783
//File: Passage.h (varco)
class Passage {
private:
    int id_; //identificativo univoco del varco
    double km_; //distanza in km dal punto di partenza
public:
    Passage(int id, double km) : id_(id), km_(km) {} //costruttore
    int getId() const { return id_; } //restituisce l'id del varco
    double getKm() const { return km_; } //restituisce la distanza in km
};

#endif