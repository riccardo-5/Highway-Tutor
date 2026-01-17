#ifndef TUTOR_H
#define TUTOR_H
#include "Highway.h"
#include <unordered_map>
#include <map>


struct Passage {  //group data
    int varcoId;
    std::string plate;
    double timestamp; 
};

class Tutor {
public:
    Tutor(const Highway& h); //constructor


    void loadPassages(const std::string& filename);  //loading data into file.
    void setTime(double secondsToAdd); //time

    void printStats() const;
    void reset();

private:

    const Highway& highway;
    double currentTime;


    std::vector<Passage> allPassages;    // Vector to store all the steps read from the file.
    size_t nextPassageIndex;     // Automatically manages memory

    std::unordered_map<std::string, Passage> activeVehicles;     // Associate License Plate -> Last Pass.
  
    std::map<int, int> varcoCounts;     //Map sorted by statistics (VarcoID -> Count).
    long totalSanctions;
    double totalSpeedSum;
    long speedMeasurementsCount;

    void processPassage(const Passage& p);
};

#endif
