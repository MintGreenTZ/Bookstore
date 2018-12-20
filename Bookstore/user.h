//
// Created by mintgreen on 18-12-17.
//

#ifndef BOOKSTORE_USER_H
#define BOOKSTORE_USER_H

#include <string>
#include <iostream>
#include "tools.h"
#include "error.h"
#include "database.h"

class user {
	standString name;
	int permission;
	database<userInfo> userDB;
public:
	user();
	~user();
	void log(std::string userID, std::string password = "");
	void logout();
	void useradd(std::string userID, std::string password, int permission, std::string name);
	void Register(std::string userID, std::string password, std::string name);
	void del(std::string userID);
	void modifyPassword(std::string userID, std::string newPassword, std::string oldPassword = "");
};

#endif //BOOKSTORE_USER_H
