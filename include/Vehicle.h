// RICCARDO TOSIN 2137380
#ifndef VEHICLE_H
#define VEHICLE_H
#include <string>
#include <vector>
#include <random>

struct Interval // represents an Interval of the Velocity Profile
{
    double start_; // seconds
    double end_; // seconds
    double v_; // m/s
    double space_; /// meters

    Interval(double start = 0, double end = 0, double velocity = 0, double space = 0) : start_{start}, end_{end}, v_{velocity}, space_{space} {}
};

class Vehicle
{
    public:
        // Constructor
        Vehicle(double entryTime, int entryJunctionId, int exitJunctionId, std::vector<Interval> velProfile);    
        
        // Getters
        const std::string& getLicensePlate() const { return licensePlate_; }
        int getEntryJunctionId() const { return entryJunctionId_; }
        int getExitJunctionId() const  { return exitJunctionId_; }
        double getEntryTime() const { return entryTime_; }
        const std::vector<Interval>& getProfile() const { return velocityProfile_; }

        // Setters
        void setEntryTime(double time) { entryTime_ = time; }

        std::string profileToString() const;

    private:
        std::string licensePlate_;
        int entryJunctionId_;
        int exitJunctionId_;
        double entryTime_;
        std::vector<Interval> velocityProfile_;

        // Private Helpers:
        std::string generateLicensePlate() const;

};



#endif