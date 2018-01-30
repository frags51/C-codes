#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;
ofstream out;
int main(int argc, char **argv){
    out.open("inp-params.txt");
    int n;
    n=atoi(argv[1]);
    out<<n<<endl;
    srand(time(NULL));
    int arr[n*3];
    for(int i=0;i<n*3;i++) arr[i]=rand();
    for(int i=0;i<n*3;i+=3){
        out<<i/3+1<<" "<<20 + arr[i]%20<<" "<<90 + arr[i+1]%40<<" "<<4+arr[i+2]%5<<endl;
    }
    out.close();
    return 0;
}
