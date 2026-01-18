// RICCARDO TOSIN 2137380
#include "../include/Simulator.h"

// Constructor
Simulator::Simulator(const Highway& highway) : highway_{highway}
{
    static std::mt19937 gen(std::random_device{}()); // create random generator. random_device() return the seed for the generator
    std::uniform_real_distribution<double> dist(kMinGap, kMaxGap); // initializing a uniform distribution within the (0.5-10) seconds range

    vehicles_.reserve(kNumOfVehicles); // optimization since I already know the total num of vehicles
    double currentDepartureTime = 0.0; // first starts at t=0 

    for (int i = 0; i < kNumOfVehicles; ++i) 
    {
        auto [entryId, exitId] = getRandomJunctionIndices(highway_.getJunctions().size()); // select randomly the entry and the exit points
        double traveledDistance = highway_.getJunctions()[exitId].getKm() - highway_.getJunctions()[entryId].getKm(); // in km
        Vehicle newVehicle(currentDepartureTime, entryId+1, exitId+1, generateVelProfile(traveledDistance, currentDepartureTime)); // transforming indexes from 0-based to 1-based
        
        vehicles_.push_back(newVehicle);

        // Generate the random gap (in seconds) from this vehicle to the next
        double gap = dist(gen);
        // Update time for next vehicle
        currentDepartureTime += gap;
    }
}

namespace fs = std::filesystem;
void Simulator::generateRuns(const std::string& path)
{
   fs::path filePath = path;
   
   std::ofstream outFile(filePath); // open output file
   if (outFile)
   {
        for (Vehicle& v : vehicles_)
        {
            outFile << v.getLicensePlate() << "  " << v.getEntryJunctionId() << "  " << v.getExitJunctionId() << "  " << v.getEntryTime() << " " << v.profileToString() << "\n";
        }
   } else {
        std::cerr << "non riesco ad aprire il file in: " << filePath << std::endl;
   }
}

void Simulator::generatePassages(const std::string& path)
{
    fs::path filePath = path;
    
    std::ofstream outFile(filePath); // open output file
    if (outFile)
    {
        for (Vehicle& v : vehicles_)
        {
            const std::vector<int>& passages_ids = highway_.getPassagesInBetween(v.getEntryJunctionId(), v.getExitJunctionId()); // retrieve all the ids of the passages crossed by the current vehicle 
            for (int id : passages_ids)
            {
                size_t intervalCounter = 0; // start from the first Interval in the velocity profile of current vehicle
                double currentPassageDist = static_cast<double>(highway_.getPassages()[id-1].getKm()) * 1000.0; // in meters
                double currentVehicleDist = static_cast<double>(highway_.getJunctions()[v.getEntryJunctionId()-1].getKm()) * 1000.0; // in meters

                const std::vector<Interval>& profile = v.getProfile(); // retrieve the current vehicle velocity profile
                
                if (intervalCounter < profile.size()) // check if the vehicle's velocity is not empty
                {
                    currentVehicleDist += profile[intervalCounter].space_; // start from the start Interval space

                    while (currentVehicleDist < currentPassageDist && (intervalCounter + 1) < profile.size()) // while we've not reached the passage yet and we've not reached the end of the profile 
                    {
                        intervalCounter++; // go to the next  Interval in the profile
                        currentVehicleDist += profile[intervalCounter].space_; // the vehicle goes to the end of the current interval
                    }
                    currentVehicleDist -= profile[intervalCounter].space_; // return to the beginning position of current Interval since we've found the right one
                    double remainingDist = currentPassageDist - currentVehicleDist;
                    double remainingTime = remainingDist / profile[intervalCounter].v_; // since the velocity is constant during an Interval
                    double transitionMoment = profile[intervalCounter].start_ + remainingTime; // calculate the transitionMoment by adding the time it takes to travel the remainingDist to the start of the interval
                
                    outFile << id << " " << v.getLicensePlate() << " " << transitionMoment << "\n";
                }
            }
        }
    } else {
        std::cerr << "non riesco ad aprire il file in: " << filePath << std::endl;
    }
}


