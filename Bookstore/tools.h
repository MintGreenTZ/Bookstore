//
// Created by mintgreen on 18-12-14.
//

//small basic classes to support the running of database

#ifndef BOOKSTORE_TOOLS_H
#define BOOKSTORE_TOOLS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include "error.h"

const int listLength = 200, blockSize = 200;
const double eps = 1e-6;

//to judge whether two types are the same
template <class U, class V> bool isSameType(const U &a, const V &b) { return false; }
template <class U> bool isSameType(const U &a, const U &b) { return true; }

//a kind of fixed-length string which is made for the convinience of storage in file
class standString {
    char ch[45];
    int len;
public:
	standString(std::string str);
	standString();
    ~standString();
    const int getLen() const;
    char &operator[] (int i);
	const char &operator[] (int i) const;
    standString &operator= (std::string);
	std::string toString() const;
};
bool operator < (const standString &a, const standString &b);
bool operator == (const standString &a, const standString &b);
bool operator <= (const standString &a, const standString &b);
bool operator != (const standString &a, const standString &b);

//set of empty elements
const std::string emptyStr = "I took the one less traveled by,";
const standString empty = standString(emptyStr);
const double emptyDouble = -std::acos(-1);

//the elements of the list which stores the number of last block and next block, size of current block,
//	and the first element of the current block
template <class T>
class listData {
	T first;
    int pre, next, size;
public:
	listData() :pre(-1), next(-1), size(0){};
	~listData() {};
	int& getNext() { return next; };
	int& getPre() { return pre; };
	int& getSize() { return size; };
	T& getFirst() { return first; };
};

//the linked-list at the top of data base indicates the concise infomation of the block
template <class T>
class list {
    listData<T> data[listLength];
public:
	list() {};
	~list() {};
	listData<T> &operator[](int i) { return data[i]; };
};

//the block of the "Block linked list" which supports different types of elements
template<class T>
class block {
	T data[blockSize * 2];
	int size;
public:
	block() :size(0) {};
	~block() {};
	T &operator[](int i) { return data[i]; };
	int &getSize() { return size; };
};

//the element to storage information associated with book
class record {
	standString ISBN, name, author, keyword;
	int quantity = 0;
	double price = 0;
public:
	record();
	record(standString _ISBN);
	record(std::string _ISBN, std::string _name, std::string _author, std::string _keyword, double _price);
	~record();
	void update(record &data);
	const standString& getISBN() const;
	const standString& getName() const;
	const standString& getAuthor() const;
	const standString& getKeyword() const;
	//the key word to compare (used for funtion:giveMeAll)
	const standString& getKey() const;
	int& getQuantity();
	double& getPrice();
	void print();
};
bool operator < (const record &a, const record &b);
bool operator == (const record &a, const record &b);
bool operator <= (const record &a, const record &b);

//the element to storage keyword and linked ISBN (multi-keyword comparison)
class slice {
	standString key, ISBN;
public:
	slice();
	slice(const std::string &_key, const std::string &_ISBN = "");
	slice(const standString &_key, const standString &_ISBN);
	slice(const standString &_key);
	slice(const record &data);
	~slice();
	//the key word to compare (used for funtion:giveMeAll)
	const standString &getKey() const;
	const standString &getISBN() const;
	//constexpr bool isSlice() { return true; }
};

bool operator < (const slice &a, const slice &b);
bool operator == (const slice &a, const slice &b);
bool operator <= (const slice &a, const slice &b);

//the element to storage information associated with users
class userInfo {
	standString ID, name, password;
	int permission;
public:
	userInfo();
	userInfo(std::string _ID);
	//another way: name -> oldPassword; password -> newPassword; permission -> privileged
	userInfo(std::string _ID, std::string _name, std::string _password, int _permission);
	//another way: name -> oldPassword; password -> newPassword; permission -> privileged
	userInfo(standString _ID, standString _name, standString _password, int _permission);
	~userInfo();
	const int getPermission() const;
	const standString getName() const;
	const standString getID() const;
	//the key word to compare (used for funtion:giveMeAll)
	const standString getKey() const;
	bool check(standString);
	bool check(std::string);
	void update(userInfo data);
};
bool operator < (const userInfo &a, const userInfo &b);
bool operator == (const userInfo &a, const userInfo &b);
bool operator <= (const userInfo &a, const userInfo &b);

//a recycle system to reuse the abandoned block due to the delete operation
class bin {
	int n, a[listLength];
public:
	bin();
	~bin();
	void add(int x);
	const int get();
};

//easy transformation from string to other types
template <class T>
T stringTo(std::string str) {
	std::stringstream ss(str);
	T x;
	if (!(ss >> x)) error("Invalid");
	return x;
}

#endif //BOOKSTORE_TOOLS_H
