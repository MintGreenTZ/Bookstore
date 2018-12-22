//
// Created by mintgreen on 18-12-18.
//

//a class store the current book, organize the data base and deal with the commands associated with books

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
	void select(std::string ISBN, int permission);
	void modify(record data, int permission);
	void import(int quantity, double cost_price, int permission);
	void buy(std::string ISBN, int quantity, int permission);
	void showFinance(int permission, int time = -1);
	void show(record data, int permission);
	std::vector<std::string> split(const standString &obj);
};

#endif //BOOKSTORE_BOOKS_H
