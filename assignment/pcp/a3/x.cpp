#include <iostream>
#include <cstdlib>
#include <thread>
#include <random>
#include <fstream>
#include <atomic>
#include <unordered_map>
// line_65: array declared on stack, re init to some thing on heap.
int n;
const int NUM_THREADS = 51;

using namespace std;

template<typename T> class StampedValue{
	public:
		long stamp;
		T value;
		StampedValue(){
			stamp = 0;
			value = 0;
		}
		StampedValue(T init){
			stamp=0;
			value = init;
		}
		StampedValue(long st, T va) {
			stamp = st;
			value = va;
		}
		static StampedValue* max(StampedValue* x, StampedValue* y){
			if(x->stamp > y->stamp) return x;
			else return y;
		}
		static StampedValue<T>* MIN_VALUE;
}; 

template <typename T> StampedValue<T>* StampedValue<T>::MIN_VALUE ;

template<typename T> class SRSW_Atomic
{
private:
	thread_local static long lastStamp;
	StampedValue<T>* r_value;

public:
	//thread_local static StampedValue<T>* lastRead;
	static thread_local std::unordered_map<SRSW_Atomic*, StampedValue<T>*> s_B;
	SRSW_Atomic(){
		lastStamp = 0;
		r_value = new StampedValue<T>();
		s_B.insert({this, r_value});
		//lastRead = r_value;
	}
	SRSW_Atomic(T init){
		r_value = new StampedValue<T>(init);
		lastStamp = 0;
		s_B.insert({this, r_value});
		
		//lastRead = r_value;
	}

	~SRSW_Atomic(){
		delete[] r_value;
	}

	void write(T v){
		long stamp = lastStamp+1;
		r_value = new StampedValue<T>(stamp, v);
		lastStamp = stamp;
	}

	T read(){
		if(s_B[this] == nullptr) s_B[this] = r_value;
		StampedValue<T>* value = r_value;
		//StampedValue<T>* last = lastRead;
		StampedValue<T>* last = s_B[this];
		StampedValue<T>* result = StampedValue<T>::max(value, last);

		//lastRead = result;
		s_B[this] = result;
		return result->value;
	}
	
};
template<typename T> thread_local long SRSW_Atomic<T>::lastStamp;
//template<typename T> thread_local StampedValue<T>* SRSW_Atomic<T>::lastRead;
template<typename T> thread_local std::unordered_map<SRSW_Atomic<T>*, StampedValue<T>*> SRSW_Atomic<T>::s_B;

template<typename T> class MRSW_Atomic{
private:
	thread_local static long lastStamp;
	SRSW_Atomic<StampedValue<T>*> a_table[NUM_THREADS][NUM_THREADS];

public:
	MRSW_Atomic(){
		lastStamp = 0;
		StampedValue<T>* value = new StampedValue<T>();
		for(int i=0; i<NUM_THREADS;i++) for(int j=0; j<NUM_THREADS;j++)
			a_table[i][j].write(value);
	}
	MRSW_Atomic(T init){
		lastStamp = 0;
		//a_table = new SRSW_Atomic<StampedValue<T>*>[NUM_THREADS][NUM_THREADS];
		StampedValue<T>* value = new StampedValue<T>(init);
		for(int i=0; i<NUM_THREADS;i++) for(int j=0; j<NUM_THREADS;j++)
			a_table[i][j].write(value);
	}

	void write(T v){
		long stamp = lastStamp+ 1;
		lastStamp = stamp;
		StampedValue<T>* value = new StampedValue<T>(stamp, v);
		for(int i=0; i<NUM_THREADS; i++)
			a_table[i][i].write(value);
	}

	T read(int me){
		StampedValue<T>* value = a_table[me][me].read();
		for (int i = 0; i < NUM_THREADS; i++) {
			value = StampedValue<T>::max(value, a_table[i][me].read());
		}
		for (int i = 0; i < NUM_THREADS; i++) {
			a_table[me][i].write(value);
		}
		return value->value; // ?
	}

};
template<typename T> thread_local long MRSW_Atomic<T>::lastStamp;

template <typename T> class MRMW_Atomic{
public:
	MRSW_Atomic<StampedValue<T>*> a_table[NUM_THREADS];

	MRMW_Atomic(T init){
		StampedValue<T>* value = new StampedValue<T>(init);
		for(int j=0; j<NUM_THREADS;j++)
			a_table[j].write(value);

	}
	~MRMW_Atomic() = default;

	void write(T value, int me){
		StampedValue<T>* max = StampedValue<T>::MIN_VALUE;
		for (int i = 0; i < NUM_THREADS; i++) {
			int xx;
			max = StampedValue<T>::max(max, a_table[i].read(me));
			int yy;
		}
		a_table[me].write(new StampedValue<T>(max->stamp + 1, value));
	}

	T read(int me){
		StampedValue<T>* max = StampedValue<T>::MIN_VALUE;
		for (int i = 0; i < NUM_THREADS; i++) {
			int xx;
			max = StampedValue<T>::max(max, a_table[i].read(me));
		}
		return max->value;
	}
	
};

