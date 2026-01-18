// RICCARDO TOSIN 2137380
#include "../include/Simulator.h"

int main() 
{
    try {
        Highway my_highway;
        my_highway.loadFromFile("../Data/Highway_valid_unsorted.txt"); // modify the path to load a different Highway (must modify the path in StartTutor too!!)
        Simulator my_simulator(my_highway); // create the simulation
        my_simulator.generateRuns("../Data/Runs.txt"); // write on files
        my_simulator.generatePassages("../Data/Passages.txt");
    } catch (const std::exception& e) {
        std::cerr << "errore nell'apertura dei file: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}