std::vector<Interval> Simulator::generateVelProfile(double totalDistanceKm, double currentDepartureTime)
{
    if (totalDistanceKm <= 0) return {}; // return an empty vector calling its default constructor (so no heap involved)

    double totalMeters = totalDistanceKm * 1000.0; // convert Km to meters
    constexpr int kMinSegmentLength = kMinVelocity * kMinTimeInterval; // the minimum segment length given the time and velocity constraints
    constexpr int kMaxSegmentLength = kMaxVelocity * kMaxTimeInterval; // the maximum segment length given the time and velocity constraints


    // Calculate bounds for how many segments an Interval can have based on time and velocity constraints
    int minNumSegments = std::ceil((double)totalMeters / kMaxSegmentLength);
    int maxNumSegments = std::floor((double)totalMeters / kMinSegmentLength);

    if (maxNumSegments < 1) maxNumSegments = 1; // if the distance traveled by the vehicle is too small, maxNumSegments becomes 0. In this case I'm forcing at least 1 segment to exist
    if (minNumSegments > maxNumSegments) minNumSegments = maxNumSegments; // making sure min doesn't surpass max
    

    // Choose the number of segments with which we want to divide our velocity profile
    int numOfIntervals = randomIntBetween(minNumSegments, maxNumSegments);
    std::vector<Interval> intervals(numOfIntervals); // I have numOfIntervals intervals in total
    std::vector<int> randomWeights(numOfIntervals); // every interval has its own weight

    int weightSum = 0;
    for (int i = 0; i < numOfIntervals; i++)
    {
        randomWeights[i] = randomIntBetween(1, 10); // choose a random weight to assign to each interval (how big the interval should be)
        weightSum += randomWeights[i]; // keep track of the weight sum
    }

    double currentPos = 0.0; // used to correct the floating point error
    double timeCounter = currentDepartureTime; // used to keep track of the time it passes between intervals (in seconds). It starts at the current vehicle departureTime

    // Convert each weight into a fraction of the totalDistance and assign it to the Interval's space_ variable
    for (int i = 0; i < numOfIntervals; ++i)
    {
        double fraction = double(randomWeights[i]) / weightSum;

        // For the very last segment, take the remaining distance to fix floating point rounding errors (like if the sum is ending at 999.9999m)
        if (i == numOfIntervals-1) {
            intervals[i].space_ = totalMeters - currentPos; // this guarantees that the sum of all Interval spaces_ equal the total distance
        } else {
            intervals[i].space_ = fraction * totalMeters;
        }
        currentPos += intervals[i].space_;
        
        // Random velocity in m/s (approx 82 - 187 km/h)
        intervals[i].v_ = randomIntBetween(23, 52); // choose a fixed velocity and calculate the Interval's time depending on that

        // Time = Space (m) / Velocity (m/s)
        double intervalTime = intervals[i].space_ / intervals[i].v_;

        intervals[i].start_ = timeCounter;
        intervals[i].end_   = timeCounter + intervalTime;
        
        timeCounter += intervalTime; // keep track of the time between vehicles departures
    }
    return intervals;
}


// --- PRIVATE HELPER FUNCTIONS

std::pair<int, int> Simulator::getRandomJunctionIndices(size_t numJunctions) const
{
    // Safety check: need at least two junctions to have an entry and an exit
    if (numJunctions < 2) return {0, 0}; 

    // Pick first index (everyone except the last one)
    int i = randomIntBetween(0, numJunctions - 2);

    // Pick second index (must be strictly after i)
    int j = randomIntBetween(i + 1, numJunctions - 1);

    return {i, j};
}

int Simulator::randomIntBetween(int from, int to) const
{
    static std::mt19937 gen(std::random_device{}()); // create the random engine (only one time since it's static and it lives in the .data section)
    std::uniform_int_distribution<int> dist(from, to);
    int random = dist(gen);
    return random;
}






