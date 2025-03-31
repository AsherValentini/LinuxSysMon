// initial driver script to read and parse from the proc/mem_info file 

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <mutex>
#include <memory>
#include <atomic>
#include <map>
#include <thread>
#include <chrono>


using namespace std; 

class IMemoryDataObserver {
public: 
    virtual ~IMemoryDataObserver() = default; 
    virtual void update(const map<string, long long>& memoryStats)=0;
}; 


class MemoryDataCollector {
public:
    void registerObserver(const shared_ptr<IMemoryDataObserver>& observer){
        lock_guard<mutex> lock(observerMutex_); 
        observers_.push_back(observer); 
    }

    void run(){
        try{
            while(!stopFlag_){
                map<string, long long> stats = collectData(); 
                notifyObservers(stats); 
                this_thread::sleep_for(chrono::seconds(1)); 

            }
        }catch(const exception &ex){
            cerr << "MemoryDataCollector error: " << ex.what() << endl;
        }

    }

    void stop(){
        stopFlag_ = true; 
    }

private: 
    map<string, long long> collectData(){
        ifstream memInfoFile("/proc/meminfo"); 
        if(!memInfoFile.is_open()){
            throw runtime_error("Unable to open /proc/meminfo"); 
        }

        map<string, long long> memoryStatsMap; 
        string line; 
        while(getline(memInfoFile, line)){
            istringstream iss(line); 
            string label; 
            long long value; 
            iss >> label >> value; 
            memoryStatsMap[label] = value; 
        }

        return memoryStatsMap; 
    }

    void notifyObservers(const map<string, long long> stats){
        lock_guard<mutex> lock(observerMutex_); 
        for(auto& observer:observers_){
            observer->update(stats);  // implement observers update method
        }
    }

    mutex observerMutex_;
    vector<shared_ptr<IMemoryDataObserver>> observers_; 
    atomic<bool> stopFlag_{false}; 
};

class MemoryDataProcessor : public IMemoryDataObserver {
public: 
    void update(const map<string, long long>& memoryStats) override {

        // Process just the MemTotal, MemAvailable and SwapTotal and SwapFree fields. 
        long long usedMemory = memoryStats.at("MemTotal:")-memoryStats.at("MemAvailable:"); 
        long long swapUsed = memoryStats.at("SwapTotal:")-memoryStats.at("SwapFree:"); 
        cout << "Used Memory: " << usedMemory << endl; 
        cout << "Memory Statistics" << endl; 

        for(auto entry:memoryStats){
            lock_guard<mutex> lock(outputMutex_); 
            cout << entry.first << " " << entry.second << endl; // currently does not prevent race conidtion need to use global mutex when it comes to console printing
        }
    }
private: 
    mutex outputMutex_; 
};

using namespace std;

int main(){
    
    auto collector = make_shared<MemoryDataCollector>(); 
    auto processor = make_shared<MemoryDataProcessor>(); 

    collector->registerObserver(processor); 

    thread collectorThread(&MemoryDataCollector::run, collector); 

    this_thread::sleep_for(chrono::seconds(1)); 

    collector->stop(); 

    collectorThread.join(); 

    return 0; 
}