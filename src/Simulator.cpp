// RICCARDO TOSIN 2137380
#include "../include/Simulator.h"

// Constructor
Simulator::Simulator(const Highway& highway)
{
    static std::mt19937 gen(std::random_device{}()); // create random generator. random_device() return the seed for the generator
    std::uniform_real_distribution<double> dist(kMinGap, kMaxGap); // initializing a uniform distribution within the (0.5-10) seconds range

    vehicles_.reserve(kNumOfVehicles); // optimization since I already know the total num of vehicles
    double currentDepartureTime = 0.0; // first starts at t=0 

    for (int i = 0; i < kNumOfVehicles; ++i) 
    {
        auto [entryId, exitId] = getRandomJunctionIndices(highway.getJunctions().size()); // select randomly the entry and the exit points
        int traveledDistance = highway.getJunctions()[exitId] - highway.getJunctions()[entryId]; // in km
        Vehicle newVehicle(currentDepartureTime, entryId, exitId, generateVelProfile(traveledDistance, currentDepartureTime));
        
        vehicles_.push_back(newVehicle);

        // Generate the random gap (in seconds) from this vehicle to the next
        double gap = dist(gen);
        // Update time for next vehicle
        currentDepartureTime += gap;
    }
}

namespace fs = std::filesystem;
void Simulator::generateRuns()
{
   fs::path targetDir = "../Data";
   fs::path filePath = targetDir / "Runs.txt"; // creates full path "../Data/Runs.txt"
   
   std::ofstream outFile(filePath); // open output file
   if (outFile)
   {
        for (Vehicle& v : vehicles_)
        {
            outFile << v.getLicensePlate() << " " << v.getEntryJunctionId() << " " << v.getExitJunctionId() << " " << v.getEntryTime() << " " << v.profileToString() << "\n";
        }
   } else {
        std::cerr << "I couldn't open the file" << std::endl;
   }
}


std::vector<Interval> Simulator::generateVelProfile(int totalDistanceKm, double currentDepartureTime)
{
    if (totalDistanceKm <= 0) return {}; // return an empty vector calling its default constructor (so no heap involved)

    double totalMeters = static_cast<double>(totalDistanceKm) * 1000.0; // convert Km to meters with double precision
    constexpr int kMinSegmentLength = kMinVelocity * kMinTimeInterval; 
    constexpr int kMaxSegmentLength = kMaxVelocity * kMaxTimeInterval; 


    // Calculate bounds for how many segments an Interval can have based on time and velocity constraints
    int minNumSegments = std::ceil((double)totalMeters / kMaxSegmentLength);
    int maxNumSegments = std::floor((double)totalMeters / kMinSegmentLength);

    if (maxNumSegments < minNumSegments) throw std::runtime_error("minNumSegments > maxNumSegments");

    // Choose the number of segments with which we want to divide our velocity profile
    int N = randomIntBetween(minNumSegments, maxNumSegments);
    std::vector<Interval> intervals(N); // I have N intervals in total
    std::vector<int> randomWeights(N); // every interval has its own weight

    int weightSum = 0;
    for (int i = 0; i < N; i++)
    {
        randomWeights[i] = randomIntBetween(1, 10); // choose a random weight to assign to each interval (how big the interval should be)
        weightSum += randomWeights[i];
    }

    double currentPos = 0.0; // used to correct the floating point error
    double timeCounter = currentDepartureTime; // used to keep track of the time it passes between intervals (in seconds). It starts at the current vehicle departureTime

    // Convert each weight into a fraction of the totalDistance and assign it to the Interval's space_ variable
    for (int i = 0; i < N; ++i)
    {
        double fraction = double(randomWeights[i]) / weightSum;

        // For the very last segment, take the remaining distance to fix floating point rounding errors (like if the sum is ending at 999.9999m)
        if (i == N-1) {
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
        
        timeCounter += intervalTime;
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






