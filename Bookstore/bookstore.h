//
// Created by mintgreen on 18-12-17.
//

#ifndef BOOKSTORE_BOOKSTORE_H
#define BOOKSTORE_BOOKSTORE_H

#include <string>
#include <iostream>
#include "tools.h"
#include "database.h"
#include "user.h"
#include "books.h"

const int totCommand = 11;
const std::string Command[totCommand] = {"su", "logout", "useradd", "register", "delete", "passwd",
										 "select", "modify", "import", "show", "buy"};
class bookstore {
	user usr;
	books book;
	std::fstream file;
	std::iostream stream;
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
