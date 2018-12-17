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

template <class T>
class listData {
	T first;
    int pre, next, size;
public:
	listData() {};
	~listData() {};
	int& getNext() { return next; };
	int& getPre() { return pre; };
	int& getSize() { return size; };
	T &getFirst() { return first; };
};

template <class T>
class list {
    listData<T> data[listLength];
public:
	list() {};
	~list() {};
	listData<T> &operator[](int i) { return data[i]; };
};

template<class T>
class block {
	T data[blockSize * 2];
	int size = 0;
public:
	block() {};
	~block() {};
	T &operator[](int i) { return data[i]; };
	int &getSize() { return size; };
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
	constexpr bool isSlice() { return true; }

};

template <class T, class U> bool isSameType(const U &a, const T &b) { return false; }
template <class T> bool isSameType(const T &a, const T &b) { return true; }

bool operator < (const slice &a, const slice &b);
bool operator == (const slice &a, const slice &b);
bool operator <= (const slice &a, const slice &b);

class record {
	standString ISBN, name, author, keyword;
	int quantity = 0;
	double price = 0;
public:
	record();
	record(standString ISBN);
	~record();
	record update(record data);
	const standString &getISBN() const;
};
bool operator < (const record &a, const record &b);
bool operator == (const record &a, const record &b);
bool operator <= (const record &a, const record &b);

class bin {
	int n, a[listLength];
public:
	bin();
	~bin();
	void add(int x);
	int get();
};
#endif //BOOKSTORE_TOOLS_H
