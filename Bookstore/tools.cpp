//
// Created by mintgreen on 18-12-14.
//

#include <algorithm>
#include <string>
#include "tools.h"

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

char& standString::operator[] (int i) { return ch[i]; }

const char& standString::operator[] (int i) const { return ch[i]; }

const int standString::getLen() const {return len;}

bool operator < (const standString &a, const standString &b) {
    int lena = a.getLen(), lenb = b.getLen();
    for (int i = 0; i < std::min(lena, lenb); i++) {
        if (a[i] < b[i]) return true;
        if (a[i] > b[i]) return false;
    }
    return lena < lenb;
}

bool operator == (const standString &a, const standString &b) {
	int lena = a.getLen(), lenb = b.getLen();
	if (lena != lenb) return false;
	for (int i = 0; i < lena; i++)
		if (a[i] != b[i]) return false;
	return true;
}

bool operator <= (const standString &a, const standString &b) {
	return a < b || a == b;
}

bool operator != (const standString &a, const standString &b) {
	return !(a == b);
}

standString& standString::operator=(std::string str) {
    len = str.length();
    for (int i = 0; i < len; i++)
        ch[i] = str[i];
    ch[len] = '\0';
    return *this;
}

std::string standString::toString() const{
	std::string ret = "";
	for (int i = 0; i < len; i++)
		ret = ret + ch[i];
	return ret;
}

//******************************************************* slice

slice::slice() {
	this->key = "";
	this->ISBN = "";
};

slice::slice(std::string _key, std::string _ISBN) :key(_key), ISBN(_ISBN){}

slice::slice(const standString &_key, const standString &_ISBN) : key(_key), ISBN(_ISBN) {}

slice::slice(standString _key) :key(_key), ISBN("") {}

slice::slice(record data) :key(data.getISBN()), ISBN(""){}

slice::~slice() {};

bool operator < (const slice &a, const slice &b) {
	return (a.getKey() < b.getKey()) || (a.getKey() == b.getKey()) && (a.getISBN() < b.getISBN());
}

bool operator == (const slice &a, const slice &b) {
	return (a.getKey() == b.getKey()) && (a.getISBN() == b.getISBN());
}

bool operator <= (const slice &a, const slice &b) {
	return (a < b) || (a == b);
}

const standString& slice::getKey() const { return key; }

const standString& slice::getISBN() const { return ISBN; }

//******************************************************* record

record::record() {
	this->ISBN = "";
}

record::record(standString ISBN) {
	this->ISBN = ISBN;
}

record::record(std::string _ISBN, std::string _name, std::string _author, std::string _keyword, double _price) :
	ISBN(_ISBN), name(_name), author(_author), keyword(_keyword), price(_price) {};

void record::update(record &data) {
	if (data.ISBN != empty) this->ISBN = data.ISBN;
	if (data.name != empty) this->name = data.name;
	if (data.author != empty) this->author = data.author;
	if (data.keyword != empty) this->keyword = data.keyword;
	if (std::abs(data.price + emptyDouble) > eps) this->price = data.price;
}

record::~record() {};

bool operator < (const record &a, const record &b) {
	return a.getISBN() < b.getISBN();
}

bool operator == (const record &a, const record &b) {
	return a.getISBN() == b.getISBN();
}

bool operator <= (const record &a, const record &b) {
	return a.getISBN() <= b.getISBN();
}

const standString& record::getISBN() const { return ISBN; }

const standString& record::getName() { return name; }

const standString& record::getAuthor() { return author; }

const standString& record::getKeyword() { return keyword; }

int& record::getQuantity() { return quantity; }

double& record::getPrice() { return price; }

void record::print() {
	std::cout << ISBN.toString();
	if (name != empty) std::cout << ' ' << name.toString();
	if (author != empty) std::cout << ' ' << author.toString();
	if (price != emptyDouble) printf(" %.2f", price);
	printf("%d±¾\n", quantity);
}

//******************************************************* userInfo

userInfo::userInfo() {}

userInfo::userInfo(std::string name) {
	this->name = name;
};

//another way: name -> oldPassword; password -> newPassword; permission -> privileged
userInfo::userInfo(std::string ID, std::string name, std::string password, int permission) {
	this->ID = ID;
	this->name = name;
	this->password = password;
	this->permission = permission;
}

userInfo::userInfo(standString ID, standString name, standString password, int permission) {
	this->ID = ID;
	this->name = name;
	this->password = password;
	this->permission = permission;
}

userInfo::~userInfo() {}

const int userInfo::getPermission() const { return permission; }

const standString userInfo::getName() const { return name; }

const standString userInfo::getID() const { return ID; }

bool userInfo::check(std::string x) {
	return standString(x) == password;
}

bool userInfo::check(standString x) {
	return x == password;
}

//another way: name -> oldPassword; password -> newPassword; permission -> privileged
void userInfo::update(userInfo data) {
	if (!data.getPermission() && !check(data.getName())) error("Invalid");
	password = data.password;
}

bool operator < (const userInfo &a, const userInfo &b) {
	return a.getName() < b.getName();
}

bool operator == (const userInfo &a, const userInfo &b) {
	return a.getName() == b.getName();
}

bool operator <= (const userInfo &a, const userInfo &b) {
	return a.getName() <= b.getName();
}

//******************************************************* bin

bin::bin() :n(listLength){
	for (int i = 0; i < n; i++)
		a[i] = n - 1 - i;
}

bin::~bin() {}

void bin::add(int x) {
	a[n++] = x;
}

const int bin::get() {
	return a[--n];
}
