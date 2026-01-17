#include "../include/Simulator.h"

int main() 
{
    Highway my_highway("Data/Highway1.txt");
    Simulator my_simulator(my_highway);
    my_simulator.generateRuns();
    my_simulator.generatePassages();
}