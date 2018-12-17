//
// Created by mintgreen on 18-12-15.
//

#ifndef BOOKSTORE_ERROR_H
#define BOOKSTORE_ERROR_H

#include <string>

class Error {
	std::string msg;
public:
	Error(std::string msg);
	~Error() throw();
	std::string getMessage();
};

void error(std::string msg) {
	throw Error(msg);
}
#endif //BOOKSTORE_ERROR_H