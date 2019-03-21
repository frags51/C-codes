// SLEEP DURATION FOR THREADMAIN 1 AND 2 ARE DIFFERENT!
// In Case of segfaults, commemnt out delete in SRSW::Write or set DEB to false
#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <random>
#include <fstream>
#include <atomic>
#include <unordered_map>
#include <vector>
// line_65: array declared on stack, re init to some thing on heap.
int n;
int NUM_THREADS = 10;

const bool DEB = false;
bool term = false;

using namespace std;

template <typename T> class StampedSnap{
    public:
    long stamp;
    T value;
    T* snap;
    StampedSnap() noexcept {
        stamp = 0;
        value = NULL;
        snap = nullptr;
    }
    StampedSnap(T val) noexcept{
        stamp = 0;
        value = val;
        snap = nullptr;
    }
    StampedSnap(long label, T val, T* snp) noexcept{
        stamp = label;
        value = val;
        snap = snp;
    }

};

// MRSW Wait free Snapshot
template <typename T> class WFSnapshot{
    private:
    vector<std::atomic<StampedSnap<T>>*> a_table;
    public:
    int capacity;
    
    WFSnapshot(int num_threads){
        capacity = num_threads;
        atomic<StampedSnap<T>> c;
        for(int i=0; i<num_threads;i++) a_table.push_back( new atomic<StampedSnap<T>>);
    }
    ~WFSnapshot(){
        for(int i=0; i<capacity;i++) delete a_table[i];        
    }
    private:
    vector<atomic<StampedSnap<T>>*>* collect(){
        vector<atomic<StampedSnap<T>>*>* copy = new vector<atomic<StampedSnap<T>>*>;
        //copy->reserve(capacity);
        for(int j=0;j<capacity;j++) copy->push_back(new atomic<StampedSnap<T>>);
        for(int j=0; j<capacity;j++) {
            copy->at(j)->store(a_table[j]->load());
        }
        //if(DEB) cout<<"Sizeof Copy: "<<copy->capacity()<<endl;
        return copy;
    }

    public:
    void update(T value, int me){
        T* snap = scan();
        StampedSnap<T> oldValue = a_table[me]->load();
        StampedSnap<T> newValue = StampedSnap<T>(oldValue.stamp+1, value, snap);
        a_table[me]->store(newValue);
    }

    T* scan(){
        vector<atomic<StampedSnap<T>>*>* oldCopy;
        vector<atomic<StampedSnap<T>>*>* newCopy;
        bool moved[this->capacity];
        oldCopy = collect();
        col: while(true){
            newCopy = collect();
            for(int j=0; j<capacity; j++){
                if(oldCopy->at(j)->load().stamp!=newCopy->at(j)->load().stamp){
                    if(moved[j]){
                        return oldCopy->at(j)->load().snap;
                    }
                    else{
                        moved[j] = true;
                        oldCopy = newCopy;
                        goto col;
                    }
                }
            }

            T* result = new T[capacity];
            for(int j=0; j<capacity; j++) result[j] = newCopy->at(j)->load().value;
            return result;
        }
    }
};

// Global Variables!
std::default_random_engine e1;
std::default_random_engine e2; 
std::exponential_distribution<double> csRand; // Time in seconds 
std::exponential_distribution<double> snRand; // Time in seconds 


// The snapshot object!
WFSnapshot<int>* reg; 

int k;
double l1, l2, wait_time = 0;


double wTime = 0.0;
double mWTime = 0.0;

// The actual register.

FILE* fp2 = fopen("MRSW_log.txt", "w");

void snap1(){
    int i= 0;
    char buf[100];
    while(i<k){
        auto _reqTime = chrono::system_clock::now(); //the timepoint
        time_t reqTime = std::chrono::system_clock::to_time_t(_reqTime);
        
        int * blah = reg->scan();

        auto _exitTime = chrono::system_clock::now(); //the timepoint
        auto exitTime = std::chrono::system_clock::to_time_t(_exitTime);
       
        auto tinfo= localtime(&exitTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);

        string s("");
        for(int i=0; i<n; i++){
            s.append("Thread: ");
            s.append(to_string(i)).append(" -> ").append(to_string(blah[i])).append(",");
        }
        fprintf(fp2, "%dth Action completed by snapshot thread at %s, \n Results: %s\n", i, buf, s.c_str());
        
        std::this_thread::sleep_for(std::chrono::duration<double>(csRand(e2)));

        chrono::duration<double> _wt= _exitTime - _reqTime;
        double ww = _wt.count();
        if(ww>mWTime) mWTime = ww;
        wTime+=ww; 

        i++;
    }
} // snap 1

