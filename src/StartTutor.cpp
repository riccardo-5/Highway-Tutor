// FEDERICO BATTISTELLA
#include <sstream>
#include "../include/Tutor.h"

int main() {
    Highway highway;
    highway.loadFromFile("../Data/Highway.txt");
    Tutor tutor(highway);
    
    tutor.loadPassages("../Data/Passages.txt"); 

    std::string line;
    std::cout << "Sistema Tutor avviato. In attesa di comandi (set_time, stats, reset, exit)..." << std::endl;

    // main loop waiting for user input 
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

                // Check if argument is in minutes (es. 20m) 
                if (argument.back() == 'm') {
                    isMinutes = true;
                    argument.pop_back(); // remove the 'm'
                }

                try {
                    secondsToAdvance = std::stod(argument);
                    if (isMinutes) {
                        secondsToAdvance *= 60.0; // converts in seconds
                    }
                    // go ahead in time
                    tutor.setTime(secondsToAdvance);
                } catch (...) {
                    std::cout << "Errore: Formato tempo non valido" << std::endl;
                }
            } else {
                std::cout << "Errore: Specificare un valore per set_time" << std::endl;
            }
        } 
        else if (command == "stats") { 
            tutor.printStats();
        } 
        else if (command == "reset") { 
            tutor.reset();
            std::cout << "Sistema resettato all'istante 0" << std::endl;
        } 
        else if (command == "exit") {
            break;
        } 
        else {
            std::cout << "Comando sconosciuto. Usa: set_time <valore>, stats, reset, exit." << std::endl;
        }
    }

    return 0;
}