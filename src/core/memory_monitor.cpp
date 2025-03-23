// initial driver script to read and parse from the proc/mem_info file 

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>


using namespace std;

int main(){
    
    // Open the proc/meminfo file.
    ifstream memInfoFile("/proc/meminfo"); 

    // Check if the file is open
    if(!memInfoFile.is_open()){
        throw runtime_error("Unable to open /proc/meminfo");
    }

    // Read the first line in the proc/meminfo file (total memory).
    string line; 
    if(!getline(memInfoFile, line)){
        throw runtime_error("Unable to read from /proc/meminfo");
    }

    // Parse the line to get the total memory. 
    istringstream iss(line); 
    string label; 
    iss >> label; // first word in the line will be the label which is not relevant 
    long long totalMemory; 
    iss >> totalMemory; 

    cout << totalMemory << endl;

    return 0; 
}