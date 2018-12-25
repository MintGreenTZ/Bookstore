//
// Created by mintgreen on 18-12-17.
//

//a class which stores the current user ,organize the database and deal with commands associated with users

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
	const int getPermission() const;
	const std::string getName() const;
	void log(std::string userID, std::string password = emptyStr);
	void logout();
	void useradd(std::string userID, std::string password, int permission, std::string name);
	void Register(std::string userID, std::string password, std::string name);
	void del(std::string userID);
	void modifyPassword(std::string userID, std::string newPassword, std::string oldPassword = emptyStr);
};

#endif //BOOKSTORE_USER_H
