/*
	Monkey Class
*/
#ifndef MONKEY_H
#define MONKEY_H

#include <iostream>
#include "animal.h"

class Monkey : public Animal {
public:
	Monkey(std::string name);
	std::string get_name();

private:
	// Note that monkeys can also have extra member variables in addition
	// to the ones that are inherited.
	int longest_jump;
};
#endif
