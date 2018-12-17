//
// Created by mintgreen on 18-12-15.
//

#include <string>
#include "error.h"

Error::Error(std::string msg) {
	this->msg = msg;
}

Error::~Error() throw() {}

std::string Error::getMessage() {
	return msg;
}