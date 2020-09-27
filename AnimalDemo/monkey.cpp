/*
	Monkey Class
*/
#include <iostream>
#include "monkey.h"

using namespace std;

// this Monkey constructor shows how we can invoke an
// alternate Animal constructor when the Monkey is created
Monkey::Monkey(string name) : Animal(name, 104) {
	cout << "Default monkey constructor called" << endl;

	// set the longest jump
	longest_jump = 5;
}

string Monkey::get_name() {
	// we have overridden the get_name function
	// instead of the plain name, we will add an honorary title
	string monkey_name = "Honorable " + Animal::get_name();
	return monkey_name;
}
