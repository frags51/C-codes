// std::string doesn;t store after '\0' char by default!
#include <string>
#include <iostream>

using namespace std;

int main(void){
	string a = "f\0ba";
	const char *b = a.c_str();
	cout<<a.at(2);

	return 0;
}