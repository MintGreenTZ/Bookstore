//
// Created by mintgreen on 18-12-18.
//

#include <vector>
#include <set>
#include "books.h"
#include "error.h"
#include "database.h"

books::books() :rec("mainDatabase"), nameDB("nameDatabase"), authorDB("nameDatabase"), keyDB("keyDatabase") {
	this->ISBN = "";
}

books::~books () {}

void books::select(std::string ISBN) {
	rec.ask(record(ISBN));
	this->ISBN = ISBN;
}

void books::modify(record data) {
	if (ISBN == (standString)"") error("Invalid");

	record old = rec.ask(standString(ISBN));
	rec.del(old);
	if (old.getName() != empty) nameDB.del(slice(old.getName(), old.getISBN()));
	if (old.getAuthor() != empty) authorDB.del(slice(old.getAuthor(), old.getISBN()));
	if (old.getKeyword() != empty) {
		std::vector<std::string> &&v = split(old.getKeyword());
		for (int i = 0; i < v.size(); i++)
			keyDB.del(slice(v[i], old.getISBN()));
	}
	record cur = old;
	cur.update(data);
	rec.add(cur);
	if (cur.getName() != empty) nameDB.add(slice(cur.getName(), cur.getISBN()));
	if (cur.getAuthor() != empty) authorDB.add(slice(cur.getAuthor(), cur.getISBN()));
	if (cur.getKeyword() != empty) {
		std::vector<std::string> &&v = split(cur.getKeyword());
		for (int i = 0; i < v.size(); i++)
			keyDB.add(slice(v[i], cur.getISBN()));
	}
}

void books::import(int quantity, int cost) {
	rec.trade(record(ISBN), quantity);
	Finance.outcome(cost);
}

void books::buy(std::string ISBN, int quantity) {
	rec.trade(record(ISBN), -quantity);
	Finance.income(rec.ask(record(ISBN)).getPrice() * quantity);
}

void books::showFinance(int time) {
	Finance.show(time);
}

void books::show(record data) {
	bool picked = 0;
	std::vector<standString> v[4];
	int cnt = 0;
	if (data.getISBN() != empty)
		v[cnt++] = rec.giveMeAll(rec.ask(record(data.getISBN())));
	if (data.getName() != empty)
		v[cnt++] = rec.giveMeAll(data.getName());
	if (data.getAuthor() != empty)
		v[cnt++] = rec.giveMeAll(data.getAuthor());
	if (data.getKeyword() != empty)
		v[cnt++] = rec.giveMeAll(data.getKeyword());
	if (!cnt) error("Invalid");
	int p[4] = { 0, 0, 0, 0 };
	bool carry_on = 1, same;
	standString Min;
	for (int i = 0; i < cnt; i++)
		carry_on &= p[i] < v[i].size();
	while (carry_on) {
		Min = v[0][p[0]];
		for (int i = 1; i < cnt; i++)
			if (v[i][p[i]] < Min)
				Min = v[i][p[i]];
		same = 1;
		for (int i = 1; same && i < cnt; i++)
			same &= Min < v[i][p[i]];
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
			carry_on &= p[i] < v[i].size();
	}
}

std::vector<std::string> books::split(const standString &obj) {
	std::string str = obj.toString();
	std::vector<std::string> ret;
	for (int i = 0; i < str.length(); i++)
		if (str[i] == '|')
			str[i] = ' ';
	stream << str;
	while (stream >> str)
		ret.push_back(str);
	return ret;
}