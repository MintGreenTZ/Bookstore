//
// Created by mintgreen on 18-12-17.
//

#include "bookstore.h"

const int totCommand = 14;
const std::string Command[totCommand] = { "su", "logout", "useradd", "register", "delete", "passwd",
										 "select", "modify", "import", "show", "buy", "exit", "help", "show finance"};
const int totPoetry = 4;
const std::string poetry[] = {
"Do not go gentle into that good night,\nOld age should burn and rave at close of day;\nRage, rage against the dying of the light.\n<Do not go gentle into that good night>\n--Dylan Thomas, 1914 - 1953\n",
"To see a world in a grain of sand,\nAnd a heaven in a wild flower,\nHold infinity in the palm of your hand,\nAnd eternity in an hour.\n",
"I love three things in this world.\nSun, Moon and You.\nSun for morning,\nMoon for night,\nand You forever.\n",
"If you were a teardrop\nin my eye,\nFor fear of losing you,\nI would never cry.\nAnd if the golden sun,\nShould cease to shine its light,\nJust one smile from you,\nWould make my whole world bright.\n"
};


bookstore::bookstore() :usr(), book(){
	
}

bookstore::~bookstore() {
	//std::cout << "finished" << std::endl;
	//Sleep(10000);
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

void bookstore::greet() {
	struct tm t;
	time_t now;
	time(&now);
	localtime_s(&t, &now);
	std::cout << "welcome to MintGreen's Bookstore system!" << std::endl;
	printf("current: %02d-%02d-%02d %02d:%02d:%02d\n",
		t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
	switch (t.tm_hour){
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		std::cout << "Protect your liver! The world is beautiful more than work." << std::endl; break;
	case 5:
	case 6:
		std::cout << "The early bird catches the worm." << std::endl;
	case 7:
	case 8:
	case 9:
	case 10:
		std::cout << "Good morning, and in case I don't see you, good afternoon, good evening, and good night!" << std::endl; break;
	case 11:
	case 12:
		std::cout << "Have a good lunch!" << std::endl; break;
	case 13:
	case 14:
		std::cout << "Good afternoon!" << std::endl; break;
	case 15:
	case 16:
	case 17:
		std::cout << "Good afternoon!" << std::endl;
		std::cout << "Why not have a cup of afternoon tea?" << std::endl; break;
	case 18:
		std::cout << "Have a good dinner!" << std::endl; break;
	case 19:
	case 20:
	case 21:
	case 22:
		std::cout << "Good evening!" << std::endl; break;
	case 23:
		std::cout << "年轻人不要老熬夜。今天做到12点，就不做了，睡大觉！	——习近平" << std::endl;  break;
	}
	srand(time(0));
	std::cout << std::endl << poetry[rand() % totPoetry] << std::endl;
}

void bookstore::friendly_mode() {
	greet();
	std::string command;
	while (true) {
		std::cout << usr.getName() << "> ";
		try {
			std::getline(std::cin, command);
			processLineFriendly(command);
		}
		catch (ErrorException &ex) {
			std::cerr << "Error: " << ex.getMessage() << std::endl;
		}
	}
}

std::vector<std::string> bookstore::split(std::string &Line) {
	std::vector<std::string> ret;
	std::string str, order[4] = {"-ISBN", "-name", "-author", "-keyword"};
	std::stringstream ss(Line);
	ss >> str;
	ret.push_back(str);
	price = emptyDouble;
	if (ret[0] == "modify" || ret[0] == "show" && Line.find("show finance") == std::string::npos) {
		for (int i = 0; i < 4; i++)
			ret.push_back(emptyStr);
		while (std::getline(ss, str, '=')) {
			int p = 0;
			while (p < (int)str.length() && str[p] == ' ') p++;
			str = str.substr(p);
			if (str == "") break;
			if (str == "-price") {
				if (!(ss >> price)) error("Invalid");
			}
			else {
				bool suc = 0;
				for (int i = 0; i < 4; i++)
					if (str == order[i])
						std::getline(ss, ret[i + 1], '"'),
						std::getline(ss, ret[i + 1], '"'),
						suc = 1;
				if (!suc) error("Invalid");
			}
		}
	}
	else {
		std::string str;
		while (ss >> str)
			ret.push_back(str);
	}
	return ret;
}

void bookstore::processLine(std::string &Line) {
	std::vector<std::string> &&sp = split(Line);
	int p = 0;
	while (p < totCommand && Command[p] != sp[0]) p++;
	try {
		if (p == totCommand) error("Invalid");
		switch (p) {
			case 0: 
				if (sp.size() == 2) usr.log(sp[1]);
				else if (sp.size() == 3) usr.log(sp[1], sp[2]);
				else error("Invalid"); break;
			case 1:
				if (sp.size() != 1) error("Invalid");
				usr.logout(); break;
			case 2:
				if (sp.size() != 5) error("Invalid");
				usr.useradd(sp[1], sp[2], stringTo<int>(sp[3]), sp[4]); break;
			case 3:
				if (sp.size() != 4) error("Invalid");
				usr.Register(sp[1], sp[2], sp[3]); break;
			case 4:
				if (sp.size() != 2) error("Invalid");
				usr.del(sp[1]); break;
			case 5:
				if (sp.size() == 3) usr.modifyPassword(sp[1], sp[2]);
				else if (sp.size() == 4)  usr.modifyPassword(sp[1], sp[3], sp[2]);
				else error("Invalid"); break;
			case 6:
				if (sp.size() != 2) error("Invalid");
				book.select(sp[1], usr.getPermission()); break;
			case 7:
				if (sp.size() != 5) error("Invalid");
				book.modify(record(sp[1], sp[2], sp[3], sp[4], price),usr.getPermission()); break;
			case 8:
				if (sp.size() != 3) error("Invalid");
				book.import(stringTo<int>(sp[1]), stringTo<double>(sp[2]), usr.getPermission()); break;
			case 9:
				if (sp.size() == 5) book.show(record(sp[1], sp[2], sp[3], sp[4], price), usr.getPermission());
				else if (sp[1] == "finance" && sp.size() == 2) book.showFinance(usr.getPermission());
				else if (sp[1] == "finance" && sp.size() == 3) book.showFinance(usr.getPermission(), stringTo<int>(sp[2]));
				else error("Invalid"); break;
			case 10:
				if (sp.size() != 3) error("Invalid");
				book.buy(sp[1], stringTo<int>(sp[2]), usr.getPermission()); break;
			case 11:
				exit(0);
			case 12:
				error("Invalid");
		}
	}
	catch (ErrorException &ex) {
		std::cout << ex.getMessage() << std::endl;
	}	
}

void bookstore::processLineFriendly(std::string &Line) {
	std::string sp[10];
	int p = 0;
	while (p < totCommand && Command[p] != Line) p++;
	try {
		if (p == totCommand) error("Invalid");
		switch (p) {
		case 0:
			std::cout << usr.getName() << "> user ID:";
			std::getline(std::cin, sp[1]);
			std::cout << usr.getName() << "> password:";
			std::getline(std::cin, sp[2]);
			usr.log(sp[1], sp[2]); break;
		case 1:
			usr.logout(); break;
		case 2:
			std::cout << usr.getName() << "> user ID:";
			std::getline(std::cin, sp[1]);
			std::cout << usr.getName() << "> password:";
			std::getline(std::cin, sp[2]);
			std::cout << usr.getName() << "> kind of user(3 for staff / 1 for customer):";
			std::getline(std::cin, sp[3]);
			std::cout << usr.getName() << "> name:";
			std::getline(std::cin, sp[4]);
			usr.useradd(sp[1], sp[2], stringTo<int>(sp[3]), sp[4]); break;
		case 3:
			std::cout << usr.getName() << "> user ID:";
			std::getline(std::cin, sp[1]);
			std::cout << usr.getName() << "> password:";
			std::getline(std::cin, sp[2]);
			std::cout << usr.getName() << "> name:";
			std::getline(std::cin, sp[3]);
			usr.Register(sp[1], sp[2], sp[3]); break;
		case 4:
			std::cout << usr.getName() << "> user ID:";
			std::getline(std::cin, sp[1]);
			usr.del(sp[1]); break;
		case 5:
			std::cout << usr.getName() << "> user ID:";
			std::getline(std::cin, sp[1]);
			if (usr.getPermission() == 7) {
				std::cout << usr.getName() << "> new password:";
				std::getline(std::cin, sp[2]);
				usr.modifyPassword(sp[1], sp[2]);
			}
			else {
				std::cout << usr.getName() << "> old password:";
				std::getline(std::cin, sp[3]);
				std::cout << usr.getName() << "> new password:";
				std::getline(std::cin, sp[2]);
				usr.modifyPassword(sp[1], sp[3], sp[2]);
			}
			break;
		case 6:
			std::cout << usr.getName() << "> ISBN:";
			std::getline(std::cin, sp[1]);
			book.select(sp[1], usr.getPermission()); break;
		case 7:
			std::cout << usr.getName() << "> ISBN:";
			std::getline(std::cin, sp[1]);
			if (sp[1].length() == 0) sp[1] = emptyStr;
			std::cout << usr.getName() << "> name:";
			std::getline(std::cin, sp[2]);
			if (sp[2].length() == 0) sp[2] = emptyStr;
			std::cout << usr.getName() << "> author:";
			std::getline(std::cin, sp[3]);
			if (sp[3].length() == 0) sp[3] = emptyStr;
			std::cout << usr.getName() << "> keyword:";
			std::getline(std::cin, sp[4]);
			if (sp[4].length() == 0) sp[4] = emptyStr;
			std::cout << usr.getName() << "> price:";
			std::getline(std::cin, sp[5]);
			if (sp[5].length() == 0)
				price = emptyDouble;
			else
				price = stringTo<double>(sp[5]);
			book.modify(record(sp[1], sp[2], sp[3], sp[4], price), usr.getPermission()); break;
		case 8:
			std::cout << usr.getName() << "> quantity:";
			std::getline(std::cin, sp[1]);
			std::cout << usr.getName() << "> cost_price(in total):";
			std::getline(std::cin, sp[1]);
			book.import(stringTo<int>(sp[1]), stringTo<double>(sp[2]), usr.getPermission()); break;
		case 9:
			std::cout << usr.getName() << "> ISBN:";
			std::getline(std::cin, sp[1]);
			if (sp[1].length() == 0) sp[1] = emptyStr;
			std::cout << usr.getName() << "> name:";
			std::getline(std::cin, sp[2]);
			if (sp[2].length() == 0) sp[2] = emptyStr;
			std::cout << usr.getName() << "> author:";
			std::getline(std::cin, sp[3]);
			if (sp[3].length() == 0) sp[3] = emptyStr;
			std::cout << usr.getName() << "> keyword:";
			std::getline(std::cin, sp[4]);
			if (sp[4].length() == 0) sp[4] = emptyStr;
			book.show(record(sp[1], sp[2], sp[3], sp[4], price), usr.getPermission());
		case 10:
			std::cout << usr.getName() << "> ISBN:";
			std::getline(std::cin, sp[1]);
			std::cout << usr.getName() << "> quantity:";
			std::getline(std::cin, sp[2]);
			book.buy(sp[1], stringTo<int>(sp[2]), usr.getPermission()); break;
		case 11:
			exit(0);
		case 12:
			std::cout << "su (log in)" << std::endl;
			std::cout << "logout (logout)" << std::endl;
			std::cout << "useradd (add a user)" << std::endl;
			std::cout << "register (register your account)" << std::endl;
			std::cout << "delete (delete an account)" << std::endl;
			std::cout << "passwd (change the password)" << std::endl;
			std::cout << "select (select a book)" << std::endl;
			std::cout << "modify (modify the infomation of current book)" << std::endl;
			std::cout << "import (import current book)" << std::endl;
			std::cout << "show (show the information)" << std::endl;
			std::cout << "show finance (show finance information)" << std::endl;
			std::cout << "buy (buy the book)" << std::endl;
			break;
		case 13:
			std::cout << usr.getName() << "> time:";
			std::getline(std::cin, sp[1]);
			if (sp[1].length() == 0)
				book.showFinance(usr.getPermission());
			else
				book.showFinance(usr.getPermission(), stringTo<int>(sp[1]));
			break;
		}
	}
	catch (ErrorException &ex) {
		std::cout << ex.getMessage() << std::endl;
	}
}