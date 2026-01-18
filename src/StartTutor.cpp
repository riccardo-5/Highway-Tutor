
#include <sstream>
#include "../include/Tutor.h"

int main() {
    Highway highway;
    highway.loadFromFile("../../Data/Highway.txt");
    Tutor tutor(highway);
    
    // Caricamento dei dati prodotti dal simulatore 
    tutor.loadPassages("../../Data/Passages.txt"); 

    std::string line;
    std::cout << "Sistema Tutor avviato. In attesa di comandi (set_time, stats, reset, exit)..." << std::endl;

    // Loop principale di attesa input dall'utente 
    while (std::cout << "> " && std::getline(std::cin, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string command;
        ss >> command;

        if (command == "set_time") { 
            std::string argument;
            if (ss >> argument) {
                double secondsToAdvance = 0;
                bool isMinutes = false;

                // Controllo se l'argomento è in minuti (es. 20m) 
                if (argument.back() == 'm') {
                    isMinutes = true;
                    argument.pop_back(); // Rimuove la 'm' per il parsing numerico
                }

                try {
                    secondsToAdvance = std::stod(argument);
                    if (isMinutes) {
                        secondsToAdvance *= 60.0; // Conversione in secondi 
                    }

                    // Avanzamento temporale e analisi transiti 
                    tutor.setTime(secondsToAdvance);
                } catch (...) {
                    std::cout << "Errore: Formato tempo non valido." << std::endl;
                }
            } else {
                std::cout << "Errore: Specificare un valore per set_time." << std::endl;
            }
        } 
        else if (command == "stats") { 
            // Stampa le statistiche complessive 
            tutor.printStats();
        } 
        else if (command == "reset") { 
            // Riporta il sistema allo stato iniziale 
            tutor.reset();
            std::cout << "Sistema resettato all'istante 0." << std::endl;
        } 
        else if (command == "exit") {
            break;
        } 
        else {
            std::cout << "Comando sconosciuto. Usa: set_time <valore>[m], stats, reset, exit." << std::endl;
        }
    }

    return 0;
}