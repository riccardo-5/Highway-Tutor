#include "../include/Simulator.h"

int main() 
{
    Highway my_highway;
    my_highway.loadFromFile("Data/Highway.txt");
    Simulator my_simulator(my_highway);
    my_simulator.generateRuns();
    my_simulator.generatePassages();
}