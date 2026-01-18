//LONGO DOMENICO 2101783
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

    // EXTRA:
    // Considero "vuote" anche righe fatte solo di spazi/tab.
    // Così il parser non esplode su file con formattazione umana.
    std::istringstream iss(line);
    iss >> std::ws;//consumo eventuali whitespace iniziali
    if (iss.eof()) continue;//riga vuota (o solo spazi): la salto

    double km;
    char type;


    // Parsing esplicito e controllato:
    // se non riesco a leggere km e tipo, il file non è conforme.
    if(!(iss >> km >> type)){
        throw std::runtime_error("Formato riga non valido: " + line);
       }
    if (km < 0) {
        throw std::runtime_error("Valore km negativo: " + std::to_string(km));
      }
    if(type == 'V'){
        passageKm.push_back(km);
       } 
    else if(type == 'S'){
        junctionKm.push_back(km);
       } 
    else{
        throw std::runtime_error("Tipo non valido: " + std::string(1, type));
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

    // EXTRA:
    // Controllo distanza minima (>= 1 km) tra SVINCOLI e VARCHI
    // in modo efficiente (O(N log M)) anziche' O(N*M) grazie alle liste ordinate.
    //
    // Idea: per ogni varco, mi basta controllare lo svincolo più vicino
    // (quello trovato da lower_bound) e quello immediatamente precedente.
    // Se nessuno dei due è troppo vicino, allora nessuno lo sarà.
    for (double vKm : passageKm) {

        // it punta al primo svincolo con km >= vKm
        auto it = std::lower_bound(junctionKm.begin(), junctionKm.end(), vKm);

        // Controllo lo svincolo "a destra" (se esiste)
        if (it != junctionKm.end()) {
            if (std::abs(*it - vKm) < 1.0) {
                throw std::runtime_error("Distanza < 1 km tra varco e svincolo (troppo vicini).");
            }
        }

        // Controllo lo svincolo "a sinistra" (se esiste)
        if (it != junctionKm.begin()) {
            auto itPrev = std::prev(it);
            if (std::abs(*itPrev - vKm) < 1.0) {
                throw std::runtime_error("Distanza < 1 km tra varco e svincolo (troppo vicini).");
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

    // Validazione ID (piu' chiaro di un out_of_range)
    if (entryJunctionID < 1 || entryJunctionID > (int)junctions_.size() ||
        exitJunctionID  < 1 || exitJunctionID  > (int)junctions_.size()) {
        throw std::runtime_error("ID svincolo fuori range in getPassagesInBetween().");
    }
    // Ottengo i km degli svincoli di ingresso e uscita
    double entryKm = getJunctionKm(entryJunctionID);
    double exitKm  = getJunctionKm(exitJunctionID);
    // Stesso svincolo -> nessun varco attraversato
    if (entryKm == exitKm) return {};
    const double lo = std::min(entryKm, exitKm);
    const double hi = std::max(entryKm, exitKm);
    const bool forward = (entryKm < exitKm);
    std::vector<int> result;
    // passages_ e' ordinato per km crescente -> uso ricerca binaria
    auto itBegin = std::upper_bound(
        passages_.begin(), passages_.end(), lo,
        [](double value, const Passage& p){ return value < p.getKm(); }
    ); // primo con km > lo

    auto itEnd = std::lower_bound(
        passages_.begin(), passages_.end(), hi,
        [](const Passage& p, double value){ return p.getKm() < value; }
    ); // primo con km >= hi

    if (forward) {
        for (auto it = itBegin; it != itEnd; ++it) {
            result.push_back(it->getId());
        }
    } else {
        for (auto it = std::make_reverse_iterator(itEnd);
             it != std::make_reverse_iterator(itBegin); ++it) {
            result.push_back(it->getId());
        }
    }
    return result;
}

//EXTRA: restituisce gli id degli svincoli tra due varchi (inclusi gli estremi)
std::vector<int> Highway::getJunctionsInBetween(int entryPassageId, int exitPassageId) const {

    // Validazione ID
    if (entryPassageId < 1 || entryPassageId > (int)passages_.size() ||
        exitPassageId  < 1 || exitPassageId  > (int)passages_.size()) {
        throw std::runtime_error("ID varco fuori range in getJunctionsInBetween().");
    }

    double entryKm = getPassageKm(entryPassageId);
    double exitKm  = getPassageKm(exitPassageId);

    // Stesso varco -> nessuno svincolo in mezzo
    if (entryKm == exitKm) return {};

    const double lo = std::min(entryKm, exitKm);
    const double hi = std::max(entryKm, exitKm);
    const bool forward = (entryKm < exitKm);

    std::vector<int> result;

    // junctions_ e' ordinato per km crescente -> uso ricerca binaria
    auto itBegin = std::upper_bound(
        junctions_.begin(), junctions_.end(), lo,
        [](double value, const Junction& j){ return value < j.getKm(); }
    ); // primo con km > lo

    auto itEnd = std::lower_bound(
        junctions_.begin(), junctions_.end(), hi,
        [](const Junction& j, double value){ return j.getKm() < value; }
    ); // primo con km >= hi

    if (forward) {
        for (auto it = itBegin; it != itEnd; ++it) {
            result.push_back(it->getId());
        }
    } else {
        for (auto it = std::make_reverse_iterator(itEnd);
             it != std::make_reverse_iterator(itBegin); ++it) {
            result.push_back(it->getId());
        }
    }

    return result;
}

double Highway::getDistance(int passageId1, int passageId2) const {
    // Safety check
    if (passageId1 < 1 || passageId1 > static_cast<int>(passages_.size()) ||
        passageId2 < 1 || passageId2 > static_cast<int>(passages_.size())) {
        std::cerr << "Errore: ID varco non valido (" << passageId1 << " o " << passageId2 << ")" << std::endl;
        return 0.0;
    }

    // We use passageId - 1 because IDs start at 1 but vector indices start at 0
    double d1 = passages_[passageId1 - 1].getKm();
    double d2 = passages_[passageId2 - 1].getKm();

    return std::abs(d1 - d2);
}