//
// Created by mintgreen on 18-12-17.
//

#include "user.h"
#include "error.h"

user::user():userDB("userDatabase") {
	std::fstream file;
	file.open("curUser", std::fstream::in | std::fstream::out | std::fstream::binary);
	if (file) {
		file.seekg(0);
		file.read(reinterpret_cast<char *> (&name), sizeof(standString));
		file.read(reinterpret_cast<char *> (&permission), sizeof(int));
	}
	else {
		std::ofstream ost("curUser");
		ost.close();
		file.open("curUser", std::fstream::in | std::fstream::out | std::fstream::binary);
		file.seekp(0);
		name = "root";
		permission = 7;
		file.write(reinterpret_cast<const char *> (&name), sizeof(standString));
		file.write(reinterpret_cast<const char *> (&permission), sizeof(int));
		userDB.add(userInfo("root", "root", "sjtu", 7));
	}
}

user::~user() {

}

const int user::getPermission() const { return permission; }

const std::string user::getName() const {
	return name.toString();
}

void user::log(std::string userID, std::string password) {
	userInfo info = userDB.ask(userID);
	if (info.getPermission() < permission || password != emptyStr && info.check(password)) {
		name = info.getName();
		permission = info.getPermission();
	}
	else 
		error("Invalid");
}

void user::logout() {
	if (this->permission < 1) error("Invalid");
	this->name = "";
	this->permission = 0;
}

void user::useradd(std::string userID, std::string password, int permission, std::string name) {
	if (this->permission <= permission || this->permission < 3) error("Invalid");
	userDB.add(userInfo(userID, name, password, permission));
}

void user::Register(std::string userID, std::string password, std::string name) {
	userDB.add(userInfo(userID, name, password, 1));
}

void user::del(std::string userID) {
	if (permission < 7) error("Invalid");
	userDB.del(userInfo(userID));
}

void user::modifyPassword(std::string userID, std::string newPassword, std::string oldPassword) {
	userDB.modify(userInfo(userID, oldPassword, newPassword, permission == 7));
}