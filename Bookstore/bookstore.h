//
// Created by mintgreen on 18-12-17.
//

#ifndef BOOKSTORE_BOOKSTORE_H
#define BOOKSTORE_BOOKSTORE_H

#include <string>
#include <iostream>
#include <fstream>
#include "tools.h"
#include "database.h"
#include "user.h"
#include "books.h"

class bookstore {
	user usr;
	books book;
	std::fstream file;
	double price;
public:
	bookstore();
	~bookstore();
	void run();
	void file_mode();
	void friendly_mode();
	std::vector<std::string> split(std::string &Line);
	void processLine(std::string &Line);
};

#endif //BOOKSTORE_BOOKSTORE_H
