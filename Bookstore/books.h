//
// Created by mintgreen on 18-12-18.
//

#ifndef BOOKSTORE_BOOKS_H
#define BOOKSTORE_BOOKS_H

#include <string>
#include <iostream>
#include "tools.h"
#include "error.h"
#include "database.h"
#include "finance.h"

class books {
	database<record> rec;
	database<slice> nameDB, authorDB, keyDB;
	standString ISBN;
	finance Finance;
public:
	books();
	~books();
	void select(std::string ISBN);
	void modify(record data);
	void import(int, double);
	void buy(std::string, int);
	void showFinance(int time = -1);
	void show(record data);
	std::vector<std::string> split(const standString &obj);
};

#endif //BOOKSTORE_BOOKS_H
