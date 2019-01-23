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
		
		// *********** start sieving.
		// [to, from)
		const int memory = (to-from) / 2; // only storing odd numbers.


		char* isNumPrime = new char[memory]; 		
		
		// set all to prime initially
		for (int i = 0; i < memory; i++) isNumPrime[i] = 1;
		
		// Now discover all non primes.
		// We don't have access to results of other threads
		// We'll check for non primes by dividing by numbers upto sqrt (to).
		// 
		for(int64_t i=3; i*i<to; i+=2){ // loop only till sqrt(to)
			// no need to explicitly loop over and mark (as not prime) multiples of multiples of known primes
			// like 3, 5, 7 and 11.
			// 
			if(i>3*3 && i%3==0) continue;
			else if(i>=5*5 && i%5 == 0) continue;
			else if(i >=7*7 && i%7 == 0) continue;
			else if(i>=11*11 && i%11==0) continue;

			int64_t firstMultiple; // The first odd multiple in range [from, to).

			firstMultiple = ((int64_t)((from+i-1)/i)) * i;

			// only mark multiples above i*i;
			//firstMultiple = firstMultiple < i*i ? i*i : firstMultiple;
			if(firstMultiple < i*i) firstMultiple = i*i;
			// Skipping even multiples!
			if((firstMultiple & 1) == 0) firstMultiple+= i;

			//now mark all non primes!
			for(int64_t j = firstMultiple; j<to; j+=2*i){
				isNumPrime[((int)(j-from))/2] = 0;
			}
		} 

		for(int i=0; i<memory; i++){
			if(isNumPrime[i]) primes->at(which)->push_back((int64_t)(2*i + from + 1));
		}
		if(from<=2) primes->at(which)->push_back(2);
		delete[] isNumPrime; 
		// *********** end sieving.
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

	/*
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
	*/

	// deletion -- memory management
	for(int i=0; i<M; i++) delete primes->at(i);
	delete primes;
	
	return 0;
} // main

