//
// Created by mintgreen on 18-12-15.
//

#ifndef BOOKSTORE_MAINDATABASE_H
#define BOOKSTORE_MAINDATABASE_H

#include <string>
#include "tools.h"

class mainDatabase {
	const int listBegin = sizeof(int);
	const int binBegin = listBegin + sizeof(list);
	const int blockBegin = binBegin + sizeof(bin);
	int blockCnt;
	list List;
	bin Bin;
	detailBlock Block;
public:
	mainDatabase();
	~mainDatabase();
	void add(std::string ISBN);
	void modify(record data);
};
#endif //BOOKSTORE_MAINDATABASE_H