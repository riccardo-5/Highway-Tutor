#ifndef HIGHWAY_H
#define HIGHWAY_H
#include <vector>
class Highway
{
    public:
        const std::vector<int>& getJunctions() const { return junctions_; }
    private:
        std::vector<int> junctions_; // the int values represent the junction distance from the origin
};



#endif