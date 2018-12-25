//
// Created by mintgreen on 18-12-17.
//

//the top organzation focused on communication with users

#ifndef BOOKSTORE_BOOKSTORE_H
#define BOOKSTORE_BOOKSTORE_H

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <random>
#include "windows.h"
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
	void greet();
	void friendly_mode();
	//spilit command according to space (modify and show command otherwise)
	std::vector<std::string> split(std::string &Line);
	void processLine(std::string &Line);
	void processLineFriendly(std::string &Line);
};

#endif //BOOKSTORE_BOOKSTORE_H
