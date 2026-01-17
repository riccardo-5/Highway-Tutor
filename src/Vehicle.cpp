// RICCARDO TOSIN 2137380
#include "../include/Vehicle.h"

// Constructor
Vehicle::Vehicle(double entryTime, int entryJunctionId, int exitJunctionId, std::vector<Interval> velProfile) 
    : licensePlate_{generateLicensePlate()}, 
      entryJunctionId_{entryJunctionId}, 
      exitJunctionId_{exitJunctionId}, 
      entryTime_{entryTime}, 
      velocityProfile_{velProfile} {}


std::string Vehicle::profileToString() const
{
    std::string s;
    for (size_t i = 0; i < velocityProfile_.size(); ++i) // for each Interval
    {
        double time = velocityProfile_[i].end_ - velocityProfile_[i].start_; // calculate t_i (how much time is the Interval) (in seconds)

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << velocityProfile_[i].v_ << " " << time;
        s += oss.str();

        if (i != velocityProfile_.size() - 1) s += ", ";
    }
    return s;
}

// --------- PRIVATE HELPER FUNCTIONS
std::string Vehicle::generateLicensePlate() const
{
    std::string licensePlate;
    for (int i = 0; i < 2; i++) // first 2 letters
    {
        char letter = 'A' + rand() % 26; // generate a random letter between A and Z (0-25)
        licensePlate += letter;
    }
    for (int i = 0; i < 3; i++) // middle 3 numbers
    {
        char number = '0' + rand() % 10; // generate a random number between 0 and 9
        licensePlate += number;
    }
    for (int i = 0; i < 2; i++) // last 2 letters
    {
        char letter = 'A' + rand() % 26;
        licensePlate += letter;
    }
    return licensePlate;
}