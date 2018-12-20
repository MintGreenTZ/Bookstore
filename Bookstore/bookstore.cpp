//
// Created by mintgreen on 18-12-17.
//

#include "bookstore.h"

bookstore::bookstore() :usr(), book(){
	
}

bookstore::~bookstore() {

}

void bookstore::run() {
	file.open("command.txt", std::fstream::in);
	if (file)
		file_mode();
	else
		friendly_mode();
}

void bookstore::file_mode() {
	std::string command;
	while (true) {
		try {
			std::getline(file, command);
			processLine(command);
		}
		catch (ErrorException &ex) {
			std::cerr << "Error: " << ex.getMessage() << std::endl;
		}
	}
}

void bookstore::friendly_mode() {	
	
}

std::vector<std::string> bookstore::split(std::string &Line) {
	std::vector<std::string> ret;
	std::string str, order[4] = {"-ISBN", "-name", "-author", "-keyword"};
	int price;
	stream << Line;
	stream >> str;
	ret.push_back(str);
	for (int i = 0; i < 4; i++)
		ret.push_back(emptyStr);
	price = emptyInt;
	int cnt = 1;
	if (ret[0] == "modify" || ret[0] == "show") {
		while (std::getline(stream, str, '=')) {
			int p = 0;
			while (p < str.length() && str[p] == ' ') p++;
			str = str.substr(p);
			if (str == "") break;
			if (str == "-price") {
				if (!(std::cin >> price)) error("Invalid");
			}
			else {
				bool suc = 0;
				for (int i = 0; i < 4; i++)
					if (str == order[i])
						std::getline(stream, ret[i + 1], '"'),
						std::getline(stream, ret[i + 1], '"'),
						suc = 1;
				if (!suc) error("Invalid");
			}
		}
	}
	else {
		while (stream >> ret[cnt++]);
	}
}

void bookstore::processLine(std::string &Line) {
	std::vector<std::string> &&sp = split(Line);
	int p = 0;
	while (p < totCommand && Command[p] != sp[0]) p++;
	try {
		if (p == totCommand) error("Invalid");
		switch (p) {
			case 0: if (sp.size() != 3) error("Invalid"); usr.log(sp[1], sp[2]);  break;
			case 1: if (sp.size() != 1) error("Invalid"); usr.logout(); break;
			case 2: if (sp.size() != 5) error("Invalid"); usr.useradd(sp[1], sp[2], stringTo<int>(sp[3]), sp[4]); break;
			case 3: if (sp.size() != 4) error("Invalid"); usr.Register(sp[1], sp[2], sp[3]); break;
			case 4: if (sp.size() != 2) error("Invalid"); usr.del(sp[1]); break;
			case 5: if (sp.size() == 3) usr.modifyPassword(sp[1], sp[2]);
					else if (sp.size() == 4)  usr.modifyPassword(sp[1], sp[2], sp[3]);
					else error("Invalid"); break;
			case 6: if (sp.size() != 2) error("Invalid"); book.select(sp[1]); break;
			case 7: if (sp.size() != 5) error("Invalid"); book.modify(record(sp[1], sp[2], sp[3], sp[4], price)); break;
			case 8: if (sp.size() != 3) error("Invalid"); book.import(stringTo<int>(sp[1]), stringTo<double>(sp[2])); break;
			case 9: if (sp.size() == 5) book.show(record(sp[1], sp[2], sp[3], sp[4], price));
					else if (sp[1] == "finance" && sp.size() == 2) book.showFinance();
					else if (sp[1] == "finance" && sp.size() == 3) book.showFinance(stringTo<int>(sp[2]));
					else error("Invalid"); break;
			case 10: if (sp.size() != 3) error("Invalid"); book.buy(sp[1], stringTo<int>(sp[2]));
		}
	}
	catch (ErrorException &ex) {
		std::string str;
		while (stream >> str);
		std::cout << ex.getMessage() << std::endl;
	}
	
}