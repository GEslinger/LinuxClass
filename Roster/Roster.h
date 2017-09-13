#ifndef ROSTER_H
#define ROSTER_H
#include <string>
#include <vector>
#include <map>
#include "Student.cpp"
using namespace std;

class Roster{
public:
	Roster(string n);
	int addStudent(string f, string l);
	bool delStudent(int i);
	bool addGrade(int i, double g);
	bool delGrade(int i, int g);
	bool changeGrade(int i, int g, double newG);
	bool removeStudent(int i);
	void displayStudent(int i);
	void displayRoster();
	~Roster();
private:
	bool isStudent(int i);
	int acc;
	string courseName;
	map<int, Student*> students;
};

#endif