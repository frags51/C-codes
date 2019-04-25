/**
 * PCP Project
 * Lock-based SkipList Testing!
 * Supreet Singh, Mayank Hooda
 */

#include <iostream>
#include <thread>
#include <string>
#include <cstdlib>
#include <chrono>		/* Measuring Time */

#include "LazySkipList.h"
#include "LockFree.h"

using namespace std;

// Init with uniformly distributed random elements
void InitReadOnlyTest(LazySkipList<int> &a, int num_elements=5000){
	int i=0;
	while(i<num_elements) a.add(i++);
}

void InitReadOnlyTest(LockFreeSkipList<int> &a, int num_elements=5000){
	int i=0;
	while(i<num_elements) a.add(i++);
}

void thFunRW(LazySkipList<int> &a, const double read_frac, 
		const int num_elements, const short iterations, 
		const short id, std::vector<double> &times){
	// init
	times[id]=0;
	int chance = read_frac*100;
	int chance2 = (100-chance)/2+chance;
	auto start = chrono::high_resolution_clock::now();
	auto wt = std::chrono::high_resolution_clock::now() - start;

	bool f;

	for(int i=0; i<iterations;i++){
		int roll = rand()%100;
		if(roll<=chance){ // read
			start = chrono::high_resolution_clock::now();
			f = a.contains(rand()%num_elements);
			wt = std::chrono::high_resolution_clock::now() - start;
		}
		else if(roll<=chance2){
			start = chrono::high_resolution_clock::now();
			f = a.add(rand()%num_elements);
			wt = std::chrono::high_resolution_clock::now() - start;
		}
		else {
			start = chrono::high_resolution_clock::now();
			f = a.remove(rand()%num_elements);
			wt = std::chrono::high_resolution_clock::now() - start;
		} // else
		times[id]+=std::chrono::duration_cast<std::chrono::microseconds>(wt).count();;
	} // for
} // thFunRW

void thFunRW2(LockFreeSkipList<int> &a, const double read_frac, 
		const int num_elements, const short iterations, 
		const short id, std::vector<double> &times){
	// init
	times[id]=0;
	int chance = read_frac*100;
	int chance2 = (100-chance)/2+chance;
	auto start = chrono::high_resolution_clock::now();
	auto wt = std::chrono::high_resolution_clock::now() - start;

	bool f;

	for(int i=0; i<iterations;i++){
		int roll = rand()%100;
		if(roll<=chance){ // read
			start = chrono::high_resolution_clock::now();
			f = a.contains(rand()%num_elements);
			wt = std::chrono::high_resolution_clock::now() - start;
		}
		else if(roll<=chance2){
			start = chrono::high_resolution_clock::now();
			f = a.add(rand()%num_elements);
			wt = std::chrono::high_resolution_clock::now() - start;
		}
		else {
			start = chrono::high_resolution_clock::now();
			f = a.remove(rand()%num_elements);
			wt = std::chrono::high_resolution_clock::now() - start;
		} // else
		times[id]+=std::chrono::duration_cast<std::chrono::microseconds>(wt).count();;
	} // for
} // thFunRW


void doReadWriteTest(const short num_threads, const double read_frac ){
	cerr<<"doReadWriteTest Log: "<<num_threads<<", "<<read_frac<<endl;
	LazySkipList<int> a{};
	int num_elements = 5000;
	const short iterations = 100;

	InitReadOnlyTest(a, num_elements/2);

	srand(time(NULL));

	thread *tids = new thread[num_threads];

	std::vector<double> times(num_threads);

	for(int i=0; i<num_threads;i++) tids[i] = thread(thFunRW, std::ref(a), 
		read_frac, num_elements, iterations, i, std::ref(times));
	for(int i=0; i<num_threads;i++) tids[i].join();

	double total_time = 0;
	for(double e:times) total_time+=e;
	cerr<<"Format: num_threads read_frac ops time(microseconds)\nOperations per millisecond\n";
	cout<<num_threads<<" "<<read_frac<<" "<<(num_threads*iterations)<<" "<<total_time<<endl;
	cout<<((num_threads*iterations*1000)/total_time)<<endl;
} // doReadOnlyTest


void doReadWriteTest2(const short num_threads, const double read_frac ){
	cerr<<"doReadWriteTest Log: "<<num_threads<<", "<<read_frac<<endl;
	LockFreeSkipList<int> a{};
	int num_elements = 5000;
	const short iterations = 100;

	InitReadOnlyTest(a, num_elements/2);

	srand(time(NULL));

	thread *tids = new thread[num_threads];

	std::vector<double> times(num_threads);

	for(int i=0; i<num_threads;i++) tids[i] = thread(thFunRW2, std::ref(a), 
		read_frac, num_elements, iterations, i, std::ref(times));
	for(int i=0; i<num_threads;i++) tids[i].join();

	double total_time = 0;
	for(double e:times) total_time+=e;
	cerr<<"Format: num_threads read_frac ops time(microseconds)\nOperations per millisecond\n";
	cout<<num_threads<<" "<<read_frac<<" "<<(num_threads*iterations)<<" "<<total_time<<endl;
	cout<<((num_threads*iterations*1000)/total_time)<<endl;
} // doReadOnlyTest


int main(int argc, char** argv){
	const short NUM_ARGS = 2;
	string err{"Usage: ./a.out <num_threads> <read_load:Fraction of contains ops>\n"};

	//a.display(' ');
	if(argc!=NUM_ARGS+1){
		cerr<<err;
		exit(1);
	}
	
	doReadWriteTest2(std::atoi(argv[1]), std::stod(argv[2]));

	//malloc_stats_print(NULL, NULL, NULL);

	return 0;
} // main