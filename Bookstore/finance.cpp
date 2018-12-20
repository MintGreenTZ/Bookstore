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
		file.open("finance", std::fstream::in | std::fstream::out | std::fstream::binary);
		n = 1;
		cur = financeSlice();
		file.write(reinterpret_cast<const char *> (&n), sizeof(int));
		file.write(reinterpret_cast<const char *> (&cur), sizeof(financeSlice));
	}
}

finance::~finance() {
	file.close();
}

void finance::income(int x){
	cur.getIncome() += x;
	file.seekp(sizeof(int) + sizeof(financeSlice) * n);
	file.write(reinterpret_cast<const char *> (&cur), sizeof(financeSlice));
	n++;
}

void finance::outcome(int x) {
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

financeSlice::financeSlice(int Income, int Outcome): income(Income), outcome(Outcome) {}

financeSlice::~financeSlice() {}

int& financeSlice::getIncome() { return income; }

int& financeSlice::getOutcome() { return outcome; }