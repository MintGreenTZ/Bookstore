//
// Created by mintgreen on 18-12-15.
//

#include <string>
#include "error.h"

ErrorException::ErrorException(std::string msg) {
	this->msg = msg;
}

ErrorException::~ErrorException() throw() {}

std::string ErrorException::getMessage() {
	return msg;
}