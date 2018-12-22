//
// Created by mintgreen on 18-12-18.
//

#include <vector>
#include <set>
#include <fstream>
#include <iostream>
#include <sstream>
#include "books.h"
#include "error.h"
#include "database.h"

books::books() :rec("mainDatabase"), nameDB("nameDatabase"), authorDB("authorDatabase"), keyDB("keyDatabase"), Finance(){
	std::fstream file;
	file.open("curISBN", std::fstream::in | std::fstream::out | std::fstream::binary);
	if (file) {
		file.seekg(0);
		file.read(reinterpret_cast<char *> (&ISBN), sizeof(standString));
	}
	else {
		std::ofstream ost("curISBN");
		ost.close();
		file.open("curISBN", std::fstream::in | std::fstream::out | std::fstream::binary);
		file.seekp(0);
		ISBN = "";
		file.write(reinterpret_cast<const char *> (&ISBN), sizeof(standString));
	}
}

books::~books () {}

void books::select(std::string ISBN, int permission) {
	if (permission < 3) error("Invalid");
	try {
		rec.ask(ISBN);
	}
	catch (ErrorException) {
		rec.add(record(ISBN));
	}
	this->ISBN = ISBN;
}

void books::modify(record data, int permission) {
	if (permission < 3 || ISBN == (standString)"") error("Invalid");

	record old = rec.ask(standString(ISBN));
	rec.del(old);
	if (old.getName() != empty) nameDB.del(slice(old.getName(), old.getISBN()));
	if (old.getAuthor() != empty) authorDB.del(slice(old.getAuthor(), old.getISBN()));
	if (old.getKeyword() != empty) {
		std::vector<std::string> &&v = split(old.getKeyword());
		for (int i = 0; i < (int)v.size(); i++)
			keyDB.del(slice(v[i], old.getISBN()));
	}
	record cur = old;
	cur.update(data);
	try {
		rec.add(cur);
	}
	catch (ErrorException) {
		rec.add(old);
		error("Invalid");
	}
	this->ISBN = cur.getISBN();
	if (cur.getName() != empty) nameDB.add(slice(cur.getName(), cur.getISBN()));
	if (cur.getAuthor() != empty) authorDB.add(slice(cur.getAuthor(), cur.getISBN()));
	if (cur.getKeyword() != empty) {
		std::vector<std::string> &&v = split(cur.getKeyword());
		for (int i = 0; i < (int)v.size(); i++)
			keyDB.add(slice(v[i], cur.getISBN()));
	}
}

void books::import(int quantity, double cost, int permission) {
	if (permission < 3 || ISBN == standString("")) error("Invalid");
	rec.trade(record(ISBN), quantity);
	Finance.outcome(cost);
}

void books::buy(std::string ISBN, int quantity, int permission) {
	if (permission < 1) error("Invalid");
	rec.trade(record(ISBN), -quantity);
	Finance.income(rec.ask(ISBN).getPrice() * quantity);
}

void books::showFinance(int permission, int time) {
	if (permission < 7) error("Invalid");
	Finance.show(time);
}

void books::show(record data, int permission) {
	if (permission < 1) error("Invalid");
	bool picked = 0;
	std::vector<standString> v[4];
	int cnt = 0;
	try {
		if (data.getISBN() != empty)
			v[cnt++] = rec.giveMeAll(rec.ask(data.getISBN()).getKey());
		if (data.getName() != empty)
			v[cnt++] = nameDB.giveMeAll(data.getName());
		if (data.getAuthor() != empty)
			v[cnt++] = authorDB.giveMeAll(data.getAuthor());
		if (data.getKeyword() != empty)
			v[cnt++] = keyDB.giveMeAll(data.getKeyword());
		if (!cnt) { rec.showAll(); return; }
	}
	catch (ErrorException) {
		return;
	}
	int p[4] = { 0, 0, 0, 0 };
	bool carry_on = 1, same;
	standString Min;
	for (int i = 0; i < cnt; i++)
		carry_on &= p[i] < (int)v[i].size();
	while (carry_on) {
		Min = v[0][p[0]];
		for (int i = 1; i < cnt; i++)
			if (v[i][p[i]] < Min)
				Min = v[i][p[i]];
		same = 1;
		for (int i = 1; same && i < cnt; i++)
			same &= Min == v[i][p[i]];
		if (same) {
			rec.ask(Min).print();
			for (int i = 0; i < cnt; i++)
				p[i]++;
		}
		else {
			for (int i = 0; i < cnt; i++)
				if (v[i][p[i]] == Min)
					p[i]++;
		}
		carry_on = 1;
		for (int i = 0; i < cnt; i++)
			carry_on &= p[i] < (int)v[i].size();
	}
}

std::vector<std::string> books::split(const standString &obj) {
	std::string str = obj.toString();
	std::vector<std::string> ret;
	for (int i = 0; i < (int)str.length(); i++)
		if (str[i] == '|')
			str[i] = ' ';
	std::stringstream ss(str);
	while (ss >> str)
		ret.push_back(str);
	return ret;
}