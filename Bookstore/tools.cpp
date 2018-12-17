//
// Created by mintgreen on 18-12-14.
//

#include <algorithm>
#include <string>
#include "tools.h"

std::fstream file;

//******************************************************* standString

standString::standString() {
    ch[0] = '\0';
}

standString::standString(std::string str) {
    len = str.length();
    for (int i = 0; i < len; i++)
        ch[i] = str[i];
    ch[len] = '\0';
}

standString::~standString() {};

char& standString::operator[](int i) {return ch[i];}

const int standString::getLen() const {return len;}

bool operator < (standString &a, standString &b) {
    int lena = a.getLen(), lenb = b.getLen();
    for (int i = 0; i < std::min(lena, lenb); i++) {
        if (a[i] < b[i]) return true;
        if (a[i] > b[i]) return false;
    }
    return lena < lenb;
}

bool operator == (standString &a, standString &b) {
	int lena = a.getLen(), lenb = b.getLen();
	if (lena != lenb) return false;
	for (int i = 0; i < lena; i++)
		if (a[i] != b[i]) return false;
	return true;
}

bool operator != (standString &a, standString &b) {
	return !(a == b);
}

standString& standString::operator=(std::string str) {
    len = str.length();
    for (int i = 0; i < len; i++)
        ch[i] = str[i];
    ch[len] = '\0';
    return *this;
}

std::string standString::toString() {
	std::string ret = "";
	for (int i = 0; i < len; i++)
		ret = ret + ch[i];
	return ret;
}

//******************************************************* listData

listData::listData() {};

listData::~listData() {};

int& listData::getNext() {return next;}

int& listData::getPre() { return pre; }

int& listData::getSize() { return size; }

slice& listData::getFirst() { return first; };

//******************************************************* list

list::list() {};

list::~list() {};

listData& list::operator[](int i) { return data[i]; }

//******************************************************* slice

slice::slice() {};

slice::slice(std::string key, std::string ISBN = "") {
	this->key = key;
	this->ISBN = ISBN;
}

slice::slice(standString key) {
	this->key = key;
	this->ISBN = "";
}

slice::slice(record data) {
	this->key = data.getISBN();
	this->ISBN = "";
}

slice::~slice() {};

bool operator < (const slice &a, const slice &b) {
	return (a.getKey() < b.getKey()) || (a.getKey() == b.getKey()) && (a.getISBN() < b.getISBN());
}

bool operator == (const slice &a, const slice &b) {
	return (a.getKey() == b.getKey) && (a.getISBN() == b.getISBN());
}

bool operator <= (const slice &a, const slice &b) {
	return (a < b) || (a == b);
}

const standString& slice::getKey() const { return key; }

const standString& slice::getISBN() const { return ISBN; }

//******************************************************* block

block::block() {}

block::~block() {}

slice& block::operator[] (int i) { return data[i]; }

int& block::getSize() { return size; }

//******************************************************* record

record::record(standString ISBN) {
	this->ISBN = ISBN;
}

record record::update(record data) {
	if (data.ISBN != empty) this->ISBN = data.ISBN;
	if (data.name != empty) this->name = data.name;
	if (data.author != empty) this->author = data.author;
	if (data.keyword != empty) this->keyword = data.keyword;
	if (std::abs(data.price + emptyInt) > eps) this->price = data.price;
}

record::~record() {};

bool operator < (const record &a, const record &b) {
	return a.getISBN() < b.getISBN();
}

bool operator == (const record &a, const record &b) {
	return a.getISBN() == b.getISBN();
}

bool operator <= (const record &a, const record &b) {
	return (a < b) || (a == b);
}

const standString& record::getISBN() const { return ISBN; }

//******************************************************* detailBlock

detailBlock::detailBlock() {}

detailBlock::~detailBlock() {}

record& detailBlock::operator[] (int i) { return data[i]; }

int& detailBlock::getSize() { return size; }

//******************************************************* bin

bin::bin() {
	int n = listLength;
	for (int i = 0; i < n; i++)
		a[i] = n - 1 - i;
}

bin::~bin() {}

void bin::add(int x) {
	a[n++] = x;
}

int bin::get() {
	return a[--n];
}