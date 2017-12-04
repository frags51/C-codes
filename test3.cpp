#include <iostream>

int fx(int a, int b=3){
return a+b;
}
int fx(int a){
return a*a;
}

int main(){std::cout<<fx(5)<<std::endl;
return 0;
}
