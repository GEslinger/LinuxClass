#include <iostream>
#include "Roster.h"
using namespace std;

Roster::Roster(string n){
	courseName = n;
	acc = 0;
}

bool Roster::isStudent(int i){
	if(students.count(i)){
		return true;
	}
	return false;
}


int Roster::addStudent(string f, string l){
	Student* s = new Student(f,l);
	students.insert(pair<int,Student*>(acc,s));

	cout << "New student " << f << " " << l << " added to ";
	cout << courseName << " with ID " << acc << endl;

	int spot = acc;
	acc++;

	return spot;
}


bool Roster::delStudent(int i){
	if(!isStudent(i)){
		cout << "No student of that ID!" << endl;
		return false;
	}

	Student* s = students.find(i)->second;
	cout << "Attempting to delete " << s->fName() << " " << s->lName() << endl;

	delete s;
	students.erase(i);

	cout << "Deletion successful" << endl;

	return true;
}


bool Roster::addGrade(int i, double g){
	if(!isStudent(i)){
		cout << "No student of that ID!" << endl;
		return false;
	}

	Student* s = students.at(i);
	s->addGrade(g);

	return true;
}


bool Roster::delGrade(int i, int g){
	if(!isStudent(i)){
		cout << "No student of that ID!" << endl;
		return false;
	}
	Student* s = students.at(i);
	
	return s->delGrade(g);	
}



bool Roster::changeGrade(int i, int g, double newG){
	if(!isStudent(i)){
		cout << "No student of that ID!" << endl;
		return false;
	}
	Student* s = students.at(i);

	return s->changeGrade(g,newG);
}


void Roster::displayStudent(int i){
	if(!isStudent(i)){
		cout << "No student of that ID!" << endl;
		return;
	}

	Student* student = students.at(i);
	cout << student->fName() << " " << student->lName() << endl;
	student->printGrades();
	cout << "Average: " << student->getAvg();
	cout << endl << endl;
}


void Roster::displayRoster(){
	for(map<int,Student*>::iterator s = students.begin(); s != students.end(); s++){
		Student* student = s->second;
		cout << student->fName() << " " << student->lName() << endl;
		student->printGrades();
		cout << "Average: " << student->getAvg();
		cout << endl << endl;
	}
}


Roster::~Roster(){
	for(map<int,Student*>::iterator s = students.begin(); s != students.end(); s++){
		delete s->second;
		//cout << "Deleted student " << s->first << endl;
	}
}

