/**
 * PCP assignment 1
 * Supreet Singh
 * This program implements both SAM-1 and DAM
 */
#include <chrono>
#include <algorithm>		/* Sort */
#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>
#include <vector>			/* For vector of threads */
#include <cmath>			/* For POW function */
using namespace std;

#define DEB true

// The counter for DAM
int64_t counter {1};
int64_t temp;
std::mutex mtx;

int64_t getAndIncrement(){ 
	mtx.lock();
	++counter;
	temp = counter;
	mtx.unlock();
	return temp;
}

// check if a number is prime! :: Assume num > 3!
// uses the fact that a prime (except 2 and 3) is of form 6k - 1 or 6k + 1 and looks only at divisors of this form.
bool isPrime(const int64_t num){
	if(num%2==0) return false;
	if(num%3==0) return false;

	int64_t i = 5;
	int w = 2;

	while(i*i <= num){ // check divisility only by primes < sqrt(num)!
		if(num%i == 0) return false;
		i+=w;
		w = 6-w;
	}

	return true;
}

// pass local ID, max range to search for primes and number of threads
void threadFuncSAM1(const short which, const int64_t MAX, const int M, const std::vector<std::vector<int64_t>* >* primes){
	int start = (which%M)+1;
	// we know that 1 is not a prime number.
	start = start==1? (M+1) : start;
	if(start==2){
		primes->at(which)->push_back(2);
		start+=M;
	}
	else if(start==3){
		primes->at(which)->push_back(3);
		start+=M;
	}

	for(;start<MAX;start+=M){
		if(isPrime(start)) primes->at(which)->push_back((int64_t) start);
	} 
}

// pass local ID, max range to search for primes and number of threads
void threadFuncDAM(const short which, const int64_t MAX, const int M, const std::vector<std::vector<int64_t>* >* primes){
	int64_t start = getAndIncrement();
	// we know that 1 is not a prime number.
	
	if(start==2){
		primes->at(which)->push_back(2);
	}
	else if(start==3){
		primes->at(which)->push_back(3);
	}
	
	for(;start<MAX;){
		start = getAndIncrement();
		if(start <MAX && isPrime(start)) primes->at(which)->push_back((int64_t) start);
	} 
}

int main(){
	// get input.
	ifstream inp {"inp-params.txt"};
	int N, M; // No to calculate prime upto, and number of threads.
	inp >> N >> M;
	inp.close();

	int64_t MAX = pow(10, (double) N);

	// vector of std:: thread objects.
	std::vector<std::thread> thread_pool;
	thread_pool.reserve(M);

	// for each thread to store primes
	std::vector<std::vector<int64_t>*>* primes = new std::vector<std::vector<int64_t>*>();
	primes->reserve(M);
	for(int i=0; i<M; i++) primes->push_back(new std::vector<int64_t>());

	// Also need to time this!
	auto start = std::chrono::system_clock::now();
	// start these threads
	for (short i = 0; i < M; ++i)
	{
		thread_pool.push_back (std::thread(threadFuncSAM1, i,MAX, M, primes));
	}

	// wait foor each thread to finish execution
	for (int i = 0; i < M; ++i)
	{
		thread_pool[i].join();
	}

	// Output time in milliseconds
	fstream time {"Times.txt"};
	auto end = std::chrono::system_clock::now();
	auto elapsed1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	//time << elapsed.count() << ' ';
	if(DEB) cout<<"SAM1 Debug: time: "<<elapsed.count() << " ms\n";
	// Timing finished!

	// merge all these primes!
	std::vector<int64_t> merged;
	for(int i=0;i<M; i++){
		for(int j=0; j<primes->at(i)->size();j++) merged.push_back(primes->at(i)->at(j));
	}
	
	// SORT!
	std::sort(merged.begin(), merged.end());

	// output to file!
	ofstream out {"Primes-SAM1.txt"};
	for(auto i: merged) out<<i<<" ";
	out.close();

	// deletion -- memory management
	for(int i=0; i<M; i++) delete primes->at(i);


	// ****************************** STARTING DAM *******************************************
	primes->clear();
	primes->reserve(M);
	for(int i=0; i<M; i++) 
		primes->push_back(new std::vector<int64_t>());

	start = std::chrono::system_clock::now();
	for (short i = 0; i < M; ++i)
	{
		thread_pool[i] = (std::thread(threadFuncDAM, i,MAX, M, primes));
	}

	// wait foor each thread to finish execution
	for (int i = 0; i < M; ++i)
	{
		thread_pool[i].join();
	}
	end = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	time << elapsed.count() << ' '<<elapsed1.count();
	if(DEB) cout<<"Debug: DAM time: "<<elapsed.count() << " ms\n";
	time.close();
	// Timing finished!

	// now merge!
	merged.clear();
	for(int i=0;i<M; i++){
		for(int j=0; j<primes->at(i)->size();j++) merged.push_back(primes->at(i)->at(j));
	}

	// SORT!
	std::sort(merged.begin(), merged.end());

	// output to file!
	ofstream out1 {"Primes-DAM.txt"};
	for(auto i: merged) out1<<i<<" ";
	out1.close();

	// mem management
	for(int i=0; i<M; i++) delete primes->at(i);
	delete primes;
	
	return 0;
} // main

