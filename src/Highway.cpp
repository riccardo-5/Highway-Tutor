//Autore: Longo Domenico 2101783
//File: Highway.cpp (autostrada)
#include "Highway.h"

#include <cmath> //per abs
#include <fstream> //per ifstream
#include <sstream> //per istringstream
#include <stdexcept> //per runtime_error  
#include <algorithm> //per sort

//Carica l'autostrada dal file
void Highway::loadFromFile(const std::string& path) {

    //pulisco eventuali dati vecchi per non accumularli
    passages_.clear();
    junctions_.clear();

    std::ifstream file(path);//apro il file in lettura

    if (!file.is_open()) {
        throw std::runtime_error("Impossibile aprire il file: " + path);//gestione errore apertura file
    }

    std::vector<double> passageKm; //vettore temporaneo per ordinare i km dei varchi
    std::vector<double> junctionKm; //vettore temporaneo per ordinare i km degli svincoli

    std::string line;//stringa per leggere le righe

    //leggo il file riga per riga
    while (std::getline(file, line)) {
        if(line.size()==0) continue; //salto righe vuote

        //uso istringstream per leggere i dati dalla riga
        std::istringstream iss(line);
        double km;
        char type;

        //controllo formato riga
        if(!(iss >> km >> type)) {
            throw std::runtime_error("Formato riga non valido: " + line);//gestione errore formato riga
            }
        //controllo km negativo
        if(km < 0) {
            throw std::runtime_error("Valore km negativo: " + std::to_string(km));//gestione errore km negativo
            }
        //controllo tipo varco o svincolo
        if(type == 'V') {
            passageKm.push_back(km); //aggiungo il km al vettore temporaneo
            } 
        //controllo tipo svincolo invece di varco
        else if(type == 'S') {
            junctionKm.push_back(km); //aggiungo il km al vettore temporaneo
            }
        //controllo tipo non valido
        else {
            throw std::runtime_error("Tipo non valido: " + std::string(1, type));//gestione errore tipo non valido
            }
        }

    //Ordino per km crescente
    std::sort(passageKm.begin(), passageKm.end());
    std::sort(junctionKm.begin(), junctionKm.end());

    //Controllo che ci siano almeno 2 varchi
    if(passageKm.size()<2){
        throw std::runtime_error("Devono essere presenti almeno 2 varchi (V).");
        }
    
    //Controllo che deve esserci almeno 1 svincolo prima del primo varco
    if(junctionKm.empty()){
        throw std::runtime_error("Devono essere presenti almeno 2 svincoli (S) prima del primo varco.");
        }
    
    //Controllo che deve esserci almeno 1 svincolo dopo l'ultimo varco
    double firstV= passageKm.front();//primo varco (km minimo
    double lastV= passageKm.back(); //ultimo varco (km massimo)
    bool foundBefore=false; //flag svincolo prima del primo varco
    bool foundAfter=false;  //flag svincolo dopo l'ultimo varco
    for(double sKm : junctionKm){
        if(sKm < firstV){
            foundBefore=true;
            }
        if(sKm > lastV){
            foundAfter=true;
            }
        }
    if(!foundBefore){
        throw std::runtime_error("Deve essere presente almeno uno svincolo (S) prima del primo varco.");
        }
    if(!foundAfter){
        throw std::runtime_error("Deve essere presente almeno uno svincolo (S) dopo l'ultimo varco.");
        }

    //Controllo che ci sia la distanza minima di 1 km tra varchi e svincoli
    for(double sKm : junctionKm){
        for(double vKm : passageKm){
            double dist=std::abs(sKm - vKm);
            if(dist < 1.0){
                throw std::runtime_error("La distanza minima tra varchi e svincoli deve essere di almeno 1 km.");
                }
            }
        }
    
    //Controllo che non ci siano duplicati nello stesso tipo
    for(int i=1; i < (int)passageKm.size(); i++){
        if(passageKm[i] == passageKm[i-1]){
            throw std::runtime_error("Due varchi (V) con la stessa distanza: " + std::to_string(passageKm[i]));
            }
        }
    for(int i=1; i < (int)junctionKm.size(); i++){
        if(junctionKm[i] == junctionKm[i-1]){
            throw std::runtime_error("Due svincoli (S) con la stessa distanza: " + std::to_string(junctionKm[i]));
            }
        }

    //Creo i varchi con id progressivi
    for(int i = 0; i < (int)passageKm.size(); i++){
        passages_.push_back(Passage(i+1, passageKm[i]));
        }
    //Creo gli svincoli con id progressivi
    for(int i = 0; i < (int)junctionKm.size(); i++){
        junctions_.push_back(Junction(i+1, junctionKm[i]));
        }
    }

//Restituisce la distanza in km di uno svincolo
double Highway::getJunctionKm(int junctionID) const {
    //junctionID  parte da 1, l'indice da 0
    return junctions_.at(junctionID - 1).getKm();
    }

//Restituisce la distanza in km di un varco
double Highway::getPassageKm(int passageID) const {
    //passageID parte da 1, l'indice da 0
    return passages_.at(passageID - 1).getKm();
    }

//Restituisce gli id dei varchi tra due svincoli
std::vector<int> Highway::getPassagesInBetween(int entryJunctionID, int exitJunctionID) const {

    double entryKm = getJunctionKm(entryJunctionID);//km dello svincolo di entrata
    double exitKm = getJunctionKm(exitJunctionID);//km dello svincolo di uscita
    
    //controllo che lo svincolo di uscita sia dopo quello di entrata
    if(exitKm <= entryKm) {
        throw std::runtime_error("Lo svincolo di uscita deve essere dopo quello di entrata.");
        }
    
    //cerco i varchi tra i due svincoli
    std::vector<int> result;

    //scorro tutti i varchi
    for(const Passage& p : passages_) {
        double pk=p.getKm();//km del varco
        //se il varco è tra i due svincoli lo aggiungo al risultato
        if(pk > entryKm && pk < exitKm) {
            result.push_back(p.getId());
            }
        }
    return result;
}