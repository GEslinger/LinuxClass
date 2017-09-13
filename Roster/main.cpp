#include <iostream>
#include "Roster.cpp"

// addStudent takes firstname, lastname and returns the ID
// delStudent takes an ID
// addGrade take ID and grade
// delGrade takes an ID and the grade index
// changeGrade takes an ID, the grade index, and the new grade
// displayStudent takes an ID

int main(){
	cout << "Grade format is index:grade\n\n";

	Roster r("C++");
	r.addStudent("Duncan", "Harmon");
	r.addStudent("Patrick", "Smith");
	r.addStudent("Griffin", "Eslinger");
	r.addStudent("Michael", "Gresko");
	r.addStudent("Darth", "Vader");
	r.delStudent(1);
	r.addGrade(0,10);
	r.addGrade(0,90);
	r.addGrade(0,93);
	r.displayStudent(0);
	r.changeGrade(0,1,95);

	r.displayRoster();

	return 0;
}