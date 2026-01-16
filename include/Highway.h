#ifndef HIGHWAY_H
#define HIGHWAY_H
#include <vector>

struct Junction
{
    int id_;
    int distFromOrigin_; // in km
};

struct Passage
{
    int id_;
    int distFromOrigin_;
};

class Highway
{
    public:

        // Getters
        const std::vector<Junction>& getJunctions() const { return junctions_; }
        const std::vector<Passage>& getPassages() const { return passages_; }



    private:
        std::vector<Junction> junctions_; // the int values represent the junction distance from the origin
        std::vector<Passage> passages_;
};



#endif