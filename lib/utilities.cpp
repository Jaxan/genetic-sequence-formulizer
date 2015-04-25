#include "utilities.hpp"

#include <limits>

using namespace std;

int divi(int x, int y){
	// clearly we don't want a division by zero
	if(!y) {
		if(x < 0) return numeric_limits<int>::min();
		if(x == 0) return 1;
		return numeric_limits<int>::max();
	}
	// in this situation there is a overflow which causes a SIGFPE
	if(x == numeric_limits<int>::min() && y == -1) {
		return numeric_limits<int>::max();
	}
	return x / y;
}


int modi(int x, int y){
	if(!y) return 0;
	if(x == numeric_limits<int>::min() && y == -1) return 0;
	return x % y;
}

// exponentiation by squaring
int powi(int base, int exp){
	if(exp < 0) return 0;
	int res = 1;
	while (exp) {
		if (exp & 1)
			res *= base;
		exp >>= 1;
		base *= base;
	}
	return res;
}
