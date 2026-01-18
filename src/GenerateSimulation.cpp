// RICCARDO TOSIN 2137380
#include "../include/Simulator.h"

int main() 
{
    try {
        Highway my_highway;
        my_highway.loadFromFile("../Data/Highway.txt");
        Simulator my_simulator(my_highway);
        my_simulator.generateRuns("../Data/Runs.txt");
        my_simulator.generatePassages("../Data/Passages.txt");
    } catch (const std::exception& e) {
        std::cerr << "errore nell'apertura dei file: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}