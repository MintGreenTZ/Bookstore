//
// Created by mintgreen on 18-12-17.
//

//the finance log which is stored in the file
//support add to the end and ask suffix sum

#ifndef BOOKSTORE_FINANCE_H
#define BOOKSTORE_FINANCE_H

#include <iostream>
#include <fstream>

class financeSlice {
	double income, outcome;
public:
	financeSlice();
	financeSlice(double Income, double Outcome);
	~financeSlice();
	double& getIncome();
	double& getOutcome();
};
financeSlice operator - (financeSlice &a, financeSlice &b);

class finance {
	int n;
	financeSlice cur;
	std::fstream file;
public:
	finance();
	~finance();
	void income(double x);
	void outcome(double x);
	void show(int x = -1);

};
#endif //BOOKSTORE_FINANCE_H
