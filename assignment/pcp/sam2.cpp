/**
 * Implements SAM-2
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

const bool FILEIO = true;

#define DEB true

const int SLICE = 1<<17; // 2^17 = 128 KByte

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
	int start = (which%M); 
	
	int64_t from;
	from = 2+ (start*SLICE); // inclusive.
	int64_t to;

	while(from < MAX){
		to = from + SLICE; //exclusive.
		if(to > MAX) to = MAX;	
		
		// *********** start processing the slice.
		// [to, from)
		int64_t j = from;
		
		// Skip even number.
		if((j&1)==0) j=j+1;

		// CHECK ONLY ODD NUMBERS ! (increment by 2.)
		for(; j<to;j+=2){
			if(isPrime(j)) primes->at(which)->push_back(j);
		}
		// *********** end processing.
		// 
		// need to explicitly check for 2/3.
		if(from<=2 && to>2) primes->at(which)->push_back(2);
		if(from<=3 && to>3) primes->at(which)->push_back(3);

		from+= ((int64_t)M*SLICE);
	} // thread while!
 
}

// pass local ID, max range to search for primes and number of threads

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
	fstream time {"Times-SAM2.txt"};
	auto end = std::chrono::system_clock::now();
	auto elapsed1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	time << elapsed1.count() << ' ';
	if(DEB) cout<<"SAM2 Debug: time: "<<elapsed1.count() << " ms\n";
	// Timing finished!
	time.close();

	if(FILEIO){
		// merge all these primes!
		std::vector<int64_t> merged;
		for(int i=0;i<M; i++){
			for(int j=0; j<primes->at(i)->size();j++) merged.push_back(primes->at(i)->at(j));
		}
	
		// SORT!
		std::sort(merged.begin(), merged.end());

		// output to file!
		ofstream out {"Primes-SAM2.txt"};
		for(auto i: merged) out<<i<<" ";
		out.close();
	}
	
	

	// deletion -- memory management
	for(int i=0; i<M; i++) delete primes->at(i);
	delete primes;
	
	return 0;
} // main

