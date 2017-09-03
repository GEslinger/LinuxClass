#include <iostream>
using namespace std;

int main(){
	while(true){
		int* please = new int;
		*please = 5;
		please = NULL;
		delete please;
	}

	// int* p = new int;

	// *p = 5;

	// cout << *p << "\n";
	// cout << p << "\n";

	// int *q;
	// q = new int;

	// *q = 5;

	// int* m = q;

	// if(m == q){
	// 	cout << "IT WORKZ\n";
	// }

	// delete p,q;
}