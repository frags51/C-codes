#include <iostream>
#include <thread>
#include <string>

#include "LazySkipList.h"

using namespace std;

void InitReadOnlyTest(LazySkipList<int> &a, int num_elements=5000){
	int i=0;
	while(i<num_elements) a.add(i++);
}
// Read only performance

int main(int argc, char** argv){
	LazySkipList<int> a{};
	string err{"Usage: ./a.out <num_threads> <read_load:Fraction of contains ops>"};
	InitReadOnlyTest(a, 10);
	a.display(' ');
	cout<<err<<endl;
	return 0;
}