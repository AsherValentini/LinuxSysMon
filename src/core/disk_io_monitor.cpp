#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdint>
#include <mutex>
#include <memory>
#include <atomic>
#include <thread>
#include <chrono>

using namespace std; 

struct DiskStats{
    string deviceName; 
    uint64_t readsCompleted; 
    uint64_t readsMerged; 
    uint64_t sectorsRead; 
    uint64_t timeSpentReading; 
    uint64_t writesCompleted; 
    uint64_t writesMerged; 
    uint64_t sectorsWritten; 
    uint64_t timeSpentWriting;
};

class IDiskDataObserver {
public:
    virtual ~IDiskDataObserver() = default; 
    virtual void update(const std::vector<DiskStats>& diskStats) = 0;  
};

class DiskDataCollector {
public: 
    void registerObserver(const shared_ptr<IDiskDataObserver>& observer){
        lock_guard<mutex> lock(observerMutex_); 
        observers_.push_back(observer);
    }
    void run(){
        try{
            while(!stopFlag_){
                vector<DiskStats> stats = collectData(); 
                notifyObservers(stats); 
                this_thread::sleep_for(chrono::seconds(1)); // Sample every second.
            }
        }catch(const exception &ex){
            cerr << "DiskData Collector error; " << ex.what() << endl; 
        }
    }
    void stop(){
        stopFlag_ = true; 
    }
private: 
    vector<DiskStats> collectData(){
        ifstream diskFile("/proc/diskstats"); 
        if(!diskFile.is_open()){
            throw runtime_error("Unable to open /proc/disktats"); 
        }
        string line; 
        vector<DiskStats> diskStats; 

        while(getline(diskFile, line)){
            istringstream iss(line); 
            string device; 
            int major, minor; 
            DiskStats entry;
            iss >> major >> minor >> device; 
            if(!isRelevantDevice(device)){
                continue;
            }
            entry.deviceName = device;
            iss >> entry.readsCompleted 
                >> entry.readsMerged 
                >> entry.sectorsRead 
                >> entry.timeSpentReading 
                >> entry.writesCompleted
                >> entry.writesMerged
                >> entry.sectorsWritten
                >> entry.timeSpentWriting; 

            diskStats.push_back(entry); 

        }
        return diskStats; 
    }

    bool isRelevantDevice(const string& deviceName){
        // Skip loop, ram, dm 
        if(deviceName.find("loop")==0 || deviceName.find("ram")==0 || deviceName.find("dm")==0){
        return false; 
        }
        // Skip if the device name ends with a digit (partition)
        if(!deviceName.empty() && isdigit(deviceName.back())){
            return false; 
        }
        return true; 
    }


    void notifyObservers(const vector<DiskStats>& stats){
        lock_guard<mutex> lock(observerMutex_); 
        for(auto& observer:observers_){
            observer->update(stats); 
        }
    }

    vector<shared_ptr<IDiskDataObserver>> observers_; 
    mutex observerMutex_; 
    atomic<bool> stopFlag_{false};

};

class DiskDataProcessor : public IDiskDataObserver {
public: 
    void update(const vector<DiskStats>& diskStats){
        for(const auto& stat:diskStats){
            lock_guard<mutex> lock(outputMutex_); 
            cout << "Device: " << stat.deviceName << endl; 
            cout << "Reads Completed: " << stat.readsCompleted << endl; 
        }
    }
private: 
    mutex outputMutex_; 
};

int main(){
    auto diskDataCollector = make_shared<DiskDataCollector>(); 
    auto diskDataProcessor = make_shared<DiskDataProcessor>(); 

    diskDataCollector->registerObserver(diskDataProcessor); 

    thread collectorThread(&DiskDataCollector::run, diskDataCollector);

    this_thread::sleep_for(chrono::seconds(10)); 

    diskDataCollector->stop(); 
    collectorThread.join(); 

    return 0; 
}

