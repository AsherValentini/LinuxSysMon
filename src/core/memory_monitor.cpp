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


using namespace std; 

class IMemoryDataObserver {
    public: 
        virtual ~IMemoryDataObserver() = default; 
        virtual void update(const vector<long long>& memoryStats)=0;
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
                    map<string, long long> stats = collect
                }
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
        
        mutex observerMutex_;
        vector<shared_ptr<IMemoryDataObserver>> observers_; 
        atomic<bool> stopFlag_{false}; 
};



using namespace std;

int main(){
    
    // Open the proc/meminfo file.
    ifstream memInfoFile("/proc/meminfo"); 

    // Check if the file is open
    if(!memInfoFile.is_open()){
        throw runtime_error("Unable to open /proc/meminfo");
    }

    map<string, long long> memoryStatsMap;
    string line;
    while (getline(memInfoFile, line)) {
        istringstream iss(line);
        string label;
        long long value;
        iss >> label >> value;
        memoryStatsMap[label]=value;
    }

    for(auto entry:memoryStatsMap){
        cout << entry.first << " " << entry.second << endl; 
    }

    return 0; 
}