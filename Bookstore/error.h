//
// Created by mintgreen on 18-12-15.
//

#ifndef BOOKSTORE_ERROR_H
#define BOOKSTORE_ERROR_H

#include <string>

class ErrorException {
	std::string msg;
public:
	ErrorException(std::string msg);
	~ErrorException() throw();
	std::string getMessage();
};

void error(std::string msg) {
	throw ErrorException(msg);
}
#endif //BOOKSTORE_ERROR_H