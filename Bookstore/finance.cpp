//
// Created by mintgreen on 20-12-17.
//

#include "finance.h"

finance::finance() {
	file.open("finance", std::fstream::in);
	if (file) {
		file.open("finance", std::fstream::in | std::fstream::out | std::fstream::binary);
		file.seekg(0);
		file.read(reinterpret_cast<char *> (&n), sizeof(int));
		file.seekg(sizeof(int) + sizeof(financeSlice) * (n - 1));
		file.read(reinterpret_cast<char *> (&cur), sizeof(financeSlice));
	}
	else {
		std::ofstream ost("finance");
		ost.close();
		file.open("finance", std::fstream::in | std::fstream::out | std::fstream::binary);
		n = 1;
		cur = financeSlice();
		file.seekp(0);
		file.write(reinterpret_cast<const char *> (&n), sizeof(int));
		file.write(reinterpret_cast<const char *> (&cur), sizeof(financeSlice));
	}
}

finance::~finance() {
	file.close();
}

void finance::income(double x){
	cur.getIncome() += x;
	file.seekp(sizeof(int) + sizeof(financeSlice) * n);
	file.write(reinterpret_cast<const char *> (&cur), sizeof(financeSlice));
	n++;
}

void finance::outcome(double x) {
	cur.getOutcome() += x;
	file.seekp(sizeof(int) + sizeof(financeSlice) * n);
	file.write(reinterpret_cast<const char *> (&cur), sizeof(financeSlice));
	n++;
}

void finance::show(int x) {
	if (x == -1 || x > n - 1) x = n - 1;
	financeSlice prt;
	file.seekg(sizeof(int) + sizeof(financeSlice) * (n - x - 1));
	file.read(reinterpret_cast<char *> (&prt), sizeof(financeSlice));
	prt = cur - prt;
	printf("+ %.2f - %.2f\n", prt.getIncome(), prt.getOutcome());
}

financeSlice::financeSlice() :income(0), outcome(0) {}

financeSlice::financeSlice(double Income, double Outcome): income(Income), outcome(Outcome) {}

financeSlice::~financeSlice() {}

double& financeSlice::getIncome() { return income; }

double& financeSlice::getOutcome() { return outcome; }

financeSlice operator - (financeSlice &a, financeSlice &b) {
	return financeSlice(a.getIncome() - b.getIncome(), a.getOutcome() - b.getOutcome());
}