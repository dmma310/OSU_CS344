/*
	Animal Class
*/
#ifndef ANIMAL_H
#define ANIMAL_H

#include <iostream>

class Animal {
public:
	Animal();
	Animal(std::string);
	Animal(std::string, int);
	std::string get_name();

private:
	std::string name;

protected:
	int age;

};
#endif