/*
void threadMain(int me, double *wta){
	//SRSW_Atomic<int>::lastRead = new StampedValue<int>();
	int lVar; int id = me; wta[me] = 0.0;
	char buf[100];
	for(int i=0; i<k; i++){
		auto _reqTime = chrono::system_clock::now(); //the timepoint
        time_t reqTime = std::chrono::system_clock::to_time_t(_reqTime);
        struct tm* tinfo= localtime(&reqTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "%dth action request by Thread %d at %s\n", i, id, buf);
        bool action = whichAct(e2);

        if(action){ // read
        	auto _st = chrono::system_clock::now();
        	// read
        	lVar = reg.read(me);

        	auto _et = chrono::system_clock::now();
        	chrono::duration<double> _wt= _et - _st;
        	*(wta+id)+= _wt.count(); // Setting Wait Time

        	fprintf(fp, "Value Read: %d\n", lVar);
        }
        else{
        	lVar = k * me;
        	auto _st = chrono::system_clock::now();

        	reg.write(lVar, me);

        	auto _et = chrono::system_clock::now();
        	chrono::duration<double> _wt= _wt= _et - _st;
        	*(wta+id)+= _wt.count(); // Setting Wait Time
        	fprintf(fp, "Value Write: %d\n", lVar);
        }
        auto _exitTime = chrono::system_clock::now(); //the timepoint
        auto exitTime = std::chrono::system_clock::to_time_t(_exitTime);
       
        tinfo= localtime(&exitTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "%dth Action completed by %d at %s\n", i, id, buf);
        std::this_thread::sleep_for(std::chrono::duration<double>(csRand(e1)));
	}
	return;
}
*/
void threadMainDef(int me, double* wta){
	int lVar;
	char buf[100];
	int id = me;
	while(!term){
        if(DEB) cout<<"Term: "<<term<<endl;
		int num = rand()%8192;
        reg->update(num, me);
        
        auto _reqTime = chrono::system_clock::now(); //the timepoint
        time_t reqTime = std::chrono::system_clock::to_time_t(_reqTime);
        struct tm* tinfo= localtime(&reqTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp2, "%d Written by by Thread %d at %s\n", num, me, buf);
        
        std::this_thread::sleep_for(std::chrono::duration<double>(csRand(e1)));
	}
	return;
}

double* wta = nullptr;
int main(){
    srand(time(NULL));
	//reg.write(56, 0);
	cout<<"starting: "<<endl;
	ifstream inp{"inp-params.txt"}; 

	// INIT
	
    inp>>n>>l1>>l2>>k;

    reg = new WFSnapshot<int>(n);

    NUM_THREADS = n;
    e1.seed(std::chrono::system_clock::now().time_since_epoch().count());
    e2.seed(std::chrono::system_clock::now().time_since_epoch().count());
    csRand = std::exponential_distribution<double>(1/l1);
    snRand = std::exponential_distribution<double>(1/l2);


    // Using my registers
	thread tid[n+1];
	//auto st = chrono::system_clock::now();
	for(int i=0; i<n; i++) 
		tid[i] = std::thread(threadMainDef, i, nullptr);

    tid[n] = std::thread(snap1);
    tid[n].join();
    term = true;

	for(int i=0; i<n; i++) 
		tid[i].join();
	//auto en = chrono::system_clock::now();
	//chrono::duration<double> f = en-st;
	
	wTime = wTime/(k);

	cout<< "Avg Wait time for my MRSW Snapshot: "<<wTime<<endl;
	cout<< "Worst Case time for my MRSW Snapshot: "<<mWTime<<endl;
    

	delete reg;
	return 0;
}