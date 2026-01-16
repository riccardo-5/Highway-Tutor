// RICCARDO TOSIN 2137380
#ifndef SIMULATOR_H
#define SIMULATOR_H
#include <cmath> // for floor and ceiling functions
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "Highway.h"
#include "Vehicle.h"


class Simulator
{
    public:
        Simulator(const Highway& highway);
        std::vector<Interval> generateVelProfile(int distanceKm, double currentDepartureTime);
        void generateRuns(); // writes on Runs.txt file
        void generatePassages(); // writes on Passages.txt file

    private:
        static constexpr int kNumOfVehicles = 10000;
        static constexpr double kMinGap = 0.5; // seconds
        static constexpr double kMinVelocity = 22.22; // m/s
        static constexpr double kMinTimeInterval = 300; // seconds
        static constexpr double kMaxGap = 10; // seconds
        static constexpr double kMaxVelocity = 52.77; // m/s
        static constexpr double kMaxTimeInterval = 900; // seconds
        std::vector<Vehicle> vehicles_;
        Highway highway_;

    

        // Private Helpers:
        std::pair<int, int> getRandomJunctionIndices(size_t numJunctions) const;
        int randomIntBetween(int from, int to) const;
};



#endif