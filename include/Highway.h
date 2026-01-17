#ifndef HIGHWAY_H
#define HIGHWAY_H
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cmath>

struct Junction
{
    int id_;
    double distFromOrigin_; // in km
};

struct Passage
{
    int id_;
    double distFromOrigin_;
};

class Highway
{
    public:
        // Constructor
        Highway(const std::string& path);

        // Getters
        const std::vector<Junction>& getJunctions() const { return junctions_; }
        const std::vector<Passage>& getPassages() const { return passages_; }


        std::vector<int> getPassagesInBetween(int startJunctionId, int endJunctionId) const;



    private:
        std::vector<Junction> junctions_; // the int values represent the junction distance from the origin
        std::vector<Passage> passages_;
        void validate();
};



#endif