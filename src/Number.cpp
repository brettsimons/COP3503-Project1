
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

int Number::gcd(std::vector<int>& numList) {
	int num1 = 0;

	for (int i = 0; i < numList.size(); i++) {
		if (i == 0) {
			num1 = numList[i];
		} else if (numList[i] < num1) {
			num1 = numList[i];
		}
	}

	int gcd = 1;

	for (int i = 2; i < num1; i++) {
		bool isGCD = false;

		for (int x = 0; x < numList.size(); x++) {
			if (numList[x] % i == 0) {
				isGCD = true;
			} else {
				isGCD = false;
				x = numList.size();
			}
		}

		if (isGCD) {
			gcd = i;
		}
	}

	return gcd;
}

