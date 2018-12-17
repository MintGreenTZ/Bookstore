//
// Created by mintgreen on 18-12-14.
//

#ifndef BOOKSTORE_TOOLS_H
#define BOOKSTORE_TOOLS_H

#include <iostream>
#include <fstream>

const int listLength = 100, blockSize = 1000;
const double eps = 1e-6;

class standString {
    char ch[45];
    int len;
public:
    standString();
    standString(std::string str);
    ~standString();
    const int getLen() const;
    char &operator[] (int i);
    standString &operator= (std::string);
	std::string toString();
};
bool operator < (const standString &a, const standString &b);
bool operator == (const standString &a, const standString &b);
bool operator != (const standString &a, const standString &b);

const standString empty = standString("I took the one less traveled by,");
const int emptyInt = -acos(-1);

class listData {
	slice first;
    int pre, next, size;
public:
    listData();
    ~listData();
    int& getNext();
	int& getPre();
	int& getSize();
    slice &getFirst();
};

class list {
    listData data[listLength];
public:
    list();
    ~list();
    listData &operator[](int i);
};

class slice {
	standString key, ISBN;
public:
	slice();
	slice(std::string key, std::string ISBN = "");
	slice(standString key);
	slice(record data);
	~slice();
	const standString &getKey() const;
	const standString &getISBN() const;
};
bool operator < (const slice &a, const slice &b);
bool operator == (const slice &a, const slice &b);
bool operator <= (const slice &a, const slice &b);

class block {
	slice data[blockSize * 2];
	int size = 0;
public:
	block();
    ~block();
	slice &operator[](int i);
	int &getSize();
};

class record {
	standString ISBN, name, author, keyword;
	int quantity = 0;
	double price = 0;
public:
	record(standString ISBN);
	~record();
	record update(record data);
	const standString &getISBN() const;
};

class detailBlock {
	record data[blockSize * 2];
	int size = 0;
public:
	detailBlock();
	~detailBlock();
	record &operator[](int);
	int &getSize();
};

class bin {
	int n, a[listLength];
public:
	bin();
	~bin();
	void add(int x);
	int get();
};
#endif //BOOKSTORE_TOOLS_H
