/*
	Animal Class
*/
#include <iostream>
#include "animal.h"

using namespace std;

Animal::Animal(string name) {
	cout << "Alternate animal constructor called" << endl;
	this->name = name;
	this->age = 0;
}

Animal::Animal(string name, int age) {
	cout << "Alternate animal constructor called" << endl;
	this->name = name;
	this->age = age;
}

Animal::Animal() {
	cout << "Default animal constructor called" << endl;
	this->name = "Specimen Unknown";
	this->age = 0;
}

string Animal::get_name() {
	return name;
}
