//
// Created by mintgreen on 18-12-17.
//

#include "user.h"
#include "error.h"

user::user():userDB("userDatabase") {
	userDB.add(userInfo("root", "root", "sjtu", 7));
	this->name = "root";
	this->permission = 7;
}

user::~user() {

}

const int user::getPermission() const { return permission; }

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