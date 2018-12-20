//
// Created by mintgreen on 18-12-17.
//

#include "user.h"
#include "error.h"

user::user():userDB("userDatabase") {
	userDB.add(userInfo("root", "root", "sjtu", 7));
}

user::~user() {

}

void user::log(std::string userID, std::string password) {
	userInfo info = userDB.ask(userInfo(userID));
	if (info.getPermission() < permission || info.check(password)) {
		name = info.getName();
		permission = info.getPermission();
	}
	else 
		error("Invalid");
}

void user::logout() {
	permission = 0;
}

void user::useradd(std::string userID, std::string password, int permission, std::string name) {
	if (permission >= this->permission) error("Invalid");
	userDB.add(userInfo(userID, password, name, permission));
}

void user::Register(std::string userID, std::string password, std::string name) {
	userDB.add(userInfo(userID, password, name, 1));
}

void user::del(std::string userID) {
	if (permission < 7) error("Invalid");
	userDB.del(userInfo(userID));
}

void user::modifyPassword(std::string userID, std::string newPassword, std::string oldPassword) {
	userDB.modify(userInfo(userID, oldPassword, newPassword, permission == 7));
}