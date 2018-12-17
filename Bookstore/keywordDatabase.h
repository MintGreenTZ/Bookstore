//
// Created by mintgreen on 18-12-14.
//

#ifndef BOOKSTORE_KEYWORDDATABASE_H
#define BOOKSTORE_KEYWORDDATABASE_H

#include <string>
#include "tools.h"

class keywordDatabase {
    const int listBegin = sizeof(int);
	const int binBegin = listBegin + sizeof(list);
    const int blockBegin = binBegin + sizeof(bin);
    int blockCnt;
	list List;
	bin Bin;
	block Block;
public:
    keywordDatabase (std::string key);
    ~keywordDatabase();
    void add(std::string key, std::string ISBN);
	void del(std::string key, std::string ISBN);
};
#endif //BOOKSTORE_KEYWORDDATABASE_H
