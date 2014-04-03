
#include "Number.h"

Number::Number() {
	// TODO Auto-generated constructor stub

}

Number::~Number() {
	// TODO Auto-generated destructor stub
}

template<class T1, class T2>
std::string Number::lcm(T1 denom1, T2 denom2) {

}

int Number::gcd(int num1, int num2) {
	if (num1 > num2) {
		int temp = num1;
		num1 = num2;
		num2 = temp;
	}

	int gcd = 1;

	for (int i = 2; i < num1; i++) {
		if (num1 % i == 0 && num2 % i == 0) {
			gcd = i;
		}
	}

	return gcd;
}
