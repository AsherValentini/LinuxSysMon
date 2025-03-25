#include <iostream>   // For std::cout, std::endl
#include <fstream>    // For std::ifstream
#include <sstream>    // For std::istringstream
#include <string>     // For std::string and std::getline
#include <vector>     // For std::vector
#include <iterator>   // For std::istream_iterator#include <stdio.h>
#include <thread>     // For std::thread
#include <atomic>     // For std::atomic
#include <memory>     // for std::shared_ptr
#include <mutex>      // For std::mutex and std::lock_guard and lock()
#include <chrono>     // for std::chrono

using namespace std;  // For removing the need to write std:: (not best practice, but this is not a pruduction project)

// Observer/Subject interface for receiving new CPU data 
class ICPUDataObserver {
public: 
    virtual ~ICPUDataObserver() = default; 
    virtual void update(const vector<long long>& cpuStats)=0;
}; 

// Subect DataCollector module for reading /proc/stat 
class CPUDataCollector {
public:
    void registerObserver(const shared_ptr<ICPUDataObserver>& observer){
        lock_guard<mutex> lock(observerMutex_); 
        observers_.push_back(observer); 
    }

    // Run the data collection loop 
    void run(){
        try{
            while(!stopFlag_){
                vector<long long> stats = collectData(); 
                notifyObservers(stats);
                this_thread::sleep_for(chrono::seconds(1)); // Sample every second

            }
        }catch (const exception &ex){
            cerr << "DataCollector error: " << ex.what() << endl; 
        }
    }

    void stop(){
        stopFlag_ = true; 
    }

private:
    vector<long long> collectData(){

        // Open the proc stat file.
        ifstream statFile("/proc/stat"); 
        if(!statFile.is_open()){

            throw runtime_error("Unable to open /proc/stat"); 
        }
        
        string line; 

        // Read the first line in the proc stat file (CPU statistics).
        if(!getline(statFile, line)){
            throw runtime_error("Unable to read from /proc/stat"); 
        }

        // Parse the line to get the individual CPU statistics 
        istringstream iss(line); // use stringstream to be able to parse numbers seperated by " "
        string cpuLabel;
        iss >> cpuLabel; // first word in line will be the cpu label which is not relavent

        vector<long long>cpuStats{istream_iterator<long long>(iss), istream_iterator<long long>()}; 

        if(cpuStats.size()<4){
            throw runtime_error("Unexpected format in /proc/stat"); 
        }

        return cpuStats; 
    }

    // Notifies all registered observers with new data. 
    void notifyObservers(const vector<long long> stats){
        lock_guard<mutex> lock(observerMutex_); 
        for(auto& observer:observers_){
            observer->update(stats); 
        }
    }

    vector<shared_ptr<ICPUDataObserver>> observers_; 
    mutex observerMutex_; 
    atomic<bool> stopFlag_{false};

};

class CPUDataProcessor : public ICPUDataObserver {
public: 
    // Observer update method.
    void update(const vector<long long>& cpuStats) override {
        long long totalJiffies = 0; 
        for (auto stat:cpuStats){
            totalJiffies += stat; 
        }
    
        // Fourth value in /proc/stat represents idle time. 
        long long idleJiffies = cpuStats[3]; 

        // On the first run, store the values 
        if (prevTotalJiffies_ != 0) {
            long long deltaTotal = totalJiffies - prevTotalJiffies_; 
            long long deltaIdle = idleJiffies -prevIdleJiffies_; 
            double cpuUsage = 100.0 * (1.0 - (deltaIdle / static_cast<double>(deltaTotal))); 
            {
                lock_guard<mutex> lock(outputMutex_); 
                cout << "CPU Usage: " << cpuUsage << "%" << endl; 
            }
        }

        prevTotalJiffies_ = totalJiffies; 
        prevIdleJiffies_= idleJiffies;  
    }

private: 
    long long prevTotalJiffies_{0}; 
    long long prevIdleJiffies_{0}; 
    mutex outputMutex_; 

};

int main(){
    // Create the DataCollector (Subject) and DataProcessor (Observer) modules.
    auto collector = make_shared<CPUDataCollector>(); 
    auto processor = make_shared<CPUDataProcessor>(); 

    // Register the DataProcessor as an oberser. 
    collector->registerObserver(processor); 

    // Run the DataProcessor as an observer. 
    thread collectorThread(&CPUDataCollector::run, collector); 

    //Sumulate a run program run time of 10 seconds. 
    this_thread::sleep_for(chrono::seconds(10)); 
    collector->stop(); 
    collectorThread.join(); 
    return 0; 
}


