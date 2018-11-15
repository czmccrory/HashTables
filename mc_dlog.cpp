#include <iostream>
#include <sstream>
#include <ctime>
#include <random>
#include "HashTable.h"

using namespace std;

/* Calculate (g^r) % n */
ulint modexp(ulint g, ulint r, ulint n) {
	ulint result = 1; // initialise result

	if(r == 0) {
		return 1;
	}

	g = g % n; // update g if g is greater than n

	while (r > 1) {
		if(r%2){ // if r is odd
			result = (result*g) % n; // multiply g with result
			
			r /= 2;
			g = (g*g) % n;
		}
		else { // r is even
			r /= 2;
			g = (g*g) % n;
		}
	}

	return (result * g) % n;
}

/* Generates a random number between min and max */
ulint random(ulint min, ulint max){
	static default_random_engine generator(static_cast<unsigned int>(time(0)));
	static uniform_int_distribution<ulint> random(min, max);
	ulint r = random(generator);
	return r;
}

ulint ordOfG(ulint g, ulint n) {
	HashTable Ord;

	for(ulint i = 0; i <= sqrt(n); i++) {
		ulint r = random(0, (n-1));
		ulint y = modexp(g, r, n);

	  	if(Ord.existingKey(y)) {
	  		if(r > Ord.getValue(y)){
	  			ulint val = r - Ord.getValue(y);
	  			return val;
	  		}
	  		else if (r < Ord.getValue(y)) {
	  			ulint val = Ord.getValue(y) - r;
	  			return val;
	  		}
	  	}
	  	else {
	  		Ord.insert(y,r);
	  	}
	}

	return n-1;
}

ulint discreteLog(ulint g, ulint a, ulint n) {
	HashTable A, B;
	ulint orderG = ordOfG(g,n);

	for(ulint i = 0; i <= sqrt(n); i++) {
		ulint r = random(0, (n-1));
		ulint y = (a * modexp(g, r, n)) % n;
		ulint result, bVal, aVal;

		if(B.existingKey(y)) {
			bVal = B.getValue(y);
			if(r > bVal) {
				result = (orderG + bVal) - r;
			} else {
				result = (bVal-r);
			}

			return result;

		} else {
			try {
				A.insert(y, r);
			}catch(...) {}
		}
	
		r = random(0, (n-1));
		y = modexp(g, r, n);

		if(A.existingKey(y)) {
			aVal = A.getValue(y);
			if(aVal > r) {
				result = (r + orderG) - aVal;
			} else {
				result = (r-aVal);
			}

			return result;

		} else {
			try {
				B.insert(y, r);
			}catch(...){}
		}
	}

	return 0;
}

ulint result(ulint g, ulint a, ulint n) {
	ulint res, dLog, orderG;

	orderG = static_cast<long>(ordOfG(g, n));
	dLog = discreteLog(g, a, n);
	res = dLog % orderG;

	return res;
}

int main(int argc, char** argv) {
	ulint g, a, n;

	if(argc != 4){
		cout << "Wrong amount of arguments - please try again." << endl;
		return 1;
	}
	
		stringstream(argv[1]) >> g;
		stringstream(argv[2]) >> a;
		stringstream(argv[3]) >> n;

		ordOfG(g, n);
		discreteLog(g, a, n);
		result(g,a,n);

		cout << "Result with g = " << g << ", a = " << a << ", and n = " << n << " is: " << (result(g,a,n)) << endl;

	return 0;
}