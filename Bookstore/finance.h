//
// Created by mintgreen on 18-12-17.
//

#ifndef BOOKSTORE_FINANCE_H
#define BOOKSTORE_FINANCE_H

#include <iostream>
#include <fstream>

class financeSlice {
	int income, outcome;
public:
	financeSlice();
	financeSlice(int Income, int Outcome);
	~financeSlice();
	int& getIncome();
	int& getOutcome();
};
financeSlice operator - (financeSlice &a, financeSlice &b) {
	return financeSlice(a.getIncome() - b.getIncome(), a.getOutcome() - b.getOutcome());
}

class finance {
	int n;
	financeSlice cur;
	std::fstream file;
public:
	finance();
	~finance();
	void income(int x);
	void outcome(int x);
	void show(int x = -1);

};
#endif //BOOKSTORE_FINANCE_H