// Global Variables!
std::default_random_engine e1;
std::default_random_engine e2; 
std::exponential_distribution<double> csRand; // Time in seconds 
std::bernoulli_distribution whichAct;

int k;
double l1, p, wait_time = 0;
// The actual register.
MRMW_Atomic<int> reg(5);
std::atomic<int> std_reg;

FILE* fp = fopen("MRMW_log.txt", "w");
FILE* fp2 = fopen("Atomic_log.txt", "w");

void threadMain(int me){
	//SRSW_Atomic<int>::lastRead = new StampedValue<int>();
	int lVar; int id = me;
	char buf[100];
	for(int i=0; i<k; i++){
		auto _reqTime = chrono::system_clock::now(); //the timepoint
        time_t reqTime = std::chrono::system_clock::to_time_t(_reqTime);
        struct tm* tinfo= localtime(&reqTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "%dth action request by Thread %d at %s\n", i, id, buf);
        bool action = whichAct(e2);

        if(action){ // read
        	lVar = reg.read(me);
        	fprintf(fp, "Value Read: %d\n", lVar);
        }
        else{
        	lVar = k * me;
        	reg.write(lVar, me);
        	fprintf(fp, "Value Write: %d\n", lVar);
        }
        auto _exitTime = chrono::system_clock::now(); //the timepoint
        auto exitTime = std::chrono::system_clock::to_time_t(_exitTime);
        chrono::duration<double> _wt= _exitTime - _reqTime;
        wait_time+= _wt.count(); // Setting Wait Time
        tinfo= localtime(&exitTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "%dth Action completed by %d at %s\n", i, id, buf);
        std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(csRand(e1)));
	}
	return;
}

void threadMainDef(int me){
	int lVar;
	char buf[100];
	int id = me;
	for(int i=0; i<k; i++){
		auto _reqTime = chrono::system_clock::now(); //the timepoint
        time_t reqTime = std::chrono::system_clock::to_time_t(_reqTime);
        struct tm* tinfo= localtime(&reqTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp2, "%dth action request by Thread %d at %s\n", i, id, buf);
        bool action = whichAct(e2);

        if(action){ // read
        	lVar = std_reg.load();
        	fprintf(fp2, "Value Read: %d\n", lVar);
        }
        else{
        	lVar = k * me;
        	std_reg.store(lVar);
        	fprintf(fp2, "Value Write: %d\n", lVar);
        }
        auto _exitTime = chrono::system_clock::now(); //the timepoint
        auto exitTime = std::chrono::system_clock::to_time_t(_exitTime);
        //chrono::duration<double> _wt= _exitTime - _reqTime;
        //wait_time+= _wt.count(); // Setting Wait Time
        tinfo= localtime(&exitTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp2, "%dth Action completed by %d at %s\n", i, id, buf);
        std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(csRand(e1)));
	}
	return;
}

int main(){
	StampedValue<int>::MIN_VALUE = new StampedValue<int>();
	
	//reg.write(56, 0);
	cout<<"starting: "<<reg.read(0)<<endl;
	ifstream inp{"inp-params.txt"}; 

    inp>>n>>k>>l1>>p;
    e1.seed(std::chrono::system_clock::now().time_since_epoch().count());
    e2.seed(std::chrono::system_clock::now().time_since_epoch().count());
    csRand = std::exponential_distribution<double>(l1);
    whichAct =  std::bernoulli_distribution(p);

	thread tid[n];
	auto st = chrono::system_clock::now();
	for(int i=0; i<n; i++) 
		tid[i] = std::thread(threadMain, i);
	for(int i=0; i<n; i++) 
		tid[i].join();
	auto en = chrono::system_clock::now();
	chrono::duration<double> f = en-st;
	wait_time = f.count();
	wait_time=wait_time/(n*k);

	cout<< "Wait time for my implementation: "<<wait_time<<endl;

	wait_time = 0;
	
	st = chrono::system_clock::now();
	for(int i=0; i<n; i++) 
		tid[i] = std::thread(threadMainDef, i);
	for(int i=0; i<n; i++) 
		tid[i].join();
	en = chrono::system_clock::now();
	f = en-st;
	wait_time = f.count();
	wait_time=wait_time/(n*k);

	cout<< "Wait time for C++ std::atomic: "<<wait_time<<endl;


	return 0;
}