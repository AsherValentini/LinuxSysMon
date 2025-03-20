#include <iostream>   // For std::cout, std::endl
#include <fstream>    // For std::ifstream
#include <sstream>    // For std::istringstream
#include <string>     // For std::string and std::getline
#include <vector>     // For std::vector
#include <iterator>   // For std::istream_iterator#include <stdio.h>


using namespace std; 

int main(){

    ifstream statFile("/proc/stat");
    string line; 

    // Read the first line in the proc stat file with overal CPU statistics 
    getline(statFile, line);
    
    // Pare the line to extract individual CPU statistics 
    istringstream iss(line); 

    string cpuLabel; 
    iss >> cpuLabel; 

    vector<long long> cpuStats{istream_iterator<long long>(iss), istream_iterator<long long>()};

    // Calculate total jiffies and idle jiffies 
    long long totalJiffies = 0; 
    for(size_t i = 0; i<cpuStats.size(); ++i){
        totalJiffies += cpuStats[i]; 
    }
    long long idleJiffies = cpuStats[3]; 

    // Calculate CPU usage as a percentage 
    double cpuUsage = 100.0 * (1.0 -(idleJiffies / static_cast<double>(totalJiffies))); 

    cout << "CPU Usage: " << cpuUsage << "%" << endl; 
    
    return 0; 
}


