/*
	Written by Justin Goins as an in-class demo of inheritance
	Your homework submissions should always include your name, the date,
	and information about the assignment.
*/
#include <iostream>
#include "animal.h"
#include "monkey.h"

using namespace std;

/*
  Illustrate how to utilize inheritance
*/
int main() {

	// demonstrate how functions work with different types of objects
	cout << "<Started to create an Animal>" << endl;
	Animal a1("Scrawny Leopard");
	cout << "<Finished creating an Animal>" << endl << endl;

	cout << "<Started to create a Monkey>" << endl;
	Monkey m1("Curious George");
	cout << "<Finished creating a Monkey>" << endl << endl;

	// now we can use the inherited variables and functions
	cout << "Calling get_name() on animal: " << a1.get_name() << endl;
	cout << "Calling get_name() on monkey: " << m1.get_name() << endl;

	// note that the Monkey version of get_name() was used on m1.

	return 0;
}
