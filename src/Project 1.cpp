//============================================================================
// Name        : Project.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "Calculator.h"
#include <iostream>

using namespace std;

int main() {
	bool quitProgram = false;
	bool quitNewExpression = false;

	vector<string> previousExpressions;
	vector<string> previousAnswers;

	while (!quitProgram) {
		int option = 0;

		cout << "Please select an option:\n1: Compute a new expression\n2: Help\n3: Review Previous Expressions and Answers\n4: Quit" << endl;
		cin >> option;

		if (option == 1) {
			quitNewExpression = false;
			while (!quitNewExpression) {
				string innerOption = "";
				cout << "Please enter an equation to simplify or enter Q to quit:" << endl;
				cin >> innerOption;

				if (innerOption == "Q") {
					quitNewExpression = true;
				} else {
					int location = innerOption.find("ans(");
					if (location >= 0) {
						vector<int> ansLocations;
						ansLocations.push_back(location);

						size_t start_pos = 0;
						while((start_pos = innerOption.find("ans(", start_pos)) != std::string::npos) {
							string answerNum = innerOption;
							location = innerOption.find(")", start_pos);
							answerNum.substr(start_pos + 4, location - start_pos + 4);
							int answer = atoi(answerNum.c_str());
							innerOption.replace(start_pos, 4, previousAnswers.at(answer));
							start_pos += previousAnswers.at(answer).length();
						}
					}

					Calculator * calc = new Calculator();
					previousExpressions.push_back(innerOption);
					previousAnswers.push_back(calc->SimplifyExpression(innerOption));
					cout << "\n" << previousAnswers.at(previousAnswers.capacity())  << "\n" << endl;
				}
			}
		} else if (option == 2) {
			cout << "Welcome to the help page! This program has several menu items starting with the \"Compute a new expression\" item which will allow you to simply enter an equation such as 1+3*8^(5/3) to receive a simplified answer. To access a previous answer and use it in the expression, enter \"ans(n)\" where \"n\" is replaced by the number of how many answers ago it was given. For example, the previous answer would be represented by \"ans(1)\" and the answer from the third previous question would be \"ans(3)\". To ensure an accurate entry, please always enter the asterisk (*) whenever multiplying and use parentheses to ensure correct understanding such as 1/2*3 is different from 1/(2*3).\nThe next non-help item, \"Review Previous Expressions and Answers\" allows you to view an of the expressions you have previously entered since opening this application and their respective answers.\nThe last menu item will allow you to exit the application." << endl;
			cout << "\nPress Enter to continue...";
			cin;
		} else if (option == 3) {
			for (int i = previousExpressions.capacity(); i > 0; i--) {
				cout << "Expression (" << previousExpressions.capacity() - i << "): " << previousExpressions.at(i) << "\n" << endl;
				cout << "\nPress Enter to continue...";
				cin;
			}
		} else if (option == 4) {
			quitProgram = true;
		}
	}

	return 0;
}
