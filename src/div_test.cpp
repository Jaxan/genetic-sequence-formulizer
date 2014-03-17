#include <iostream>
#include <vector>
#include <limits>

// I ran into some trouble with division, so I decided to test the boundary
// cases. Turns out that INT_MIN / -1 overflows and signals a SIGFPE :(.

using namespace std;
using T = long int;

static T myDiv(T x, T y){
	if(!y) {
		std::cout << "SIGFPE";
		if(x < 0) return numeric_limits<T>::min();
		if(x == 0) return 1;
		return numeric_limits<T>::max();
	}
	// somehow this is wrong for shorts and chars, but ok for all bigger integral types
	if(numeric_limits<T>::is_signed && x == numeric_limits<T>::min() && y == -1) {
		std::cout << "SIGFPE";
		return numeric_limits<T>::max();
	}
	return x / y;
}

int main(){
	// The bad guys
	vector<T> v = {numeric_limits<T>::min(), -1, 0, 1, numeric_limits<T>::max()};

	for(auto&&x : v){
		std::cout << x << " ";
	}
	std::cout << std::endl;

	for(auto&& x : v){
		for(auto&& y : v){
			std::cout << x << " / " << y << " = " << std::flush;
			std::cout << myDiv(x, y) << std::endl;
		}
	}
}
